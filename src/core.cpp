#include "./core.hpp"
#include <iostream>
#include <list>
#include <unordered_map>
#include <filesystem>
#include <thread>
#include <chrono>
#include "./threadSafeQueue.hpp"
#include "./magicWrapper.hpp"
#include "analysis/analyzer.hpp"
#include "printer/table.hpp"
#include "utils/utils.hpp"

void Core::start(const std::vector<std::string> &paths)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    std::vector<std::string> collectedPaths;
    for (const auto &path : paths)
    {
        std::filesystem::path fsPath(path);

        if (!std::filesystem::exists(fsPath))
        {
            continue;
        }

        if (std::filesystem::is_regular_file(fsPath))
        {
            collectedPaths.push_back(path);
        }
        else if (std::filesystem::is_directory(fsPath))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(fsPath))
            {
                if (std::filesystem::is_regular_file(entry.path()))
                {
                    collectedPaths.push_back(entry.path().string());
                }
            }
        }
    }
    std::cout << "Total files found: " << collectedPaths.size() << std::endl;

    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0)
    {
        threadCount = 4;
    }

    ThreadSafeQueue<std::string> fileQueue;
    std::vector<std::thread> workers;
    std::vector<std::list<Model::CodeStats>> threadStats(threadCount);

    for (const auto &filePath : collectedPaths)
    {
        fileQueue.push(filePath);
    }
    fileQueue.close();

    for (unsigned int i = 0; i < threadCount; ++i)
    {
        workers.emplace_back([&fileQueue, &threadStats, i, this]() {
            while (true)
            {
                std::optional<std::string> optionalFilePath = fileQueue.pop();
                if (!optionalFilePath)
                {
                    break;
                }

                std::string filePath = std::move(*optionalFilePath);
                if (!isBinaryFile(filePath))
                {
                    std::optional<Model::CodeStats> stats = processFile(filePath);
                    if (stats)
                    {
                        threadStats[i].push_back(*stats);
                    }
                }
            }
        });
    }
    for (auto &worker : workers)
    {
        worker.join();
    }

    std::unordered_map<std::string, Model::CodeStats> mergedStatsMap;
    for (const auto &threadList : threadStats)
    {
        for (const auto &stats : threadList)
        {
            auto it = mergedStatsMap.find(stats.title);
            if (it != mergedStatsMap.end())
            {
                it->second += stats;
            }
            else
            {
                mergedStatsMap[stats.title] = stats;
            }
        }
    }

    std::vector<Model::CodeStats> mergedStats;
    mergedStats.reserve(mergedStatsMap.size());
    for (const auto &pair : mergedStatsMap)
    {
        mergedStats.push_back(pair.second);
    }

    Model::CodeStats totalStats;
    for (const auto &stats : mergedStats)
    {
        totalStats += stats;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time taken: " << Utils::formatDuration(duration) << std::endl;

    Printer::Table table;
    table.setHeader({"Language", "Empty Lines", "Comment Lines", "Code Lines", "Total Lines"});
    for (const auto &stats : mergedStats)
    {
        table.addRow({stats.title,
                      std::to_string(stats.emptyLineCount),
                      std::to_string(stats.commentLineCount),
                      std::to_string(stats.codeLineCount),
                      std::to_string(stats.totalLineCount)});
    }
    table.setSummary({"Summary",
                      std::to_string(totalStats.emptyLineCount),
                      std::to_string(totalStats.commentLineCount),
                      std::to_string(totalStats.codeLineCount),
                      std::to_string(totalStats.totalLineCount)});
    table.print();
}

std::optional<Model::CodeStats> Core::processFile(std::string path) const
{
    Analyzer *analyzer = Analyzer::create(path);
    if (analyzer)
    {
        Model::CodeStats stats;
        stats = analyzer->start(path);
        return stats;
    }
    else
    {
        return std::nullopt;
    }
}

bool Core::isBinaryFile(const std::string &path) const
{
    try
    {
        thread_local MagicWrapper magicWrapper;

        const char *mime_type = magic_file(magicWrapper.getMagicCookie(), path.c_str());
        if (mime_type == nullptr)
        {
            std::cerr << "Cannot determine MIME type: " << magic_error(magicWrapper.getMagicCookie()) << std::endl;
            return false;
        }

        bool isBinary = (std::string(mime_type).find("text") == std::string::npos);

        return isBinary;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}
