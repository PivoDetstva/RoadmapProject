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