#include "manager.h"

void JournalManager::addEntry(JournalEntry entry)
{
    entries.push_back(entry);
}

void JournalManager::saveToFile(string filename)
{
    std::ofstream file(filename);
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
    std::cout << "loading\n";
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
void JournalManager::printWIthIndex() const
{
    if (entries.empty())
    {
        std::cout << "Empty.\n";
        return;
    }
    std::cout << "Index print";
    for (size_t i = 0; i < entries.size(); ++i)
    {
        std::cout << "\n[" << i + 1 << "]" << entries[i].serialize() << std::endl;
    }
}
void JournalManager::printAll() const
{
    if (entries.empty())
    {
        std::cout << "There was no entries yet!\n";
        return;
    }
    std::cout << "printing\n";
    for (const auto &entry : entries)
    {

        std::cout << entry.serialize() << std::endl;
    }
}
void JournalManager::searchByDate(string queryDate) const
{
    bool found = false;
    for (const auto &entry : entries)
    {
        if (entry.getDate() == queryDate)
        {
            std::cout << entry.serialize() << endl;
            found = true;
            std::cout << "Entry found\n";
        }
    }
    if (!found)
        cout << "Not found\n";
}
void JournalManager::deleteEntry(int index)
{
    if (index < 1 || index > entries.size())
    {
        std::cout << "Invalid index!\n";
        return;
    }

    entries.erase(entries.begin() + index - 1);
    std::cout << "Deleted entry #" << index << endl;
}
string JournalManager::trim(const string &s)
{
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}
bool JournalManager::isValidDate(const string &date)
{
    if (date.length() != 10)
        return false;
    if (date[4] == '-' && date[7] == '-')
        return true;
    return false;
}
bool JournalManager::isValidPath(string pathStr)
{
    return std::filesystem::exists(pathStr);
}
void JournalManager::showFromFile(int index) const
{
}
void JournalManager::previewCode(int index) const
{
    if (index < 1 || index > entries.size())
    {
        std::cout << "Invalid index!\n";
        return;
    }

    string path = entries[index - 1].getPath();
    if (path.empty())
    {
        std::cout << "User haven't provide any file" << std::endl;
        return;
    }
    if (!std::filesystem::exists(path))
    {
        std::cout << "Error: FIle wasn't found on " << path << "\n";
        return;
    }

    std::ifstream codefile(path);
    if (!codefile.is_open())
    {
        std::cout << "Error opening the file codefile\n";
        return;
    }
    std::cout << "\n--- START OF CODE (" << path << ") ---\n";
    string line;
    while (std::getline(codefile, line))
    {
        std::cout << line << std::endl;
    }
    std::cout << "\n--- END OF CODE ---\n";
}