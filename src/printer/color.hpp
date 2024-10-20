#pragma once

#include <string>
#include <unordered_map>

namespace Printer
{
    class Color
    {
    public:
        enum class FgColor
        {
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Magenta,
            Cyan,
            White
        };

        enum class BgColor
        {
            Normal,
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Magenta,
            Cyan,
            White
        };

        Color() = default;
        ~Color() = default;

        static std::string color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor = BgColor::Normal);

    private:
        static const std::unordered_map<FgColor, std::string> FG_COLORS;
        static const std::unordered_map<BgColor, std::string> BG_COLORS;
        static const std::string COLOR_CLOSE;
        static const std::string BREAK_LINE;
    };
} // namespace Printer
