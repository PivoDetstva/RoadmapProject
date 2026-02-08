#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <filesystem>
#include <optional>
#include <algorithm>
#include <chrono>
#include <string_view>
#include "entry.h"

#pragma once
class JournalManager
{
private:
    std::vector<JournalEntry> entries;

public:
    JournalManager() {};
    void saveToFile(string_view filename);
    void addEntry(const JournalEntry &entry);
    void loadFromFile(string_view filename); // change later to boolean
    void printAll() const;
    void searchByDate(string_view queryDate) const;
    void deleteEntry(int index);
    void printWithIndex() const;
    string trim(const string &s);
    bool isValidDate(const string &date);
    bool isValidPath(string pathStr);
    void searchByContent(string keyword) const;
    void previewCode(int index) const;
    void toLower(std::string &s) const;
    void openEntry(int index) const;
    bool openCheck();
    std::optional<JournalEntry> searchByID(int id);
    /*Should look like this:
    for (const auto& entry : entries) {
        if (entry.getId() == id) {
            return entry;
        }
    }
    return std::nullopt;
}
    It is good new option for a search, also editEntry should be added next by.
    This option needs a logic refactoring, cause I don't have ID now, and main implementing should be like that:
    auto result = manager.findEntryById(5);
    if (result.has_value()) {
    std::cout << result->getTitle();
    }
    else {
    std::cout << "Not found";
}

*/
};