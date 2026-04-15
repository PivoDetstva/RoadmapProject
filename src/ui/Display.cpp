#include "Display.h"
void Display::pressEnterToContinue() const
{
    if (std::cin.rdbuf()->in_avail() > 0)
    {
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    std::cout << "\nPress Enter to continue..."; // how dan I do it to only accept enter?
    std::cin.get();
}
std::string Display::escapeForShell(const std::string &path) const
{
    std::string escaped = "'";

    for (char c : path)
    {
        if (c == '\'')
        {

            escaped += "'\\''";
        }
        else
        {
            escaped += c;
        }
    }

    escaped += "'";
    return escaped;
}
void Display::showEntryList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type)
{

    if (entries.empty())
    {
        std::cerr << COLOR::RED << "✗ Error: Entry file is empty." << COLOR::RESET << "\n";
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
                std::cout << COLOR::RED << "✗File can be malicious, returning to menu..." << COLOR::RESET << "\n";
                return;
            }
        }
        else if (choice == 'n' || choice == 'N')
        {
            std::cout << "Going back to menu...\n";
            pressEnterToContinue();
        }
    }
    pressEnterToContinue();
}
void Display::openCode(const std::vector<JournalEntry>::const_iterator &iterator) const
{
    std::string path = iterator->getPath();

    if (path.empty())
    {
        std::cerr << COLOR::RED << "✗User haven't provide any file" << COLOR::RESET << "\n";
        return;
    }
    if (!std::filesystem::exists(path))
    {
        std::cerr << COLOR::RED << "✗Error: FIle wasn't found on " << path << COLOR::RESET << "\n";
        return;
    }

    std::ifstream codefile(path);
    if (!codefile.is_open())
    {
        std::cerr << COLOR::RED << "✗Error opening the file codefile" << COLOR::RESET << "\n";
        return;
    }
    std::cout << "\n--- START OF CODE (" << path << ") ---\n";
    std::string line;
    while (std::getline(codefile, line))
    {
        std::cout << line << std::endl;
    }
    std::cout << "\n--- END OF CODE ---\n";
}
void Display::showCodeList(std::vector<JournalEntry> &entries, std::vector<JournalEntry *> &displayView, SortType type)
{

    if (entries.empty())
    {
        std::cerr << COLOR::RED << "✗Error: Entry file is empty." << COLOR::RESET << "\n";
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
        std::cerr << COLOR::RED << "✗No entries with code found!" << COLOR::RESET << "\n";
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
Also would be really cool for a search refactoring, now I would like to make it in a cycle,
but would be better to make a templated search. and refactor all the method.
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