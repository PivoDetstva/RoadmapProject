#include "RoadmapApp.h"

void Menu::displayMainMenu()
{

    std::cout << "\n======= ROADMAP JOURNAL =======\n";
    std::cout << "1. Add entry\n"
              << "2. Show you everything\n"
              << "3. Search entries\n"
              << "4. Delete entry\n"
              << "5. Read entry code\n"
              << "6. Edit already existing entries.\n"
              << "7. Show brief statistics\n"
              << "8. Export your journal\n"
              << "9. Import journal from markdown file\n"
              << "Choose 10 for more detailed guide."
              << "If you want to exit the app press '0'\n"
              << "===============================\n"

              << "Choice: ";
}
void Menu::handleAddEntry()
{
    int id;
    std::string date, title, args, path;
    // clearly can be done with inputhandle
    std::cin.ignore();

    std::cout << "Make an entry! Write date, arguments and path, to divide it use commas.\n";
    while (true)
    {
        std::cout << "Let's start with a date, write a date, or just 'today', use YYYY-MM-DD: ";
        std::getline(std::cin, date);
        date = validator.trim(date);
        if (date == "today")
        {
            auto now = std::chrono::system_clock::now();
            auto localTime = std::chrono::current_zone()->to_local(now);
            date = std::format("{:%Y-%m-%d}", localTime);
        }
        if (date.empty())
        {
            std::cerr << COLOR::RED << "Date cannot be empty!" << COLOR::RESET << "\n";
            continue;
        }
        if (validator.isValidDate(date) == true)
        {
            std::cout << COLOR::GREEN << "Date has been approved!" << COLOR::RESET << "\n";
            break;
        }
        else
        {
            std::cout << COLOR::RED << "✗Invalid date format! Use YYYY-MM-DD" << COLOR::RESET << "\n";
        }
    }
    while (true)
    {
        std::cout << "Now tell what is entry all about: ";
        std::getline(std::cin, title);
        title = validator.trim(title);
        if (title.empty())
        {
            std::cout << COLOR::RED << "✗You haven't provided any text :|" << COLOR::RESET << "\n";
        }
        else
        {
            break;
        }
    }
    while (true)
    {
        std::cout << "And entry text: ";
        std::getline(std::cin, args);
        args = validator.trim(args);
        if (args.empty())
        {
            std::cout << COLOR::RED << "✗You haven't provided any text :|" << COLOR::RESET << "\n";
        }
        else
        {
            break;
        }
    }
    while (true)
    {

        std::cout << "If you want to save the file enter his path, "
                  << "If this is just text entry, enter 'none'\n";
        std::getline(std::cin, path);
        path = validator.trim(path);
        if (path == CONSTS::NO_CODE_PATH)
        {
            std::cout << COLOR::GREEN << "✓Text entry saved\n"
                      << COLOR::RESET;
            break;
        }
        if (validator.isValidPath(path) == false)
        {
            std::cout << COLOR::RED << "✗Invalid path!" << COLOR::RESET << "\n";
        }
        else
        {
            break;
        }
    }
    id = manager.getNextID();
    JournalEntry newEntry(id, date, title, args, path);
    manager.addEntry(newEntry);

    std::cout << COLOR::GREEN << "✓Added new entry!" << COLOR::RESET << "\n";
    display.pressEnterToContinue();
}
void Menu::handleViewEntries()
{
    if (manager.isEmpty())
    {
        std::cout << "\nYou can add an entry manually with 1 menu option or import pre-made file with option 9\n";
        std::cin.ignore();
        display.pressEnterToContinue();
        return;
    }
    manager.printAll(inputHandler.getSortType());
    int index = inputHandler.getInt("Which entry do you want to open?(press '0' to come back to menu)");

    if (index == 0)
    {
        std::cout << "Coming back to menu...";
        std::cin.ignore();
        display.pressEnterToContinue();
        return;
    }

    JournalEntry *selected = manager.getEntryByViewIndex(index);
    if (selected == nullptr)
    {
        std::cout << COLOR::RED << "✗Invalid index! selected\n"
                  << COLOR::RESET;
        return;
    }
    manager.openEntry(selected->getID());
}
void Menu::handleSearch()
{
    if (manager.openCheck())
    {
        std::cin.ignore();
        return;
    }
    std::string input;
    std::cin.ignore();
    std::cout << "Enter a date or any word of the entry: ";

    std::getline(std::cin, input);
    input = validator.trim(input);
    for (char ch : input)
    { // maybe add "search again?" thing with cycle. Do in v1.2 with refactor.
        if (ch >= 48 && ch <= 57 || ch == 45)
        {

            manager.searchByDate(input);
            std::cout << "Found those entries by " << input << " request\n";
            return;
        }
        else
        {

            manager.searchByContent(input);
            std::cout << "Found those entries by " << input << " request\n";
            return;
        }
    }
}
void Menu::handleDelete()
{
    if (manager.openCheck())
    {
        std::cin.ignore();
        return;
    }

    manager.printAll(inputHandler.getSortType());
    int index = inputHandler.getInt("Choose which entry to delete: ");

    JournalEntry *selected = manager.getEntryByViewIndex(index);
    if (selected == nullptr)
    {
        std::cerr << COLOR::RED << "✗Invalid index!" << COLOR::RESET << "\n";
        return;
    }
    std::cout << "\nDelete entry: \"" << selected->getTitle() << "\"?\n";

    std::string confirmation = inputHandler.getString("Type 'yes' to confirm: ");

    if (confirmation == "yes" || confirmation == "y" || confirmation == "ye")
    {
        manager.deleteEntry(selected->getID());
        std::cout << COLOR::GREEN << "✓ Entry deleted\n"
                  << COLOR::RESET;
    }
    else
    {
        std::cout << "Deletion cancelled\n";
    }
    std::cin.ignore();
    display.pressEnterToContinue();
}
void Menu::handlePreviewCode()
{
    if (manager.openCheck())
    {
        std::cin.ignore();
        return;
    }
    if (!manager.codeCheck())
    {
        std::cout << "There is no entries with attached code...\n";
        return;
    }
    manager.printWithCode(inputHandler.getSortType());
    int index = inputHandler.getInt("Which entry code do you want to see?");

    JournalEntry *selected = manager.getEntryByCodeViewIndex(index);
    if (selected == nullptr)
    {
        std::cerr << COLOR::RED << "✗Invalid index!" << COLOR::RESET << "\n";
        return;
    }
    manager.previewCode(selected->getID());
    std::cin.ignore();
    display.pressEnterToContinue();
}
void Menu::handleEdit()
{
    manager.printAll(inputHandler.getSortType());

    int index = inputHandler.getInt("\nEnter entry number to edit: ");
    JournalEntry *selected = manager.getEntryByViewIndex(index);

    if (selected == nullptr)
    {
        std::cerr << COLOR::RED << "✗Invalid index!" << COLOR::RESET << "\n";
        return;
    }
    try
    {
        manager.editEntry(selected->getID());
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << COLOR::RED << "Error: " << e.what() << COLOR::RESET << "\n";
    }
}
void Menu::handleExportMarkdown()
{
    if (manager.openCheck())
    {
        return;
    }
    std::string filename = inputHandler.getString("Export filename (make a name, e.g., journal.md): ");
    validator.trim(filename);
    if (filename.empty()) // should add trim here to parse it with spaces
    {
        filename = validator.defaultExportName();
    }

    if (filename.find(".md") == std::string::npos)
    {
        filename += ".md";
    }

    manager.exportMarkdown(filename);
}
void Menu::handleImportMarkdown()
{
    // make it to chose from /export directory.
    std::string filename = inputHandler.getString("Import from File: ");

    if (!std::filesystem::exists(filename))
    {
        std::cerr << COLOR::RED << "✗ File not found" << COLOR::RESET << "\n";
        return;
    }

    manager.importMarkdown(filename);
}
void Menu::showHelp()
{
    std::cout << "\n=== HELP ===\n\n";
    std::cout << "This is a personal programming journal.\n\n";
    std::cout << "Tips:\n";
    std::cout << "- Add entries as you learn new concepts\n";
    std::cout << "- Attach code files to track your progress\n";
    std::cout << "- Export to Markdown for backup or sharing\n";
    std::cout << "- Use search to find past solutions\n\n";
    std::cout << "Data is stored in: data/data.dat\n";
    std::cout << "Exports are saved in: exports/\n\n";

    std::cin.ignore();
    display.pressEnterToContinue();
}