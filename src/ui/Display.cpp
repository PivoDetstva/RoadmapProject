#include "Display.h"
void Display::pressEnterToContinue() const
{
    if (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
std::string Display::escapeForShell(const std::string &path) const
{
    std::string escaped = "'"; // Start with opening quote

    for (char c : path)
    {
        if (c == '\'')
        { // If we find a single quote in path
            // Close quote, add escaped quote, reopen quote
            escaped += "'\\''";
        }
        else
        {
            escaped += c;
        }
    }

    escaped += "'"; // Closing quote
    return escaped;
}
void Display::showEntryList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type)
{

    if (entries.empty())
    {
        std::cerr << "Error: Entry file is empty.\n";
        return;
    }
    displayView.clear();
    for (auto &entry : entries)
    {
        displayView.push_back(&entry);
    }
    long codecount = std::count_if(entries.begin(), entries.end(), [](const JournalEntry &e)
                                   { return e.getPath() != CONSTS::NO_CODE_PATH; });
    if (type == SortType::BY_DATE)
    {
        std::sort(displayView.begin(), displayView.end(), [](const JournalEntry *a, const JournalEntry *b)
                  { return a->getDate() < b->getDate(); });
    }
    else if (type == SortType::BY_ID)
    {
        std::sort(displayView.begin(), displayView.end(), [](const JournalEntry *a, const JournalEntry *b)
                  { return a->getID() < b->getID(); });
    }
    std::cout << "Found " << displayView.size() << " entries!\n"
              << codecount << " of them contains code!\n";

    for (size_t i = 0; i < displayView.size(); ++i)
    {
        std::cout << "[" << i + 1 << "] " << displayView[i]->getDate()
                  << " | " << displayView[i]->getTitle();
        if (displayView[i]->getPath() != CONSTS::NO_CODE_PATH)
        {
            std::cout << " [Contains code]\n";
        }
        std::cout << "\n";
    }
}
void Display::openGuts(const std::vector<JournalEntry>::const_iterator &iterator) const
{
    const JournalEntry &entry = *iterator;
    std::cout << "\t\t" << entry.getTitle()
              << "\n\n"
              << entry.getText()
              << "\n"
              << "Created on " << entry.getDate();
    const auto &path = entry.getPath();
    if (path.length() > 1 && path != CONSTS::NO_CODE_PATH)
    {

        char choice = inputHandler.getChar("\nAttached code found. Open file? (y/n): ");
        if (choice == 'y' || choice == 'Y')
        {
            if (validator.isSafePath(entry.getPath()))
            {
                std::string command = "xdg-open '" + escapeForShell(entry.getPath());
                std::system(command.c_str());
            }
            else
            {
                std::cout << "File can be malicious, returning to menu...\n";
                return;
            }
        }
        else if (choice == 'n' || choice == 'N')
        {
            pressEnterToContinue();
            std::cout << "Going back to menu...\n";
        }
    }
    pressEnterToContinue();
}
void Display::openCode(const std::vector<JournalEntry>::const_iterator &iterator) const
{
    std::string path = iterator->getPath();

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
    std::string line;
    while (std::getline(codefile, line))
    {
        std::cout << line << std::endl;
    }
    std::cout << "\n--- END OF CODE ---\n";
    pressEnterToContinue();
}
void Display::showCodeList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type)
{

    if (entries.empty())
    {
        std::cerr << "Error: Entry file is empty.\n";
        return;
    }
    displayView.clear();
    for (auto &entry : entries)
    {
        if (entry.getPath() != CONSTS::NO_CODE_PATH)
        {

            displayView.push_back(&entry);
        }
    }
    if (displayView.empty())
    {
        std::cout << "No entries with code found!\n";
        return;
    }
    long codecount = std::count_if(entries.begin(), entries.end(), [](const JournalEntry &e)
                                   { return e.getPath() != CONSTS::NO_CODE_PATH; });
    if (type == SortType::BY_DATE)
    {
        std::sort(displayView.begin(), displayView.end(), [](const JournalEntry *a, const JournalEntry *b)
                  { return a->getDate() < b->getDate(); });
    }
    else if (type == SortType::BY_ID)
    {
        std::sort(displayView.begin(), displayView.end(), [](const JournalEntry *a, const JournalEntry *b)
                  { return a->getID() < b->getID(); });
    }
    std::cout << "Found " << codecount << " entries!\n";

    for (size_t i = 0; i < displayView.size(); ++i)
    {
        std::cout << "[" << i + 1 << "] " << displayView[i]->getDate()
                  << " | " << displayView[i]->getTitle() << "\n";
    }
}

/*Got an idea how to make template function
display.showFiltered(entries,
    [](const auto& e) { return e.getPath() != CONSTS::NO_CODE_PATH; },
    SortType::BY_DATE);

// Show only entries from 2024:
display.showFiltered(entries,
    [](const auto& e) { return e.getDate().find("2024") == 0; },
    SortType::BY_DATE);

// Show only entries with "C++" in title:
display.showFiltered(entries,
    [](const auto& e) { return e.getTitle().find("C++") != string::npos; },
    SortType::BY_ID);*/