#include "manager.h"
#include "storage.h"
#include "Constants.h"
void JournalManager::addEntry(const JournalEntry &entry)
{
    entries.push_back(entry);
}

void JournalManager::saveData(const std::string &filename)
{
    Storage storage;
    storage.saveToFile(entries, CONSTS::filename);
}
void JournalManager::loadData()
{
    Storage storageTool;
    this->entries = storageTool.loadFromFile(CONSTS::filename);
}
void JournalManager::pressEnterToContinue() const
{
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "\nPress Enter to continue...";
    std::cin.get();
}
void JournalManager::printWithIndex(SortType type)
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
                  << " | " << displayView[i]->getTitle() << "\n";
        if (displayView[i]->getPath() != CONSTS::NO_CODE_PATH)
        {
            std::cout << " [Contains code]";
        }
    }
}

void JournalManager::searchByDate(std::string_view queryDate) const
{
    bool found = false;
    for (const auto &entry : entries)
    {
        if (entry.getDate().find(queryDate) != std::string::npos)
        {
            std::cout << entry.getDate() << " | " << entry.getTitle() << std::endl;
            found = true;
            std::cout << "Entry found\n";
        }
    }
    if (!found)
        std::cout << "Not found\n";
    pressEnterToContinue();
}
void JournalManager::deleteEntry(int index)
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << "Error: Entry not found!\n";
        return;
    }

    entries.erase(it);
    std::cout << "Deleted entry #" << index << std::endl;
    pressEnterToContinue();
}
std::string JournalManager::trim(const std::string &s)
{
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}
bool JournalManager::isValidDate(const std::string &date)
{

    std::istringstream ss{date};
    std::chrono::year_month_day ymd;

    if (!(ss >> std::chrono::parse("%Y-%m-%d", ymd)))
    {
        return false;
    }
    return ymd.ok();
}
bool JournalManager::isValidPath(std::string_view pathStr)
{
    return std::filesystem::exists(pathStr);
}
void JournalManager::searchByContent(const std::string &keyword) const
{
    InputHandle inputHandle;
    bool found = false;

    std::string lowerKeyword = keyword;
    inputHandle.toLower(lowerKeyword);

    for (const auto &entry : entries)
    {
        std::string title = entry.getTitle();
        std::string text = entry.getText();
        auto it = std::search(title.begin(), title.end(), lowerKeyword.begin(), lowerKeyword.end(),
                              [](unsigned char ch1, unsigned char ch2)
                              {
                                  return std::tolower(ch1) == ch2;
                              });
        auto it2 = std::search(text.begin(), text.end(), lowerKeyword.begin(), lowerKeyword.end(),
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
    pressEnterToContinue();
}
void JournalManager::previewCode(int index) const
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << "Error: Entry not found!\n";
        return;
    }

    std::string path = it->getPath();

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

void JournalManager::openEntry(int index) const
{
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << "Error: Entry not found!\n";
        return;
    }

    const JournalEntry &entry = *it;
    std::cout << "\t\t" << entry.getTitle()
              << "\n\n"
              << entry.getText()
              << "\n"
              << "Created on " << entry.getDate();
    const auto &path = entry.getPath();
    if (path.length() > 1 && path != CONSTS::NO_CODE_PATH)
    {
        InputHandle input;

        char choice = input.getChar("\nAttached code found. Open file? (y/n): ");
        if (choice == 'y' || choice == 'Y')
        {
            if (isSafePath(entry.getPath()))
            {
                std::string command = "xdg-open '" + entry.getPath() + "'";
                // add the apostrophe reading exception later
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
bool JournalManager::isSafePath(std::string_view pathStr) const
{
    try
    {
        std::filesystem::path filePath(pathStr);

        if (!std::filesystem::exists(filePath))
        {
            std::cerr << "Error: File does not exist\n";
            return false;
        }

        if (!std::filesystem::is_regular_file(filePath))
        {
            std::cerr << "Error: Not a regular file\n";
            return false;
        }

        std::string ext = filePath.extension().string();
        if (ext != ".cpp" && ext != ".h" && ext != ".txt" && ext != ".md")
        {
            std::cerr << "Error: Unsupported file type\n";
            return false;
        } // maybe later extend the variety of acceptable formats, like bash or else

        std::filesystem::path canonicalPath = std::filesystem::canonical(filePath);

        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << "Error: Filesystem error - " << e.what() << "\n";
        return false;
    }
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
void JournalManager::printWithCode(SortType type)
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
    std::cout << "Found " << codecount << " entries!\n";

    for (size_t i = 0; i < displayView.size(); ++i)
    {
        if (displayView[i]->getPath() != CONSTS::NO_CODE_PATH)
        {
            std::cout << "[" << i + 1 << "] " << displayView[i]->getDate()
                      << " | " << displayView[i]->getTitle() << "\n";
        }
    }
    pressEnterToContinue();
}
bool JournalManager::codeCheck()
{
    return std::any_of(entries.begin(), entries.end(), [](const JournalEntry &e)
                       { return !e.getPath().empty(); });
}
void JournalManager::editEntry(int index)
{
    InputHandle inputhandler;
    auto it = std::find_if(entries.begin(), entries.end(),
                           [index](const JournalEntry &e)
                           { return e.getID() == index; });

    if (it == entries.end())
    {
        std::cerr << "Error: Entry not found!\n";
        return;
    }

    JournalEntry &entry = *it;
    std::cout << "\n=== EDITING ENTRY #" << index << " ===\n"
              << "Current details:\n"
              << "  Date: " << entry.getDate() << "\n"
              << "  Title: " << entry.getTitle() << "\n"
              << "  Text: " << entry.getText() << "\n"
              << "  Code path: " << entry.getPath() << "\n\n";

    int choice = inputhandler.getInt("What would you like to edit?\n1. Title\n2. Text\n3. Date\n4. Code path\n5. Cancel\nChoice: ");
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

        if (!isValidDate(newValue))
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
            if (!isSafePath(newValue))
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
        std::cerr << "Error: Invalid choice\n";
        return;
    }

    std::cout << "\nSave changes? (y/n): ";
    char save;
    std::cin >> save;

    if (save == 'y' || save == 'Y')
    {
        saveData(CONSTS::DEFAULT_DATA_FILE);
    }
    pressEnterToContinue();
}
