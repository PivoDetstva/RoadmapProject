#include <string>
#include "Constants.h"
#pragma once
class Encryptor
{
public:
    static void applyXOR(std::string &data, const char key = CONSTS::DEFAULT_XOR_KEY);
    static std::string toHex(const std::string &data);
    static std::string fromHex(const std::string &data);
};