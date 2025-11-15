#include "./core.hpp"
#include <iostream>
#include <array>
#include <list>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include <fstream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <cctype>
#include "./threadSafeQueue.hpp"
#include "./magicWrapper.hpp"
#include "analysis/analyzer.hpp"
#include "printer/table.hpp"
#include "utils/utils.hpp"

void Core::start(const std::vector<std::string> &paths)
{
    auto startTime = std::chrono::high_resolution_clock::now();

    ThreadSafeQueue<std::string> fileQueue;
    std::size_t totalFiles = 0;

    unsigned int threadCount = std::thread::hardware_concurrency();
    if (threadCount == 0)
    {
        threadCount = 2;
    }
    threadCount = std::max(1u, threadCount);

    std::vector<std::thread> workers;
    std::vector<std::list<Model::CodeStats>> threadStats(threadCount);

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
                if (!Core::isBinaryFile(filePath))
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

    for (const auto &path : paths)
    {
        std::filesystem::path fsPath(path);

        if (!std::filesystem::exists(fsPath))
        {
            continue;
        }

        if (std::filesystem::is_regular_file(fsPath))
        {
            fileQueue.push(fsPath.string());
            ++totalFiles;
        }
        else if (std::filesystem::is_directory(fsPath))
        {
            std::error_code errorCode;
            for (std::filesystem::recursive_directory_iterator it(fsPath, std::filesystem::directory_options::skip_permission_denied, errorCode), end; it != end; it.increment(errorCode))
            {
                if (errorCode)
                {
                    errorCode.clear();
                    continue;
                }

                const auto &entryPath = it->path();
                if (std::filesystem::is_regular_file(entryPath))
                {
                    fileQueue.push(entryPath.string());
                    ++totalFiles;
                }
            }
        }
    }
    fileQueue.close();

    std::cout << "Total files found: " << totalFiles << std::endl;

    for (auto &worker : workers)
    {
        worker.join();
    }

    std::unordered_map<std::string, Model::CodeStats> mergedStatsMap;
    for (const auto &threadList : threadStats)
    {
        for (const auto &stats : threadList)
        {
            auto it = mergedStatsMap.find(stats.name);
            if (it != mergedStatsMap.end())
            {
                it->second += stats;
            }
            else
            {
                mergedStatsMap[stats.name] = stats;
            }

            mergedStatsMap[stats.name].fileCount++;
        }
    }

    std::vector<Model::CodeStats> mergedStats;
    mergedStats.reserve(mergedStatsMap.size());
    for (const auto &pair : mergedStatsMap)
    {
        mergedStats.push_back(pair.second);
    }
    std::sort(mergedStats.begin(), mergedStats.end(), [](const auto &a, const auto &b) {
        return a.name < b.name;
    });

    Model::CodeStats totalStats;
    for (const auto &stats : mergedStats)
    {
        totalStats += stats;
        totalStats.fileCount += stats.fileCount;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Time taken: " << Utils::formatDuration(duration) << std::endl;

    Printer::Table table;
    table.setHeader({"Language", "Files", "Empty Lines", "Comment Lines", "Code Lines", "Total Lines"});
    for (const auto &stats : mergedStats)
    {
        table.addRow({stats.name,
                      {std::to_string(stats.fileCount), Printer::Alignment::Right},
                      {std::to_string(stats.emptyLineCount), Printer::Alignment::Right},
                      {std::to_string(stats.commentLineCount), Printer::Alignment::Right},
                      {std::to_string(stats.codeLineCount), Printer::Alignment::Right, Printer::Color::FgColor::Green},
                      {std::to_string(stats.totalLineCount), Printer::Alignment::Right}});
    }
    table.setSummary({{"Summary"},
                      {std::to_string(totalStats.fileCount), Printer::Alignment::Right},
                      {std::to_string(totalStats.emptyLineCount), Printer::Alignment::Right},
                      {std::to_string(totalStats.commentLineCount), Printer::Alignment::Right},
                      {std::to_string(totalStats.codeLineCount), Printer::Alignment::Right, Printer::Color::FgColor::Red},
                      {std::to_string(totalStats.totalLineCount), Printer::Alignment::Right}});
    table.print();
}

std::optional<Model::CodeStats> Core::processFile(std::string path) const
{
    std::filesystem::path fsPath(path);
    IAnalyzer *analyzer = IAnalyzer::create(fsPath.filename().string());
    if (analyzer)
    {
        return analyzer->start(path);
    }
    else
    {
        return std::nullopt;
    }
}

bool Core::isBinaryFile(const std::string &path)
{
    try
    {
        const std::filesystem::path fsPath(path);
        if (isKnownTextExtension(fsPath))
        {
            if (looksTextualFile(fsPath))
            {
                return false;
            }
        }

        thread_local MagicWrapper magicWrapper;

        const char *mimeType = magic_file(magicWrapper.getMagicCookie(), path.c_str());
        if (mimeType == nullptr)
        {
            std::cerr << "Cannot determine MIME type: " << magic_error(magicWrapper.getMagicCookie()) << std::endl;
            return false;
        }

        const std::string mime(mimeType);
        static const std::array<const char *, 3> textIndicators{"text", "json", "javascript"};
        const bool isTextual = std::any_of(textIndicators.begin(), textIndicators.end(), [&](const char *indicator) {
            return mime.find(indicator) != std::string::npos;
        });

        return !isTextual;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return false;
    }
}

bool Core::isKnownTextExtension(const std::filesystem::path &path)
{
    // Keep list in sync with IAnalyzer::create() extension checks.
    static const std::unordered_set<std::string> textualExtensions = {
        ".adb", ".ads", ".asm", ".bat", ".c", ".c++", ".cbl", ".cc", ".cl", ".cmake", ".cmd", ".cob",
        ".cpp", ".cppm", ".cs", ".css", ".cxx", ".d", ".dart", ".erl", ".f", ".f90", ".for",
        ".fs", ".fsi", ".fsx", ".go", ".h", ".hh", ".hpp", ".hs", ".htm", ".html", ".hxx",
        ".inl", ".ipp", ".java", ".js", ".json", ".kt", ".kts", ".less", ".lisp", ".lsp",
        ".lua", ".m", ".md", ".nim", ".pas", ".php", ".pl", ".py", ".qml", ".r", ".rb",
        ".rs", ".s", ".sass", ".scala", ".scss", ".sh", ".sql", ".swift", ".tcl", ".tpp",
        ".ts", ".txx", ".vb", ".vhd", ".vhdl", ".vue", ".wl", ".xml", ".yaml", ".yml", ".zig"};

    std::string ext = path.extension().string();
    if (ext.empty())
    {
        const std::string filename = path.filename().string();
        return filename == "CMakeLists.txt" || filename == "Dockerfile";
    }

    std::transform(ext.begin(), ext.end(), ext.begin(), [](unsigned char ch) {
        return static_cast<char>(std::tolower(ch));
    });

    return textualExtensions.find(ext) != textualExtensions.end();
}

bool Core::looksTextualFile(const std::filesystem::path &path)
{
    constexpr std::size_t sniffSize = 4096;
    thread_local std::array<char, sniffSize> sniffBuffer{};

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open())
    {
        return false;
    }

    file.read(sniffBuffer.data(), sniffSize);
    const std::streamsize bytesRead = file.gcount();
    if (bytesRead == 0)
    {
        return true;
    }

    int suspiciousCount = 0;
    for (std::streamsize i = 0; i < bytesRead; ++i)
    {
        const unsigned char ch = static_cast<unsigned char>(sniffBuffer[static_cast<std::size_t>(i)]);
        if (ch == 0)
        {
            return false;
        }

        const bool isControl = (ch < 0x07) || (ch > 0x0D && ch < 0x20 && ch != 0x1B);
        if (isControl)
        {
            ++suspiciousCount;
        }
    }

    return suspiciousCount * 100 < static_cast<int>(bytesRead) * 5;
}
