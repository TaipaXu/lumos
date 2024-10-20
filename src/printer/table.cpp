#include "./table.hpp"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace Printer
{
    Table::Cell::Cell(const std::string &value, Alignment align, Color::FgColor fgColor, Color::BgColor bgColor)
        : value{value}, align{align}, fgColor{fgColor}, bgColor{bgColor}
    {
    }

    Table::Cell::Cell(const std::string &value, Alignment align, Color::FgColor fgColor)
        : Cell(value, align, fgColor, Color::BgColor::Normal)
    {
    }

    Table::Cell::Cell(const std::string &value, Alignment align)
        : Cell(value, align, Color::FgColor::White, Color::BgColor::Normal)
    {
    }

    Table::Cell::Cell(const std::string &value, Color::FgColor fgColor, Color::BgColor bgColor)
        : Cell(value, Alignment::Left, fgColor, bgColor)
    {
    }

    Table::Cell::Cell(const std::string &value, Color::FgColor fgColor)
        : Cell(value, Alignment::Left, fgColor, Color::BgColor::Normal)
    {
    }

    Table::Cell::Cell(const std::string &value)
        : Cell(value, Alignment::Left, Color::FgColor::White, Color::BgColor::Normal)
    {
    }

    void Table::setHeader(const std::vector<std::string> &header)
    {
        this->header.clear();
        for (const auto &cell : header)
        {
            this->header.push_back(Cell(cell));
        }
    }

    void Table::addRow(const std::vector<Cell> &row)
    {
        this->rows.push_back(row);
    }

    void Table::setSummary(const std::vector<Cell> &summary)
    {
        this->summary = summary;
    }

    void Table::print() const
    {
        const size_t maxWidth = 20;
        std::vector<size_t> columnWiths;
        for (const auto &cell : header)
        {
            columnWiths.push_back(std::min(cell.getValue().length(), maxWidth));
        }
        for (const auto &row : rows)
        {
            if (columnWiths.size() > 0)
            {
                columnWiths[0] = std::max(columnWiths[0], row[0].getValue().length());
            }
        }

        // printTopOrBottom(columnWiths);

        std::cout << "|";
        for (size_t i = 0; i < header.size(); ++i)
        {
            std::string formatted = formatString(header[i], columnWiths[i]);
            std::cout << " " << formatted << " |";
        }
        std::cout << std::endl;

        printSeparator(columnWiths);

        for (const auto &row : rows)
        {
            std::cout << "|";
            for (size_t i = 0; i < row.size(); ++i)
            {
                std::string formatted = formatString(row[i], columnWiths[i]);
                std::cout << " " << formatted << " |";
            }
            std::cout << std::endl;
        }

        printSeparator(columnWiths);

        std::cout << "|";
        for (size_t i = 0; i < summary.size(); ++i)
        {
            std::string formatted = formatString(summary[i], columnWiths[i]);
            std::cout << " " << formatted << " |";
        }
        std::cout << std::endl;

        // printTopOrBottom(columnWiths);
    }

    std::string Table::formatString(const Cell &cell, size_t width) const
    {
        const std::string value = cell.getValue();
        const Alignment align = cell.getAlignment();
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
        return Color::color(formatted, cell.getFgColor(), cell.getBgColor());
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

    void Table::printTopOrBottom(const std::vector<size_t> &columnWiths) const
    {
        std::cout << "+";
        for (size_t i = 0; i < columnWiths.size(); ++i)
        {
            std::cout << std::string(columnWiths[i] + 2, '-') << "+";
        }
        std::cout << std::endl;
    }
} // namespace Printer
