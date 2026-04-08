#include <string>
#include <iostream>
#include <string_view>
#include <chrono>
#include <sstream>
#include <filesystem>
#pragma once
class Validator
{
private:
public:
    bool isValidDate(const std::string &date);
    bool isValidPath(std::string_view pathStr);
    std::string trim(const std::string &s);
    bool isSafePath(std::string_view pathStr) const;
    // escapeforshell
};