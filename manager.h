#include <iostream>
#include <string>
#include <fstream>
#include <vector>
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
};