#include <string>
#pragma once
class Encryptor
{
public:
    static void applyXOR(std::string &data, const char key = 'K');
    static std::string toHex(const std::string &data);
    static std::string fromHex(const std::string &data);
};