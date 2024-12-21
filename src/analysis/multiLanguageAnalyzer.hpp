#pragma once

#include <vector>
#include "./iAnalyzer.hpp"
#include "./analyzer.hpp"

class MultiLanguageAnalyzer : public IAnalyzer
{
public:
    struct LanguageBlock
    {
        std::string start;
        std::string end;
        IAnalyzer *analyzer;
    };

public:
    MultiLanguageAnalyzer(const std::string &name, const std::vector<LanguageBlock> &languageBlocks, IAnalyzer *defaultAnalyzer = nullptr);
    ~MultiLanguageAnalyzer() = default;

private:
    Model::CodeStats analyzeStream(std::istream &in) const override;

private:
    std::vector<LanguageBlock> languageBlocks;
    IAnalyzer *defaultAnalyzer;
};
