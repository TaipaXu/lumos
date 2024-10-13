#pragma once

#include <string>
#include <vector>
#include "models/result.hpp"

class Analyzer
{
public:
    struct MultiLineComment
    {
        std::string start;
        std::string end;
    };

public:
    Analyzer(const std::vector<std::string> &singleLineCommentSymbols, const std::vector<MultiLineComment> &multiLineCommentSymbols);
    Analyzer(const std::vector<std::string> &singleLineCommentSymbols);
    Analyzer(const std::vector<MultiLineComment> &multiLineCommentSymbols);
    ~Analyzer() = default;
    static Analyzer *create(const std::string &fileName);

    Model::CodeStats start(std::string &path) const;

protected:
    const std::vector<std::string> singleLineCommentSymbols;
    const std::vector<MultiLineComment> multiLineCommentSymbols;

private:
    bool isSingleLineComment(const std::string &line) const;
    bool isMultiLineCommentStart(const std::string &line, std::string &multilineCommentStart) const;
    bool isMultiLineCommentEnd(const std::string &line, const std::string &multilineCommentStart) const;
    bool isMultiLineCommentInOneLine(const std::string &line) const;

private:
    const bool hasSingleLineCommentSymbols;
    const bool hasMultiLineCommentSymbols;
};
