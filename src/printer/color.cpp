#include "./color.hpp"
#include <sstream>

namespace Printer
{
    const std::unordered_map<Color::FgColor, std::string> Color::FG_COLORS{
        {FgColor::Black, "\033[30m"},
        {FgColor::Red, "\033[31m"},
        {FgColor::Green, "\033[32m"},
        {FgColor::Yellow, "\033[33m"},
        {FgColor::Blue, "\033[34m"},
        {FgColor::Magenta, "\033[35m"},
        {FgColor::Cyan, "\033[36m"},
        {FgColor::White, "\033[37m"},
    };

    const std::unordered_map<Color::BgColor, std::string> Color::BG_COLORS{
        {BgColor::Normal, ""},
        {BgColor::Black, "\033[40m"},
        {BgColor::Red, "\033[41m"},
        {BgColor::Green, "\033[42m"},
        {BgColor::Yellow, "\033[43m"},
        {BgColor::Blue, "\033[44m"},
        {BgColor::Magenta, "\033[45m"},
        {BgColor::Cyan, "\033[46m"},
        {BgColor::White, "\033[47m"},
    };

    const std::string Color::COLOR_CLOSE{"\033[0m"};
    const std::string Color::BREAK_LINE{"\n"};

    std::string Color::color(const std::string &str, const FgColor &fgColor, const BgColor &bgColor)
    {
        std::ostringstream result;
        result << FG_COLORS.at(fgColor) << BG_COLORS.at(bgColor) << str << COLOR_CLOSE;
        return result.str();
    }
} // namespace Printer
