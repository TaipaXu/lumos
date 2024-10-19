#pragma once

#include <string>
#include <vector>

namespace Printer
{
    class Table
    {
    private:
        enum class Alignment
        {
            Left,
            Right,
            Center
        };

    public:
        Table() = default;
        ~Table() = default;

        void setHeader(const std::vector<std::string> &header);
        void addRow(const std::vector<std::string> &row);
        void setSummary(const std::vector<std::string> &summary);
        void print();

    private:
        std::string formatString(const std::string &value, size_t width, Alignment align = Alignment::Left) const;
        void printSeparator(const std::vector<size_t> &columnWiths) const;

    private:
        std::vector<std::string> header;
        std::vector<std::vector<std::string>> rows;
        std::vector<std::string> summary;
    };
} // namespace Printer
