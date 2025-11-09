#include "./magicWrapper.hpp"
#include <iostream>
#include <filesystem>
#include <string>
#include <vector>

#ifdef _WIN32
#include <array>
#include <windows.h>
#endif

namespace
{
#ifdef _WIN32
    std::string locateMagicDatabase()
    {
        std::array<char, 4096> modulePath{};
        DWORD length = GetModuleFileNameA(nullptr, modulePath.data(), static_cast<DWORD>(modulePath.size()));
        if (length == 0 || length >= modulePath.size())
        {
            return {};
        }

        const std::filesystem::path exePath(modulePath.data(), modulePath.data() + length);
        const std::filesystem::path exeDir = exePath.parent_path();

        std::vector<std::filesystem::path> candidates = {
            exeDir / "magic.mgc",
            exeDir.parent_path() / "magic.mgc"};

        for (const auto &candidate : candidates)
        {
            std::error_code ec;
            if (std::filesystem::exists(candidate, ec))
            {
                const auto resolved = std::filesystem::weakly_canonical(candidate, ec);
                return ec ? candidate.string() : resolved.string();
            }
        }

        return {};
    }
#endif
} // namespace

MagicWrapper::MagicWrapper()
{
    magicCookie = magic_open(MAGIC_MIME_TYPE);
    if (magicCookie == nullptr)
    {
        std::cerr << "Unable to initialize magic library" << std::endl;
        throw std::runtime_error("Unable to initialize magic library");
    }

    if (magic_load(magicCookie, nullptr) != 0)
    {
#ifdef _WIN32
        const std::string dbPath = locateMagicDatabase();
        if (!dbPath.empty() && magic_load(magicCookie, dbPath.c_str()) == 0)
        {
            return;
        }
#endif

        std::cerr << "Cannot load magic database: " << magic_error(magicCookie) << std::endl;
        magic_close(magicCookie);
        throw std::runtime_error("Cannot load magic database");
    }
}

MagicWrapper::~MagicWrapper()
{
    if (magicCookie)
    {
        magic_close(magicCookie);
        magicCookie = nullptr;
    }
}

magic_t MagicWrapper::getMagicCookie() const
{
    return magicCookie;
}
