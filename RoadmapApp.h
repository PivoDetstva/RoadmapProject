#include "entry.h"
#include "manager.h"
#include "Constants.h"
#include "storage.h"
#include "InputClass.h"

enum enums
{
    add = 1,
    show = 2,
    gofind = 3,
    dont = 4,
    preview = 5,
    edit = 6,
    sexit = 0,
    // case export in markdown in future
};

class RoadmapApp
{
private:
    JournalManager manager;
    InputHandle inputHandler;
    bool isRunning;

public:
    void run()
    {
        manager.loadData();
        while (isRunning)
        {
            std::cout << "\n======= ROADMAP JOURNAL =======\n";
            std::cout << "1. Add entry\n"
                      << "2. Show you everything\n"
                      << "3. Search entries\n"
                      << "4. Delete entry\n"
                      << "5. Read entry code\n"
                      << "6. Edit already existing entries.\n"
                      << "If you want to exit the app press '0'\n"
                      << "===============================\n"
                      << "Choice: ";

            int switchinput = inputHandler.getInt("What do you choose?");

            if (switchinput == sexit)
            {
                manager.saveData(CONSTS::filename);
                std::cout << "Goodbye!\n";
                isRunning = false;
            }
            std::cout << "\033[A\033[2K";

            switch (switchinput)
            {
            case add:
            {
                int id;
                std::string date, title, args, path;

                std::cin.ignore();

                std::cout << "Make an entry! Write date, arguments and path, to divide it use commas.\n";
                while (true)
                {
                    std::cout << "Let's start with a date, use YYYY-MM-DD: ";
                    std::getline(std::cin, date);
                    date = manager.trim(date);
                    if (manager.isValidDate(date) == false)
                    {
                        std::cout << COLOR::RED << "Invalid date format! Use YYYY-MM-DD" << COLOR::RESET << "\n";
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
                    title = manager.trim(title);
                    if (title.empty())
                    {
                        std::cout << COLOR::RED << "You haven't provided any text :|" << COLOR::RESET << "\n";
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
                    args = manager.trim(args);
                    if (args.empty())
                    {
                        std::cout << COLOR::RED << "You haven't provided any text :|" << COLOR::RESET << "\n";
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
                    path = manager.trim(path);
                    if (path == CONSTS::NO_CODE_PATH)
                    {
                        std::cout << COLOR::GREEN << "Text entry saved\n"
                                  << COLOR::RESET;
                        break;
                    }
                    if (manager.isValidPath(path) == false)
                    {
                        std::cout << COLOR::RED << "Invalid path!" << COLOR::RESET << "\n";
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
                break;
            }
            case show:
            {

                manager.printWithIndex(inputHandler.getSortType());
                if (manager.openCheck())
                {
                    continue;
                }
                int index = inputHandler.getInt("Which entry do you want to open?(print '0' if none)");

                JournalEntry *selected = manager.getEntryByViewIndex(index);
                if (selected == nullptr)
                {
                    std::cout << "Invalid index!\n";
                    break;
                }
                manager.openEntry(selected->getID()); // maybe make it a cycle? So user would read entry and menu not pop up
                break;
            }
            case gofind:
            {
                // todo: when word is mistaken or something, let user make choice again
                //[implement it into while cycle]
                if (manager.openCheck())
                {
                    continue;
                }
                std::string input;
                std::cin.ignore();
                std::cout << "Enter a date or any word of the entry: ";

                std::getline(std::cin, input);
                input = manager.trim(input);
                for (char ch : input)
                {
                    if (ch >= 48 && ch <= 57 || ch == 45)
                    {
                        std::cout << "Found those entries by " << input << "request\n";
                        manager.searchByDate(input);
                        // here should lie open entry logic
                        break;
                    }
                    else
                    {
                        std::cout << "Found those entries by " << input << "request\n";
                        manager.searchByContent(input);
                        // here should lie open entry logic
                        break;
                    }
                }
                break;
            }
            case dont:
            {
                if (manager.openCheck())
                {
                    continue;
                }

                manager.printWithIndex(inputHandler.getSortType());
                int index = inputHandler.getInt("Choose which entry to delete: ");

                JournalEntry *selected = manager.getEntryByViewIndex(index);
                if (selected == nullptr)
                {
                    std::cout << "Invalid index!\n";
                    break;
                }
                manager.deleteEntry(selected->getID());
                break;
            }
            case preview:
            {
                if (manager.openCheck())
                {
                    continue;
                }
                if (manager.codeCheck())
                {
                    std::cout << "There is no entries with attached code...\n";
                    break;
                }
                manager.printWithCode(inputHandler.getSortType());
                int index = inputHandler.getInt("Which entry code do you want to see?");

                JournalEntry *selected = manager.getEntryByViewIndex(index);
                if (selected == nullptr)
                {
                    std::cout << "Invalid index!\n";
                    break;
                }
                manager.previewCode(selected->getID());
                break;
            }
            case edit:
            {
                manager.printWithIndex(inputHandler.getSortType());

                int index = inputHandler.getInt("\nEnter entry number to edit: ");
                JournalEntry *selected = manager.getEntryByViewIndex(index);

                if (selected == nullptr)
                {
                    std::cout << "Invalid index!\n";
                    break;
                }
                try
                {
                    manager.editEntry(selected->getID());
                }
                catch (const std::out_of_range &e)
                {
                    std::cerr << "Error: " << e.what() << "\n";
                }
                break;
            }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
};