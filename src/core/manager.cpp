#include "manager.h"
#include "storage.h"
#include "Constants.h"
void JournalManager::addEntry(const JournalEntry &entry)
{
    entries.push_back(entry);
}

void JournalManager::saveData(const std::string &filename)
{
    storage.saveToFile(entries, CONSTS::filename);
}
void JournalManager::loadData()
{
    this->entries = storage.loadFromFile(CONSTS::filename);
}
void JournalManager::printAll(SortType type)
{
    display.showEntryList(entries, displayView, type);
}

void JournalManager::searchByDate(std::string_view queryDate)
{
    fifind.searchDate(queryDate, entries);
    display.pressEnterToContinue();
}
void JournalManager::deleteEntry(int index)
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << COLOR::RED << "✗Error: Entry not found!" << COLOR::RESET << "\n";
        return;
    }

    entries.erase(it);
    std::cout << COLOR::GREEN << "✓ Successfully deleted entry #" << COLOR::RESET << index << std::endl;
    display.pressEnterToContinue();
}
void JournalManager::searchByContent(const std::string &keyword)
{
    fifind.searchContent(keyword, entries);
    display.pressEnterToContinue();
}
void JournalManager::previewCode(int index) const
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << COLOR::RED << "✗Error: Entry not found!" << COLOR::RESET << "\n";
        return;
    }
    display.openCode(it);
}

void JournalManager::openEntry(int index) const
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << COLOR::RED << "✗Error: Entry not found!" << COLOR::RESET << "\n";
        return;
    }
    display.openGuts(it);
}
bool JournalManager::openCheck()
{
    if (entries.empty())
    {
        std::cerr << COLOR::RED << "\n✗Entry list are empty!" << COLOR::RESET << "\n";
        display.pressEnterToContinue();
        return true;
    }
    else
        return false;
}
int JournalManager::getNextID() const
{
    if (entries.empty())
    {
        return 1;
    }

    int maxID = 0;
    for (const auto &entry : entries)
    {
        if (entry.getID() > maxID)
        {
            maxID = entry.getID();
        }
    }
    return maxID + 1;
}
JournalEntry *JournalManager::getEntryByViewIndex(int userIndex)
{
    if (userIndex < 1 || userIndex > (int)displayView.size())
    {
        return nullptr;
    }

    return displayView[userIndex - 1];
}
JournalEntry *JournalManager::getEntryByCodeViewIndex(int userIndex)
{
    if (userIndex < 1 || userIndex > (int)codeView.size())
    {
        return nullptr;
    }

    return codeView[userIndex - 1];
}
void JournalManager::printWithCode(SortType type) // todo: make it with good ID's and refactor
{
    display.showCodeList(entries, codeView, type);
}
bool JournalManager::codeCheck()
{
    return std::any_of(entries.begin(), entries.end(), [](const JournalEntry &e)
                       { return !e.getPath().empty(); });
}
void JournalManager::editEntry(int index)
{

    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << COLOR::RED << "✗Error: Entry not found!" << COLOR::RESET << "\n";
        return;
    }

    JournalEntry &entry = *it;
    std::cout << "\n=== EDITING ENTRY #" << index << " ===\n"
              << "Current details:\n"
              << "  Date: " << entry.getDate() << "\n"
              << "  Title: " << entry.getTitle() << "\n"
              << "  Text: " << entry.getText() << "\n"
              << "  Code path: " << entry.getPath() << "\n\n";

    int choice = inputHandler.getInt("What would you like to edit?\n1. Title\n2. Text\n3. Date\n4. Code path\n5. Cancel\nChoice: ");
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::string newValue;

    switch (choice)
    {
    case 1:
        std::cout << "Enter new title: ";
        std::getline(std::cin, newValue);
        entry.setTitle(newValue);
        std::cout << "Title updated!\n";
        break;

    case 2:
        std::cout << "Enter new text: ";
        std::getline(std::cin, newValue);
        entry.setText(newValue);
        std::cout << "Text updated!\n";
        break;

    case 3:
        std::cout << "Enter new date (YYYY-MM-DD): ";
        std::getline(std::cin, newValue);

        if (!validator.isValidDate(newValue))
        {
            std::cerr << "Error: Invalid date format\n";
            return;
        }

        entry.setDate(newValue);
        std::cout << "Date updated!\n";
        break;

    case 4:
        std::cout << "Enter new code path (or 'none'): ";
        std::getline(std::cin, newValue);

        if (newValue != "none" && newValue != CONSTS::NO_CODE_PATH)
        {
            if (!validator.isSafePath(newValue))
            {
                std::cerr << "Error: Invalid or unsafe path\n";
                return;
            }
        }

        entry.setPath(newValue);
        std::cout << "Code path updated!\n";
        break;

    case 5:
        std::cout << "Edit cancelled\n";
        return;

    default:
        std::cerr << COLOR::RED << "✗Error: Invalid choice" << COLOR::RESET << "\n";
        return;
    }

    std::cout << "\nSave changes? (y/n): ";
    char save;
    std::cin >> save;

    if (save == 'y' || save == 'Y')
    {
        saveData(CONSTS::DEFAULT_DATA_FILE);
    }
    display.pressEnterToContinue();
}
void JournalManager::showStatistics() const
{
    if (entries.empty())
    {
        std::cout << "No entries yet.\n";
        return;
    }
    int withCode = std::count_if(entries.begin(), entries.end(),
                                 [](const auto &e)
                                 { return e.getPath() != CONSTS::NO_CODE_PATH; });

    std::cout << "\n=== JOURNAL STATISTICS ===\n";
    std::cout << "Total Entries: " << entries.size() << "\n";
    std::cout << "  With code: " << withCode << "\n";
    std::cout << "  Text only: " << (entries.size() - withCode) << "\n\n";

    auto [oldest, newest] = std::minmax_element(entries.begin(), entries.end(),
                                                [](const auto &a, const auto &b)
                                                { return a.getDate() < b.getDate(); });

    std::cout << "Date Range: " << oldest->getDate()
              << " to " << newest->getDate() << "\n\n";

    std::map<std::string, int> monthCounts;
    for (const auto &entry : entries)
    {
        monthCounts[entry.getDate().substr(0, 7)]++;
    }

    std::cout << "Entries by Month:\n";
    for (const auto &[month, count] : monthCounts)
    {
        std::cout << "  " << month << ": ";
        for (int i = 0; i < count; ++i)
            std::cout << "█";
        std::cout << " (" << count << ")\n";
    }

    display.pressEnterToContinue();
}
void JournalManager::exportMarkdown(const std::string &filename)
{
    if (entries.empty())
    {
        std::cerr << COLOR::RED << "✗Nothing to export!" << COLOR::RESET << "\n";
        return;
    }
    if (storage.exportToMarkdown(entries, filename))
    {
        display.pressEnterToContinue();
    }
}
void JournalManager::importMarkdown(const std::string &filename)
{
    if (storage.importFromMarkdown(entries, filename))
    {
        display.pressEnterToContinue();
    }
}