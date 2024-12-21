#include "./multiLanguageAnalyzer.hpp"
#include <fstream>
#include <algorithm>
#include <boost/algorithm/string.hpp>

MultiLanguageAnalyzer::MultiLanguageAnalyzer(const std::string &name, const std::vector<MultiLanguageAnalyzer::LanguageBlock> &languageBlocks, IAnalyzer *defaultAnalyzer)
    : IAnalyzer{name}, languageBlocks{languageBlocks}, defaultAnalyzer{defaultAnalyzer}
{
}

Model::CodeStats MultiLanguageAnalyzer::analyzeStream(std::istream &in) const
{
    Model::CodeStats allStats;
    allStats.name = name;

    std::string defaultLanguageContent;
    std::string languageContent;
    bool inLanguageBlock = false;
    std::string lastBlockLanguageEnd;

    std::string line;
    while (std::getline(in, line))
    {
        std::string trimmed = boost::algorithm::trim_copy(line);

        if (inLanguageBlock)
        {
            if (line.ends_with(lastBlockLanguageEnd))
            {
                if (defaultAnalyzer)
                {
                    defaultLanguageContent += line + "\n";
                }
                else
                {
                    allStats.codeLineCount++;
                    allStats.totalLineCount++;
                }

                inLanguageBlock = false;
                for (const auto &block : languageBlocks)
                {
                    if (block.end == lastBlockLanguageEnd)
                    {
                        allStats += block.analyzer->startFromContent(languageContent);
                        lastBlockLanguageEnd.clear();
                        break;
                    }
                }
                languageContent.clear();
            }
            else
            {
                languageContent += line + "\n";
            }
        }
        else
        {
            if (defaultAnalyzer == nullptr && trimmed.empty())
            {
                allStats.emptyLineCount++;
                allStats.totalLineCount++;
                continue;
            }

            for (const auto &block : languageBlocks)
            {
                if (trimmed.starts_with(block.start) && trimmed.ends_with(block.end))
                {
                    if (!defaultAnalyzer)
                    {
                        allStats.codeLineCount++;
                        allStats.totalLineCount++;
                    }
                }
                else if (trimmed.starts_with(block.start))
                {
                    inLanguageBlock = true;
                    lastBlockLanguageEnd = block.end;
                    if (defaultAnalyzer)
                    {
                        defaultLanguageContent += line + "\n";
                    }
                    else
                    {
                        allStats.codeLineCount++;
                        allStats.totalLineCount++;
                    }
                }
            }
            if (defaultAnalyzer && !inLanguageBlock)
            {
                defaultLanguageContent += line + "\n";
            }
        }
    }

    if (defaultAnalyzer && !defaultLanguageContent.empty())
    {
        allStats += defaultAnalyzer->startFromContent(defaultLanguageContent);
    }

    return allStats;
}
