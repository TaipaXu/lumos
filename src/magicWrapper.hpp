#pragma once

#include <magic.h>

class MagicWrapper
{
public:
    MagicWrapper();

    ~MagicWrapper();

    magic_t getMagicCookie() const;

private:
    magic_t magicCookie;
};
