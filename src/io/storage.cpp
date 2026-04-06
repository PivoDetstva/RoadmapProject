#include "storage.h"

void Storage::saveToFile(const std::vector<JournalEntry> &dataToSave, const std::string &filename)
{
    std::string tempFilename(filename);
    tempFilename = filename + ".tmp";
    std::ofstream file(tempFilename);
    if (!file.is_open())
    {
        std::cerr << "Error: save file wasn't opened \n";
        return;
    }
    for (const auto &entry : dataToSave)
    {
        std::string encrypted = entry.serialize();
        Storage::applyXOR(encrypted);
        std::string safedata = Storage::toHex(encrypted);
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
std::vector<JournalEntry> Storage::loadFromFile(const std::string &filename)
{
    std::ifstream loadfile(filename);
    std::vector<JournalEntry> loadedEntries;
    std::cout << "loading\n";
    if (!loadfile.is_open())
    {
        std::cerr << "Error: load file wasn't opened\n";
        return loadedEntries;
    }
    std::string line;
    while (std::getline(loadfile, line))
    {
        if (line.empty())
            continue;

        std::string decoded = Storage::fromHex(line);
        Storage::applyXOR(decoded);
        JournalEntry entry;
        entry.deserialize(decoded);
        loadedEntries.push_back(entry);
    }
    loadfile.close();

    return loadedEntries;
}
void Storage::applyXOR(std::string &data, const char key)
{
    for (char &c : data)
    {
        c ^= key;
    }
}
std::string Storage::toHex(const std::string &input)
{
    std::ostringstream oss;
    for (unsigned char c : input)
    {
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return oss.str();
}
std::string Storage::fromHex(const std::string &input)
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
std::string Storage::escapeMarkdown(const std::string &text)
{
    std::string result;
    result.reserve(text.size() * 1.2);
    for (char c : text)
    {
        switch (c)
        {
        case '*':
        case '_':
        case '#':
        case '[':
        case ']':
        case '(':
        case ')':
        case '`':
        case '\\':
            result += '\\';
            break;
        }
        result += c;
    }
    return result;
}
bool Storage::exportToMarkdown(const std::vector<JournalEntry> &entries,
                               const std::string &outputFile)
{
    std::ofstream mdFile(outputFile);
    if (!mdFile.is_open())
    {
        std::cerr << "Error to open export file\n";
        return false;
    }

    auto getCurrentDate = []()
    {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        std::ostringstream oss;
        oss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
        return oss.str();
    };

    mdFile << "# My Programming Diary\n\n";
    mdFile << "Exported on: " << getCurrentDate() << "_\n\n";
    mdFile << "---\n\n";

    std::vector<const JournalEntry *> sortedEntries;
    for (const auto &entry : entries)
    {
        sortedEntries.push_back(&entry);
    }
    std::sort(sortedEntries.begin(), sortedEntries.end(), [](const JournalEntry *a, const JournalEntry *b)
              { return a->getDate() < b->getDate(); });

    for (const auto &entry : sortedEntries)
    {
        mdFile << "## " << escapeMarkdown(entry->getTitle()) << "\n\n";

        mdFile << "**Date** " << entry->getDate() << " \n";
        mdFile << "**ID** " << entry->getID() << " \n";
        if (entry->getPath() != CONSTS::NO_CODE_PATH)
        {
            mdFile << "**File with code:** `" << entry->getPath() << "`  \n";
            /*Ai recommendation, test later
                 mdFile << "**Attached Code:**\n\n";

        std::ifstream codeFile(entry->getPath());
        if (codeFile.is_open())
        {
            // Determine language from extension
            std::string ext = std::filesystem::path(entry->getPath())
                              .extension().string();
            std::string lang = (ext == ".cpp" || ext == ".h") ? "cpp" : "text";

            mdFile << "```" << lang << "\n";

            std::string line;
            while (std::getline(codeFile, line))
            {
                mdFile << line << "\n";
            }

            mdFile << "```\n\n";
        }
        else
        {
            mdFile << "_Code file not found: " << entry->getPath() << "_\n\n";
        }
    }*/
        }
        mdFile << "\n";

        mdFile << escapeMarkdown(entry->getText()) << "\n\n";

        mdFile << "---\n\n";
    }

    mdFile << "End of the journal. Total entries: " << entries.size() << "\n";

    mdFile.close();

    std::cout << COLOR::GREEN << "\n✓Exported " << COLOR::RESET << entries.size()
              << COLOR::GREEN << " entries to " << COLOR::RESET << outputFile << "\n";

    return true;
}
int Storage::getNextIDFromEntries(const std::vector<JournalEntry> &entries)
{
    if (entries.empty())
        return 1;

    int maxID = 0;
    for (const auto &e : entries)
    {
        if (e.getID() > maxID)
            maxID = e.getID();
    }
    return maxID + 1;
}
bool Storage::importFromMarkdown(std::vector<JournalEntry> &entries, const std::string &inputFile)
{
    std::ifstream mdFile(inputFile);
    if (!mdFile.is_open())
    {
        std::cerr << COLOR::RED << "✗Error opening the file" << COLOR::RESET << "\n";
        return false;
    }
    std::string line;
    std::string currentTitle, currentDate, currentText, currentPath;
    bool inEntry = false;

    while (std::getline(mdFile, line))
    {
        if (line.find("#My Programming Journal") == 0 || line.find("---") == 0 || line.empty())
        {
            continue;
        }
        if (line.find("## ") == 0)
        {
            if (inEntry && !currentTitle.empty())
            {
                int id = getNextIDFromEntries(entries);
                entries.emplace_back(id, currentDate, currentTitle, currentText, currentPath);
            }
            currentTitle = line.substr(3);
            currentText.clear();
            currentPath = CONSTS::NO_CODE_PATH;
            inEntry = true;
            continue;
        }
        if (line.find("**Date:**") == 0)
        {
            currentDate = line.substr(10);
            continue;
        }
        if (line.find("**FIle with code:**"))
        {
            auto start = line.find('`') + 1;
            auto end = line.rfind('`');
            if (start != std::string::npos && end != std::string::npos)
            {
                currentPath = line.substr(start, end - start);
            }
            continue;
        }
        if (inEntry)
        {
            if (!currentText.empty())
            {
                currentText += "\n";
            }
            currentText += line;
        }
    }
    if (inEntry && !currentTitle.empty())
    {
        int id = getNextIDFromEntries(entries);
        entries.emplace_back(id, currentDate, currentTitle, currentText, currentPath);
    }
    mdFile.close();
    std::cout << COLOR::GREEN << "✓ Imported" << entries.size() << "entries" << COLOR::RESET << "\n";
    return true;
}