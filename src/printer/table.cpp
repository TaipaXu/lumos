#include "./table.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace Printer
{
    void Table::setHeader(const std::vector<std::string> &header)
    {
        this->header = header;
    }

    void Table::addRow(const std::vector<std::string> &row)
    {
        this->rows.push_back(row);
    }

    void Table::setSummary(const std::vector<std::string> &summary)
    {
        this->summary = summary;
    }

    void Table::print()
    {
        const size_t maxWidth = 20;
        std::vector<size_t> columnWiths;
        for (const auto &cell : header)
        {
            columnWiths.push_back(std::min(cell.length(), maxWidth));
        }
        for (const auto &row : rows)
        {
            if (columnWiths.size() > 0)
            {
                columnWiths[0] = std::max(columnWiths[0], row[0].length());
            }
        }

        std::cout << "|";
        for (size_t i = 0; i < header.size(); ++i)
        {
            std::string formatted = formatString(header[i], columnWiths[i], Alignment::Center);
            std::cout << " " << formatted << " |";
        }
        std::cout << std::endl;

        printSeparator(columnWiths);

        for (const auto &row : rows)
        {
            std::cout << "|";
            for (size_t i = 0; i < row.size(); ++i)
            {
                Alignment align = i == 0 ? Alignment::Left : Alignment::Right;
                std::string formatted = formatString(row[i], columnWiths[i], align);
                std::cout << " " << formatted << " |";
            }
            std::cout << std::endl;
        }

        printSeparator(columnWiths);

        std::cout << "|";
        for (size_t i = 0; i < summary.size(); ++i)
        {
            Alignment align = i == 0 ? Alignment::Left : Alignment::Right;
            std::string formatted = formatString(summary[i], columnWiths[i], align);
            std::cout << " " << formatted << " |";
        }
        std::cout << std::endl;
    }

    std::string Table::formatString(const std::string &value, size_t width, Alignment align) const
    {
        std::string formatted;
        if (value.length() > width)
        {
            if (width > 3)
            {
                formatted = value.substr(0, width - 3) + "...";
            }
            else
            {
                formatted = value.substr(0, width);
            }
        }
        else if (value.length() == width)
        {
            formatted = value;
        }
        else
        {
            size_t padding = width - value.length();
            if (align == Alignment::Left)
            {
                formatted = value + std::string(padding, ' ');
            }
            else if (align == Alignment::Right)
            {
                formatted = std::string(padding, ' ') + value;
            }
            else
            {
                size_t padLeft = padding / 2;
                size_t padRight = padding - padLeft;
                formatted = std::string(padLeft, ' ') + value + std::string(padRight, ' ');
            }
        }
        return formatted;
    }

    void Table::printSeparator(const std::vector<size_t> &columnWiths) const
    {
        std::cout << "|";
        for (size_t i = 0; i < columnWiths.size(); ++i)
        {
            std::cout << std::string(columnWiths[i] + 2, '-') << "|";
        }
        std::cout << std::endl;
    }
} // namespace Printer
