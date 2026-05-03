#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "entry.hpp"
#include "InputClass.hpp"

class Finder
{
private:
    InputHandle inputHandler;

public:
    void searchContent(const std::string &keyword, std::vector<JournalEntry> &entries);
    void searchDate(std::string_view queryDate, std::vector<JournalEntry> &entries) const;
};