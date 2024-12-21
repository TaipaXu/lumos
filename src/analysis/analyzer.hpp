#pragma once

#include <string>
#include <vector>
#include "./iAnalyzer.hpp"
#include "models/result.hpp"

class Analyzer : public IAnalyzer
{
public:
    struct MultiLineComment
    {
        std::string start;
        std::string end;
    };

public:
    Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols, const std::vector<MultiLineComment> &multiLineCommentSymbols);
    Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols);
    Analyzer(const std::string &name, const std::vector<MultiLineComment> &multiLineCommentSymbols);
    ~Analyzer() = default;

private:
    Model::CodeStats analyzeStream(std::istream &in) const override;
    bool isSingleLineComment(const std::string &line) const;
    bool isMultiLineCommentStart(const std::string &line, std::string &multilineCommentStart) const;
    bool isMultiLineCommentEnd(const std::string &line, const std::string &multilineCommentStart) const;
    bool isMultiLineCommentInOneLine(const std::string &line) const;

private:
    const std::vector<std::string> singleLineCommentSymbols;
    const std::vector<MultiLineComment> multiLineCommentSymbols;
    const bool hasSingleLineCommentSymbols;
    const bool hasMultiLineCommentSymbols;
};
