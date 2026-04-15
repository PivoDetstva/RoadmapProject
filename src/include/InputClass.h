#include <string>
#include <iostream>
#include <limits>
#include "Constants.h"

#pragma once

enum class SortType
{
    BY_ID,
    BY_DATE
};

class InputHandle
{
private:
public:
    void toLower(std::string &s) const;
    int getInt(std::string prompt);
    SortType getSortType();
    std::string getString(std::string prompt);
    char getChar(std::string prompt) const;
};
