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
            result += '\\'; // Add escape character
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
            mdFile << "**File with code:** `" << entry->getPath() << "\n\n";
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

    mdFile << "End of the journal. Total entries: " << entries.size() << "_\n";

    mdFile.close();

    std::cout << COLOR::GREEN << "✓Exported " << COLOR::RESET << entries.size()
              << COLOR::GREEN << " entries to " << COLOR::RESET << outputFile << "\n";

    return true;
}