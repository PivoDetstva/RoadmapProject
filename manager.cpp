#include "manager.h"
#include "encryptor.h"
void JournalManager::addEntry(const JournalEntry &entry)
{
    entries.push_back(entry);
}

void JournalManager::saveToFile(string_view filename)
{
    std::string fname(filename);
    std::string tempFilename(fname);
    tempFilename = fname + ".tmp";
    std::ofstream file(tempFilename);
    if (!file.is_open())
    {
        std::cerr << "Error: save file wasn't opened \n";
        return;
    }
    for (const auto &entry : entries)
    {
        string encrypted = entry.serialize();
        Encryptor::applyXOR(encrypted); // calls too few arguments error, so I needed to add 'K' int class method
        string safedata = Encryptor::toHex(encrypted);
        file << safedata << std::endl;
    }
    file.close();
    std::error_code ec;
    std::filesystem::rename(tempFilename, filename, ec);
    if (ec)
    {
        std::cerr << "Error renaming file " << ec.message() << std::endl;
        return;
    }
}
void JournalManager::loadFromFile(string_view filename)
{
    std::string fname(filename);
    std::ifstream loadfile(fname);
    std::cout << "loading\n";
    if (!loadfile.is_open())
    {
        std::cerr << "Error: load file wasn't opened\n";
        return;
    }
    string line;
    while (std::getline(loadfile, line))
    {
        if (line.empty())
            continue;

        string decoded = Encryptor::fromHex(line);
        Encryptor::applyXOR(decoded);
        JournalEntry entry;
        entry.deserialize(decoded);
        entries.push_back(entry);
    }
    loadfile.close();
}
void JournalManager::printWithIndex() const
{

    if (entries.empty())
    {
        std::cerr << "Error: Entry file is empty.\n";
        return;
    }
    std::vector<JournalEntry> sortedEntries = entries;
    long codecount = std::count_if(entries.begin(), entries.end(), [](const JournalEntry &e)
                                   { return e.getPath() != "none"; });

    std::sort(sortedEntries.begin(), sortedEntries.end(), [](const JournalEntry &a, const JournalEntry &b)
              { return a.getDate() < b.getDate(); });

    std::cout << "Found " << sortedEntries.size() << " entries!\n"
              << codecount << "of them contains code!";

    for (size_t i = 0; i < sortedEntries.size(); ++i)
    {
        std::cout << "\n[" << i + 1 << "] " << sortedEntries[i].getDate() << " | "
                  << sortedEntries[i].getTitle();
        if (sortedEntries[i].getPath() != "none")
        {
            std::cout << " [Contains code]";
        }
    }
}
void JournalManager::searchByDate(string_view queryDate) const
{
    bool found = false;
    for (const auto &entry : entries)
    {
        if (entry.getDate().find(queryDate) != std::string::npos)
        {
            std::cout << entry.getDate() << " | " << entry.getTitle() << endl;
            found = true;
            std::cout << "Entry found\n";
        }
    }
    if (!found)
        std::cout << "Not found\n";
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
    std::istringstream ss{date};
    std::chrono::year_month_day ymd;

    if (!(ss >> std::chrono::parse("%Y-%m-%d", ymd)))
    {
        return false;
    }
    return ymd.ok();
}
bool JournalManager::isValidPath(string pathStr)
{
    return std::filesystem::exists(pathStr);
}
void JournalManager::searchByContent(string keyword) const
{
    bool found = false;

    string lowerKeyword = keyword;
    toLower(lowerKeyword);

    for (const auto &entry : entries)
    {
        string title = entry.getTitle();
        string text = entry.getText();
        auto it = std::search(
            title.begin(), title.end(),
            lowerKeyword.begin(), lowerKeyword.end(),
            [](unsigned char ch1, unsigned char ch2)
            {
                return std::tolower(ch1) == ch2;
            });
        auto it2 = std::search(text.begin(), text.end(),
                               lowerKeyword.begin(), lowerKeyword.end(),
                               [](unsigned char ch1, unsigned char ch2)
                               {
                                   return std::tolower(ch1) == ch2;
                               });
        if (it != title.end() || it2 != text.end())
        {
            std::cout << "Found: " << entry.getDate() << " | " << entry.getTitle() << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "Nothing found.\n";
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
        std::cerr << "Error: FIle wasn't found on " << path << "\n";
        return;
    }

    std::ifstream codefile(path);
    if (!codefile.is_open())
    {
        std::cerr << "Error opening the file codefile\n";
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
void JournalManager::toLower(std::string &s) const
{
    for (char &c : s)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }
}
void Encryptor::applyXOR(std::string &data, const char key)
{
    for (char &c : data)
    {
        c ^= key;
    }
}
void JournalManager::openEntry(int index) const
{
    if (index < 1 || index > entries.size())
    {
        std::cout << "Wrong index!\n";
        return;
    }
    const auto &entry = entries.at(index - 1);
    std::cout << "\t\t" << entry.getTitle()
              << "\n\n"
              << entry.getText()
              << "\n"
              << "Created on " << entry.getDate();
    const auto &path = entry.getPath();
    if (path.length() > 1 && path != "none")
    {
        char choice;
        std::cout << "\nAttached code found. Open file? (y/n): ";
        std::cin >> choice;
        if (choice == 'y' || choice == 'Y')
        {
            std::string command = "xdg-open " + entry.getPath();
            std::system(command.c_str());
        }
    }
}
bool JournalManager::openCheck()
{
    if (entries.empty())
    {
        std::cout << "Entry list are empty!\n";
        return true;
    }
    else
        return false;
}
std::string Encryptor::toHex(const std::string &input)
{
    std::ostringstream oss;
    for (unsigned char c : input)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}
std::string Encryptor::fromHex(const std::string &input)
{
    std::string output;
    if (input.length() % 2 != 0)
        return "";
    for (size_t i = 0; i < input.length(); i += 2)
    {
        std::string byteString = input.substr(i, 2);
        char byte = (char)strtol(byteString.c_str(), nullptr, 16);
        output += byte;
    }
    return output;
}