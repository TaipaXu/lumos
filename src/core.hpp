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
    Model::CodeStats processFile(std::string path) const;
    bool isBinaryFile(const std::string &path) const;
};
