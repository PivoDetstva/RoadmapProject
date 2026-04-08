#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <map>
#include <string_view>
#include "validation.h"
#include "entry.h"
#include "storage.h"
#include "InputClass.h"
#include "Display.h"
#include "SearchEngine.h"
#pragma once

class JournalManager
{
private:
    std::vector<JournalEntry> entries;
    std::vector<JournalEntry *> displayView;
    std::vector<JournalEntry *> codeView;
    InputHandle inputHandler;
    Display display;
    Validator validator;
    Finder fifind;
    Storage storage;

public:
    JournalManager() {};
    void saveData(const std::string &filename);
    void addEntry(const JournalEntry &entry);
    void loadData();
    void searchByDate(std::string_view queryDate);
    void deleteEntry(int index);
    void printAll(SortType type);
    void searchByContent(const std::string &keyword);
    void previewCode(int index) const;
    void openEntry(int index) const;
    bool openCheck();
    int getNextID() const;
    JournalEntry *getEntryByViewIndex(int userIndex);
    JournalEntry *getEntryByCodeViewIndex(int userIndex);
    void printWithCode(SortType type);
    bool codeCheck();
    void editEntry(int index);
    void showStatistics() const;
    void exportMarkdown(const std::string &filename);
    void importMarkdown(const std::string &filename);
};