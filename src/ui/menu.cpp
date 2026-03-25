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
        std::cout << "Let's start with a date, use YYYY-MM-DD: ";
        std::getline(std::cin, date);
        date = validator.trim(date);
        if (validator.isValidDate(date) == false)
        {
            std::cout << COLOR::RED << "✗Invalid date format! Use YYYY-MM-DD" << COLOR::RESET << "\n";
        }
        else
        {
            std::cout << COLOR::GREEN << "Date has been approved!" << COLOR::RESET << "\n";
            break;
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

    std::cout << COLOR::BLUE << "Added new entry!" << COLOR::RESET << "\n";
}
void Menu::handleViewEntries()
{

    manager.printAll(inputHandler.getSortType());
    if (manager.openCheck())
    {
        std::cout << "✗Error, opencheck hasn't passed\n";
        return;
    }
    int index = inputHandler.getInt("Which entry do you want to open?(press '0' to come back to menu)");

    if (index == 0)
    {
        std::cout << "Coming back to menu...\n";
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
    manager.openEntry(selected->getID()); // maybe make it a cycle? So user would read entry and menu not pop up
}
void Menu::handleSearch()
{
    // todo: when word is mistaken or something, let user make choice again
    //[implement it into while cycle]
    if (manager.openCheck())
    {
        std::cout << COLOR::RED << "✗didn't passed through opencheck\n"
                  << COLOR::RESET;
        return;
    }
    std::string input;
    std::cin.ignore();
    std::cout << "Enter a date or any word of the entry: ";

    std::getline(std::cin, input);
    input = validator.trim(input);
    for (char ch : input)
    { // maybe add "search again?" thing with cycle
        if (ch >= 48 && ch <= 57 || ch == 45)
        {
            std::cout << "Found those entries by " << input << "request\n";
            manager.searchByDate(input);
            return;
        }
        else
        {
            std::cout << "Found those entries by " << input << "request\n";
            manager.searchByContent(input);
            return;
        }
    }
}
void Menu::handleDelete()
{
    if (manager.openCheck())
    {
        std::cout << "didn't passed the opencheck\n";
        return;
    }

    manager.printAll(inputHandler.getSortType());
    int index = inputHandler.getInt("Choose which entry to delete: ");

    JournalEntry *selected = manager.getEntryByViewIndex(index);
    if (selected == nullptr)
    {
        std::cout << "Invalid index!\n";
        return;
    }
    manager.deleteEntry(selected->getID());
}
void Menu::handlePreviewCode()
{
    if (manager.openCheck())
    {
        std::cout << "openerror\n";
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
        std::cout << "Invalid index! selected\n";
        return;
    }
    manager.previewCode(selected->getID());
}
void Menu::handleEdit()
{
    manager.printAll(inputHandler.getSortType());

    int index = inputHandler.getInt("\nEnter entry number to edit: ");
    JournalEntry *selected = manager.getEntryByViewIndex(index);

    if (selected == nullptr)
    {
        std::cout << "Invalid index!\n";
        return;
    }
    try
    {
        manager.editEntry(selected->getID());
    }
    catch (const std::out_of_range &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
    }
}
void Menu::handleExportMarkdown()
{
    if (manager.openCheck())
    {
        std::cout << "Open check not passed\n";
        return;
    }
    std::string filename;
    std::cout << "Export filename(make up a file name, e.g., journal.md): ";
    std::getline(std::cin, filename);

    if (filename.empty())
    {
        filename = CONSTS::STANDART_MARKDOWN_FILE;
    }

    if (filename.find(".md") == std::string::npos)
    {
        filename += ".md";
    }

    manager.exportMarkdown(filename);
}