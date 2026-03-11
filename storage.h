#include <string>
#include <vector>
#include <filesystem>
#include "entry.h"
#include "Constants.h"
#pragma once
class Storage
{
public:
    static void applyXOR(std::string &data, const char key = CONSTS::DEFAULT_XOR_KEY);
    static std::string toHex(const std::string &data);
    static std::string fromHex(const std::string &data);
    void saveToFile(const std::vector<JournalEntry> &dataToSave, const std::string &filename);
    std::vector<JournalEntry> loadFromFile(const std::string &filename);
};