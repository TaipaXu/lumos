#pragma once

#include <string>
#include <string_view>
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
    bool isSingleLineComment(std::string_view line) const;
    bool isMultiLineCommentStart(std::string_view line, std::string &multilineCommentStart) const;
    bool isMultiLineCommentEnd(std::string_view line, std::string_view multilineCommentStart) const;
    bool isMultiLineCommentInOneLine(std::string_view line) const;

private:
    const std::vector<std::string> singleLineCommentSymbols;
    const std::vector<MultiLineComment> multiLineCommentSymbols;
    const bool hasSingleLineCommentSymbols;
    const bool hasMultiLineCommentSymbols;
};
