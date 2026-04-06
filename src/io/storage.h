#include <string>
#include <vector>
#include <filesystem>
#include <chrono>
#include "entry.h"
#include "Constants.h"
#pragma once
class Storage
{
private:
    static void applyXOR(std::string &data, const char key = CONSTS::DEFAULT_XOR_KEY);
    static std::string toHex(const std::string &data);
    static std::string fromHex(const std::string &data);

public:
    void saveToFile(const std::vector<JournalEntry> &dataToSave, const std::string &filename);
    std::vector<JournalEntry> loadFromFile(const std::string &filename);
    bool exportToMarkdown(const std::vector<JournalEntry> &entries, const std::string &outputFile);
    bool importFromMarkdown(std::vector<JournalEntry> &entries, const std::string &inputFIle);
    int getNextIDFromEntries(const std::vector<JournalEntry> &entries);
    std::string escapeMarkdown(const std::string &text);
};