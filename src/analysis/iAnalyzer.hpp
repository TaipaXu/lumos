#pragma once

#include <string>
#include "models/result.hpp"

class IAnalyzer
{
public:
    IAnalyzer(const std::string &name);
    virtual ~IAnalyzer() = default;

    static IAnalyzer *create(const std::string &fileName);
    Model::CodeStats start(std::string &path) const;
    Model::CodeStats startFromContent(const std::string &content) const;

protected:
    virtual Model::CodeStats analyzeStream(std::istream &in) const = 0;

protected:
    const std::string name;
};
