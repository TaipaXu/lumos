#include "./core.hpp"
#include <iostream>
#include <filesystem>
#include <thread>
#include <chrono>
#include "./threadSafeQueue.hpp"
#include "./magicWrapper.hpp"
#include "analysis/analyzer.hpp"
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
    std::vector<Model::CodeStats> threadStats(threadCount);

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
                auto optFilePath = fileQueue.pop();
                if (!optFilePath)
                {
                    break;
                }

                std::string filePath = std::move(*optFilePath);

                if (!isBinaryFile(filePath))
                {
                    Model::CodeStats stats = processFile(filePath);
                    threadStats[i] += stats;
                }
            }
        });
    }

    for (auto &worker : workers)
    {
        worker.join();
    }

    Model::CodeStats totalStats;
    for (const auto &stats : threadStats)
    {
        totalStats += stats;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Function execution time: " << Utils::formatDuration(duration) << std::endl;

    std::cout << "Total lines: " << totalStats.totalLineCount << std::endl;
    std::cout << "Code lines: " << totalStats.codeLineCount << std::endl;
    std::cout << "Comment lines: " << totalStats.commentLineCount << std::endl;
    std::cout << "Empty lines: " << totalStats.emptyLineCount << std::endl;
}

Model::CodeStats Core::processFile(std::string path) const
{
    Analyzer *analyzer = Analyzer::create(path);
    Model::CodeStats stats;
    if (analyzer)
    {
        stats += analyzer->start(path);
        delete analyzer;
    }
    return stats;
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
