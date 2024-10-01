#include "./core.hpp"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <chrono>
#include <magic.h>
#include "./analyzer.hpp"

void Core::start(const std::vector<std::string> &paths)
{
    const auto startTime = std::chrono::high_resolution_clock::now();

    int count = 0;
    for (const std::string &path : paths)
    {
        std::filesystem::path fsPath(path);

        if (!std::filesystem::exists(fsPath))
        {
            return;
        }

        if (std::filesystem::is_regular_file(fsPath))
        {
            if (!isBinaryFile(path))
            {
                count++;
                precessFile(path);
            }
        }
        else if (std::filesystem::is_directory(fsPath))
        {
            for (const auto &entry : std::filesystem::recursive_directory_iterator(fsPath))
            {
                if (std::filesystem::is_regular_file(entry.path()))
                {
                    if (!isBinaryFile(entry.path()))
                    {
                        count++;
                        precessFile(entry.path());
                    }
                }
            }
        }
    }

    std::cout << "Total files found: " << count << std::endl;

    const auto endTime = std::chrono::high_resolution_clock::now();
    const auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);
    std::cout << "Function execution time: " << duration.count() << " milliseconds" << std::endl;

    std::cout << "Total lines: " << totalStats.totalLines << std::endl;
    std::cout << "Code lines: " << totalStats.codeLines << std::endl;
    std::cout << "Comment lines: " << totalStats.commentLines << std::endl;
    std::cout << "Empty lines: " << totalStats.emptyLines << std::endl;
}

void Core::precessFile(std::string path)
{
    Analyzer *analyzer = Analyzer::create(path);
    if (analyzer)
    {
        totalStats += analyzer->start(path);
        delete analyzer;
    }
}

bool Core::isBinaryFile(const std::string &path) const
{
    magic_t magicCookie = magic_open(MAGIC_MIME_TYPE);
    if (magicCookie == nullptr)
    {
        // std::cerr << "Unable to initialize magic library" << std::endl;
        return false;
    }

    if (magic_load(magicCookie, "/usr/share/file/magic.mgc") != 0)
    {
        // std::cerr << "Cannot load magic database: " << magic_error(magicCookie) << std::endl;
        magic_close(magicCookie);
        return false;
    }

    const char *mime_type = magic_file(magicCookie, path.c_str());
    if (mime_type == nullptr)
    {
        // std::cerr << "Cannot determine MIME type: " << magic_error(magicCookie) << std::endl;
        magic_close(magicCookie);
        return false;
    }

    bool isBinary = (std::string(mime_type).find("text") == std::string::npos);

    magic_close(magicCookie);

    return isBinary;
}
