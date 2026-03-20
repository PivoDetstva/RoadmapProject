#include <vector>
#include <algorithm>
#include "entry.h"
#include "InputClass.h"
#include "validation.h"
#include "Constants.h"
#pragma once

class Display
{
private:
    InputHandle inputHandler;
    Validator validator;

public:
    void showEntryList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type);
    void openGuts(const std::vector<JournalEntry>::const_iterator &iterator) const;
    void pressEnterToContinue() const;
    void openCode(const std::vector<JournalEntry>::const_iterator &iterator) const;
    void showCodeList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type);
    std::string escapeForShell(const std::string &path) const;
};