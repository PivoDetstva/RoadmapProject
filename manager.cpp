#include "manager.h"

void JournalManager::addEntry(JournalEntry entry)
{
    entries.push_back(entry);
}

void JournalManager::saveToFile(string filename)
{
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open())
    {
        std::cout << "error opening the file save \n";
        return;
    }
    std::cout << "saving\n";
    for (const auto &entry : entries)
    {
        file << entry.serialize() << std::endl;
    }
    file.close();
    std::cout << "saving complete\n";
}
void JournalManager::loadFromFile(string filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cout << "error opening the file load\n";
        return;
    }
    string line;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        JournalEntry entry;
        entry.deserialize(line);
        entries.push_back(entry);
    }
    file.close();
    std::cout << "LOading complete\n";
}
