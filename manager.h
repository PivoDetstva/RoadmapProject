#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include <cctype>
#include "entry.h"

#pragma once
class JournalManager
{
private:
    std::vector<JournalEntry> entries;

public:
    JournalManager() {};
    void saveToFile(string filename);
    void addEntry(JournalEntry entry);
    void loadFromFile(string filename); // change later to boolean
    void printAll() const;
    void searchByDate(string queryDate) const;
    void deleteEntry(int index);
    void printWIthIndex() const;
    string trim(const string &s);
    bool isValidDate(const string &date);
    bool isValidPath(string pathStr);
    void searchByContent(string keyword) const;
    void previewCode(int index) const;
    void toLower(std::string &s);
};