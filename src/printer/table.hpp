#pragma once

#include <string>
#include <vector>
#include "printer/color.hpp"

namespace Printer
{
    enum class Alignment
    {
        Left,
        Right,
        Center
    };

    class Table
    {
        class Cell
        {
        public:
            Cell(const std::string &value, Alignment align, Color::FgColor fgColor, Color::BgColor bgColor);
            Cell(const std::string &value, Alignment align, Color::FgColor fgColor);
            Cell(const std::string &value, Alignment align);
            Cell(const std::string &value, Color::FgColor fgColor, Color::BgColor bgColor);
            Cell(const std::string &value, Color::FgColor fgColor);
            Cell(const std::string &value);
            ~Cell() = default;

            inline const std::string &getValue() const
            {
                return value;
            }
            inline Alignment getAlignment() const
            {
                return align;
            }
            inline Color::FgColor getFgColor() const
            {
                return fgColor;
            }
            inline Color::BgColor getBgColor() const
            {
                return bgColor;
            }

        private:
            std::string value;
            Alignment align;
            Color::FgColor fgColor;
            Color::BgColor bgColor;
        };

    public:
        Table() = default;
        ~Table() = default;

        void setHeader(const std::vector<std::string> &header);
        void addRow(const std::vector<Cell> &row);
        void setSummary(const std::vector<Cell> &summary);
        void print() const;

    private:
        std::string formatString(const Cell &cell, size_t width) const;
        void printSeparator(const std::vector<size_t> &columnWiths) const;
        void printTopOrBottom(const std::vector<size_t> &columnWiths) const;

    private:
        std::vector<Cell> header;
        std::vector<std::vector<Cell>> rows;
        std::vector<Cell> summary;
    };
} // namespace Printer
