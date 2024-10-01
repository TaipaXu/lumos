#include "./magicWrapper.hpp"
#include <iostream>

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
