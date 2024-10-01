#include "./analyzer.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include "./analyzers.hpp"

Analyzer::Analyzer(const std::vector<std::string> &singleLineCommentSymbols, const std::vector<MultiLineComment> &multiLineCommentSymbols)
    : singleLineCommentSymbols{singleLineCommentSymbols},
      multiLineCommentSymbols{multiLineCommentSymbols},
      hasMultiLineCommentSymbols{!multiLineCommentSymbols.empty()}
{
}

Analyzer *Analyzer::create(const std::string &fileName)
{
    if (fileName.ends_with(".h") || fileName.ends_with(".hpp") || fileName.ends_with(".hxx") || fileName.ends_with(".hh") || fileName.ends_with(".cpp") || fileName.ends_with(".cc") || fileName.ends_with(".cxx") || fileName.ends_with(".c") || fileName.ends_with(".C") || fileName.ends_with(".c++") || fileName.ends_with(".inl") || fileName.ends_with(".ipp") || fileName.ends_with(".tpp") || fileName.ends_with(".txx") || fileName.ends_with(".cppm"))
    {
        return new CppAnalyzer();
    }
    else if (fileName.ends_with(".py"))
    {
        return new PythonAnalyzer();
    }
    else if (fileName.ends_with(".xml") || fileName.ends_with(".htm"))
    {
        return new XMLAnalyzer();
    }
    else if (fileName.ends_with(".html") || fileName.ends_with(".htm"))
    {
        return new HtmlAnalyzer();
    }
    else if (fileName.ends_with(".css"))
    {
        return new CssAnalyzer();
    }
    else if (fileName.ends_with(".sass") || fileName.ends_with(".scss"))
    {
        return new SassAnalyzer();
    }
    else if (fileName.ends_with(".less"))
    {
        return new LessAnalyzer();
    }
    else if (fileName.ends_with(".js"))
    {
        return new JavaScriptAnalyzer();
    }
    else if (fileName.ends_with(".ts"))
    {
        return new TypeScriptAnalyzer();
    }
    else if (fileName.ends_with(".vue"))
    {
        return new VueAnalyzer();
    }
    else if (fileName.ends_with(".json"))
    {
        return new JsonAnalyzer();
    }
    else if (fileName.ends_with(".md"))
    {
        return new MarkdownAnalyzer();
    }
    else if (fileName.ends_with(".java"))
    {
        return new JavaAnalyzer();
    }
    else if (fileName.ends_with(".kt"))
    {
        return new KotlinAnalyzer();
    }
    else if (fileName.ends_with(".dart"))
    {
        return new DartAnalyzer();
    }
    else if (fileName.ends_with(".yaml") || fileName.ends_with(".yml"))
    {
        return new YamlAnalyzer();
    }

    return nullptr;
}

Model::CodeStats Analyzer::start(std::string &path) const
{
    std::ifstream file(path, std::ios::in);
    Model::CodeStats stats;
    std::string line;
    bool inBlockComment = false;
    std::string lastMultiLineCommentStart;

    while (std::getline(file, line))
    {
        stats.totalLineCount++;
        std::string trimmed = boost::algorithm::trim_copy(line);

        if (trimmed.empty())
        {
            stats.emptyLineCount++;
        }
        else
        {
            if (hasMultiLineCommentSymbols)
            {
                if (inBlockComment)
                {
                    stats.commentLineCount++;

                    if (isMultiLineCommentEnd(trimmed, lastMultiLineCommentStart))
                    {
                        inBlockComment = false;
                        lastMultiLineCommentStart.clear();
                    }
                }
                else
                {
                    if (isMulitLineCommentInOneLine(trimmed))
                    {
                        stats.commentLineCount++;
                    }
                    else
                    {
                        if (isMultiLineCommentStart(trimmed, lastMultiLineCommentStart))
                        {
                            stats.commentLineCount++;
                            inBlockComment = true;
                        }
                        else
                        {
                            if (isSingleLineComment(trimmed))
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
                if (isSingleLineComment(trimmed))
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

bool Analyzer::isSingleLineComment(const std::string &line) const
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

bool Analyzer::isMultiLineCommentStart(const std::string &line, std::string &multilineCommentStart) const
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

bool Analyzer::isMultiLineCommentEnd(const std::string &line, const std::string &multilineCommentStart) const
{
    auto it = std::find_if(multiLineCommentSymbols.begin(), multiLineCommentSymbols.end(), [&](const MultiLineComment &symbol) { return symbol.start == multilineCommentStart; });
    if (it != multiLineCommentSymbols.end())
    {
        return line.ends_with(it->end);
    }
    return false;
}

bool Analyzer::isMulitLineCommentInOneLine(const std::string &line) const
{
    for (const auto &symbol : multiLineCommentSymbols)
    {
        if (line.find(symbol.start) != std::string::npos && line.find(symbol.end) != std::string::npos && line.find(symbol.start) < line.find(symbol.end))
        {
            return true;
        }
    }
    return false;
}
