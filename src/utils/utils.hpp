#pragma once

#include <string>
#include <string_view>
#include <chrono>

namespace Utils
{
    std::string_view trimView(std::string_view view);

    std::string formatDuration(std::chrono::milliseconds duration);
} // namespace Utils
