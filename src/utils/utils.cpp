#include "./utils.hpp"
#include <sstream>
#include <iomanip>

namespace Utils
{
    std::string formatDuration(std::chrono::milliseconds duration)
    {
        using namespace std::chrono;

        int64_t totalMilliseconds = duration.count();
        if (totalMilliseconds < 1)
        {
            return "less than 1ms";
        }

        int64_t milliseconds = totalMilliseconds % 1000;
        int64_t totalSeconds = totalMilliseconds / 1000;
        int64_t seconds = totalSeconds % 60;
        int64_t totalMinutes = totalSeconds / 60;
        int64_t minutes = totalMinutes % 60;
        int64_t hours = totalMinutes / 60;

        std::ostringstream oss;
        if (hours > 0)
        {
            oss << hours << "h ";
        }
        if (minutes > 0)
        {
            oss << minutes << "min ";
        }
        if (seconds > 0)
        {
            oss << seconds << "s ";
        }
        if (milliseconds > 0 || totalMilliseconds < 1000)
        {
            oss << milliseconds << "ms";
        }

        std::string result = oss.str();
        if (!result.empty() && result.back() == ' ')
        {
            result.pop_back();
        }
        return result;
    }
} // namespace Utils
