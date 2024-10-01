#pragma once

namespace Model
{
    struct CodeStats
    {
        int totalLineCount = 0;
        int codeLineCount = 0;
        int commentLineCount = 0;
        int emptyLineCount = 0;

        CodeStats operator+(const CodeStats &other) const
        {
            CodeStats result;
            result.totalLineCount = this->totalLineCount + other.totalLineCount;
            result.codeLineCount = this->codeLineCount + other.codeLineCount;
            result.commentLineCount = this->commentLineCount + other.commentLineCount;
            result.emptyLineCount = this->emptyLineCount + other.emptyLineCount;
            return result;
        }

        CodeStats &operator+=(const CodeStats &other)
        {
            this->totalLineCount += other.totalLineCount;
            this->codeLineCount += other.codeLineCount;
            this->commentLineCount += other.commentLineCount;
            this->emptyLineCount += other.emptyLineCount;
            return *this;
        }
    };
} // namespace Model
