#pragma once

namespace Model
{
    struct CodeStats
    {
        int totalLines = 0;
        int codeLines = 0;
        int commentLines = 0;
        int emptyLines = 0;

        CodeStats operator+(const CodeStats &other) const
        {
            CodeStats result;
            result.totalLines = this->totalLines + other.totalLines;
            result.codeLines = this->codeLines + other.codeLines;
            result.commentLines = this->commentLines + other.commentLines;
            result.emptyLines = this->emptyLines + other.emptyLines;
            return result;
        }

        CodeStats &operator+=(const CodeStats &other)
        {
            this->totalLines += other.totalLines;
            this->codeLines += other.codeLines;
            this->commentLines += other.commentLines;
            this->emptyLines += other.emptyLines;
            return *this;
        }
    };
} // namespace Model
