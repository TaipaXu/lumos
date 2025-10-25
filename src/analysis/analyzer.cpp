#include "./analyzer.hpp"
#include <fstream>
#include <algorithm>
#include "utils/utils.hpp"

Analyzer::Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols, const std::vector<MultiLineComment> &multiLineCommentSymbols)
    : IAnalyzer{name},
      singleLineCommentSymbols{singleLineCommentSymbols},
      multiLineCommentSymbols{multiLineCommentSymbols},
      hasSingleLineCommentSymbols{!singleLineCommentSymbols.empty()},
      hasMultiLineCommentSymbols{!multiLineCommentSymbols.empty()}
{
}

Analyzer::Analyzer(const std::string &name, const std::vector<std::string> &singleLineCommentSymbols)
    : IAnalyzer{name},
      singleLineCommentSymbols{singleLineCommentSymbols},
      hasSingleLineCommentSymbols{!singleLineCommentSymbols.empty()},
      hasMultiLineCommentSymbols{false}
{
}

Analyzer::Analyzer(const std::string &name, const std::vector<MultiLineComment> &multiLineCommentSymbols)
    : IAnalyzer{name},
      multiLineCommentSymbols{multiLineCommentSymbols},
      hasSingleLineCommentSymbols{false},
      hasMultiLineCommentSymbols{!multiLineCommentSymbols.empty()}
{
}

Model::CodeStats Analyzer::analyzeStream(std::istream &in) const
{
    Model::CodeStats stats;
    stats.name = name;
    std::string line;
    bool inCommentBlock = false;
    std::string lastMultiLineCommentStart;

    while (std::getline(in, line))
    {
        stats.totalLineCount++;
        std::string_view trimmed = Utils::trimView(line);

        if (trimmed.empty())
        {
            stats.emptyLineCount++;
        }
        else
        {
            if (hasMultiLineCommentSymbols)
            {
                if (inCommentBlock)
                {
                    stats.commentLineCount++;

                    if (isMultiLineCommentEnd(trimmed, lastMultiLineCommentStart))
                    {
                        inCommentBlock = false;
                        lastMultiLineCommentStart.clear();
                    }
                }
                else
                {
                    if (isMultiLineCommentInOneLine(trimmed))
                    {
                        stats.commentLineCount++;
                    }
                    else
                    {
                        if (isMultiLineCommentStart(trimmed, lastMultiLineCommentStart))
                        {
                            stats.commentLineCount++;
                            inCommentBlock = true;
                        }
                        else
                        {
                            if (hasSingleLineCommentSymbols && isSingleLineComment(trimmed))
                            {
                                stats.commentLineCount++;
                            }
                            else
                            {
                                stats.codeLineCount++;
                            }
                        }
                    }
                }
            }
            else
            {
                if (hasSingleLineCommentSymbols && isSingleLineComment(trimmed))
                {
                    stats.commentLineCount++;
                }
                else
                {
                    stats.codeLineCount++;
                }
            }
        }
    }

    return stats;
}

bool Analyzer::isSingleLineComment(std::string_view line) const
{
    for (const auto &symbol : singleLineCommentSymbols)
    {
        if (line.starts_with(symbol))
        {
            return true;
        }
    }
    return false;
}

bool Analyzer::isMultiLineCommentStart(std::string_view line, std::string &multilineCommentStart) const
{
    for (const auto &symbol : multiLineCommentSymbols)
    {
        if (line.starts_with(symbol.start))
        {
            multilineCommentStart = symbol.start;
            return true;
        }
    }
    return false;
}

bool Analyzer::isMultiLineCommentEnd(std::string_view line, std::string_view multilineCommentStart) const
{
    auto it = std::find_if(multiLineCommentSymbols.begin(), multiLineCommentSymbols.end(), [&](const MultiLineComment &symbol) { return symbol.start == multilineCommentStart; });
    if (it != multiLineCommentSymbols.end())
    {
        return line.ends_with(it->end);
    }
    return false;
}

bool Analyzer::isMultiLineCommentInOneLine(std::string_view line) const
{
    for (const MultiLineComment &symbol : multiLineCommentSymbols)
    {
        size_t startPos = line.find(symbol.start);
        size_t endPos = line.find(symbol.end, startPos + symbol.start.length());
        if (startPos != std::string::npos && endPos != std::string::npos && startPos < endPos)
        {
            return true;
        }
    }
    return false;
}
