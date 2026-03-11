#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <string_view>
#include "entry.h"

#pragma once
enum class SortType
{
    BY_ID,
    BY_DATE
};

class JournalManager
{
private:
    std::vector<JournalEntry> entries;
    std::vector<JournalEntry *> displayView;
    SortType currentSort;

public:
    JournalManager() {};
    void saveData(const std::string &filename);
    void addEntry(const JournalEntry &entry);
    void loadData();
    void searchByDate(std::string_view queryDate) const;
    void deleteEntry(int index);
    void printWithIndex(SortType type);
    std::string trim(const std::string &s);
    bool isValidDate(const std::string &date);
    bool isValidPath(std::string_view pathStr);
    void searchByContent(const std::string &keyword) const;
    void previewCode(int index) const;
    void toLower(std::string &s) const;
    void openEntry(int index) const;
    bool openCheck();
    int getNextID() const;
    bool isSafePath(std::string_view pathStr) const;
    void refreshView(SortType type) const;
    JournalEntry *getEntryByViewIndex(int userIndex);
};