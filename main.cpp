#include "entry.h"
#include "manager.h"
#include "Constants.h"
#include "storage.h"

enum enums
{
    add = 1,
    show = 2,
    sexit = 3,
    gofind = 4,
    dont = 5,
    preview = 6,
    menu = 9,
    // case export in markdown in future
};

int main()
{
    JournalManager manager;

    manager.loadData();

    std::cout << "Welcome to our Roadmap menu, you have 6 choices: \n"
              << "1.Add Entry\n"
              << "2.Show all\n"
              << "3.Save and exit\n"
              << "4.Search an entry\n"
              << "5.Delete an entry\n"
              << "6.Preview code\n"
              << "If you want to see this menu again press '9'\n";
    int switchinput;
    while (true)
    {

        if (!(std::cin >> switchinput))
        {
            if (std::cin.eof())
                break;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Input broke\n";
            continue;
        }

        if (switchinput == sexit)
        {
            manager.saveData(CONSTS::filename);
            std::cout << "Goodbye!\n";
            break;
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
            int index;
            SortType type;
            std::string input;
            std::cout << "How do you want to sort the entries?\n";
            std::cin >> input;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
            manager.toLower(input);
            if (input == "date")
            {
                type = SortType::BY_DATE;
            }
            else if (input == "id")
            {
                type = SortType::BY_ID;
            }
            else
            {
                std::cout << "Wrong formation of sorting\n";
                break;
            }
            manager.printWithIndex(type);
            if (manager.openCheck())
            {
                continue;
            }
            std::cout << "\nChoose what entry do you want to open: ";
            std::cin >> index;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
            JournalEntry *selected = manager.getEntryByViewIndex(index);
            if (selected == nullptr)
            {
                std::cout << "Invalid index!\n";
                break;
            }
            manager.openEntry(selected->getID()); // maybe make it a cycle? So user would read entry and menu not pop up
            break;
        }
        case menu:
        {
            std::cout << COLOR::YELLOW << "1.Add Entry" << COLOR::RESET << "\n"
                      << COLOR::YELLOW << "2.Show all" << COLOR::RESET << "\n"
                      << COLOR::YELLOW << "3.Save and exit" << COLOR::RESET << "\n"
                      << COLOR::YELLOW << "4.Search an entry" << COLOR::RESET << "\n"
                      << COLOR::YELLOW << "5.Delete an entry" << COLOR::RESET << "\n"
                      << COLOR::YELLOW << "6.Preview code" << COLOR::RESET << "\n";
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
            int index;
            SortType type;
            std::string input;
            std::cout << "How do you want to sort the entries?\n";
            std::cin >> input;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }

            manager.toLower(input);
            if (input == "date")
            {
                type = SortType::BY_DATE;
            }
            else if (input == "id")
            {
                type = SortType::BY_ID;
            }
            else
            {
                std::cout << "Wrong formation of sorting\n";
                break;
            }
            std::cout << "Choose which entry to delete: ";
            manager.printWithIndex(type);
            std::cin >> index;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
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
            int index;
            SortType type;
            std::string input;
            std::cout << "How do you want to sort the entries?\n";
            std::cin >> input;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
            manager.toLower(input);
            if (input == "date")
            {
                type = SortType::BY_DATE;
            }
            else if (input == "id")
            {
                type = SortType::BY_ID;
            }
            else
            {
                std::cout << "Wrong formation of sorting\n";
                break;
            }
            std::cout << "Which entry code you want to see?";
            manager.printWithIndex(type);
            std::cin >> index;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
            JournalEntry *selected = manager.getEntryByViewIndex(index);
            if (selected == nullptr)
            {
                std::cout << "Invalid index!\n";
                break;
            }
            manager.previewCode(selected->getID());
            break;
        }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1)); // for better UI, cause this long message trashes the buffer
        std::cout << "Your current options: \n"
                  << COLOR::YELLOW << "1.Add Entry" << COLOR::RESET << "\n"
                  << COLOR::YELLOW << "2.Show all" << COLOR::RESET << "\n"
                  << COLOR::YELLOW << "3.Save and exit" << COLOR::RESET << "\n"
                  << COLOR::YELLOW << "4.Search an entry" << COLOR::RESET << "\n"
                  << COLOR::YELLOW << "5.Delete an entry" << COLOR::RESET << "\n"
                  << COLOR::YELLOW << "6.Preview code" << COLOR::RESET << "\n"
                  << "If you want to see this menu again press '9'\n";
    }
    return 0;
}
