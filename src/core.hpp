#pragma once

#include <string>
#include <vector>
#include <optional>
#include <filesystem>
#include "models/result.hpp"

class Core
{
public:
    Core() = default;
    ~Core() = default;

    void start(const std::vector<std::string> &paths);

private:
    std::optional<Model::CodeStats> processFile(std::string path) const;
    static bool isBinaryFile(const std::string &path);
    static bool isKnownTextExtension(const std::filesystem::path &path);
    static bool looksTextualFile(const std::filesystem::path &path);
};
