#pragma once

#include <string>
#include <vector>
#include "models/result.hpp"

class Core
{
public:
    Core() = default;
    ~Core() = default;

    void start(const std::vector<std::string> &paths);

private:
    void precessFile(std::string path);
    bool isBinaryFile(const std::string &path) const;

private:
    Model::CodeStats totalStats;
};
