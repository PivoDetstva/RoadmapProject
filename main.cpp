#include "entry.h"
#include "manager.h"

string filename = "data.txt";
string tempfile = "data.tmp";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string RESET = "\033[0m";

enum enums
{
    add = 1,
    show = 2,
    sexit = 3,
    gofind = 4,
    dont = 5,
    preview = 6,
    menu = 9,
    // case export in plan?
};

int main()
{
    JournalManager manager;
    manager.loadFromFile(filename);

    std::cout << "Welcome to our Roadmap menu, you have 3 choices: \n"
              << "1.Add Entry\n"
              << "2.Show all\n"
              << "3.Save and exit\n"
              << "4.Search an entry\n"
              << "5.Delete an entry\n"
              << "6.Preview code\n"
              << "If you want to see this menu again press '9'\n";
    // I'll need to repeat this after every action made for better UI understanding, but in console
    // it would be a mess, so better have this idea for future
    int switchinput;
    // add ANSI
    while (true)
    {

        if (!(std::cin >> switchinput))
        {
            std::cin.clear();
            std::cin.ignore(10000, '\n');

            continue;
        }

        if (switchinput == sexit)
        {
            manager.saveToFile(filename);
            std::cout << "Goodbye!\n";
            break;
        }
        std::cout << "\033[A\033[2K";
        switch (switchinput)
        {
        case add:
        {
            string date, title, args, path;

            cin.ignore();

            std::cout << "Make an entry! Write date, arguments and path, to divide it use commas.\n";
            while (true)
            {
                std::cout << "Let's start with a date, use YYYY-MM-DD: ";
                std::getline(std::cin, date);
                // IN FUTURE: refactor date, make months and date validation. Maybe change to int. or use date.h
                date = manager.trim(date);
                if (manager.isValidDate(date) == false)
                {
                    std::cout << RED << "Invalid date format! Use YYYY-MM-DD" << RESET << "\n";
                }
                else
                {
                    std::cout << GREEN << "Date has been approved!" << RESET << "\n";
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
                    std::cout << RED << "You haven't provided any text :|" << RESET << "\n";
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
                    std::cout << RED << "You haven't provided any text :|" << RESET << "\n";
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
                if (path == "none")
                {
                    std::cout << GREEN << "Text entry saved\n"
                              << RESET;
                    break;
                }
                if (manager.isValidPath(path) == false)
                {
                    std::cout << RED << "Invalid path!" << RESET << "\n";
                }
                else
                {
                    break;
                }
            }
            JournalEntry newEntry(date, title, args, path);
            manager.addEntry(newEntry);

            std::cout << BLUE << "Added new entry!" << RESET << "\n";
            break;
        }
        case show:
        {
            int index;
            manager.printWIthIndex();
            std::cout << "\nChoose what entry do you want to open: ";
            std::cin >> index;
            manager.openEntry(index);
            break;
        }
        case menu:
        {
            std::cout << "1.Add Entry\n"
                      << "2.Show all entries\n"
                      << "3.Save and Exit\n"
                      << "4. Find by date\n"
                      << "5.Delete entry\n"
                      << "6.Preview code\n"
                      << "9. Preview menu\n";
            break;
        }
        case gofind:
        {
            // todo: when word is mistaken or something, let user make choice again
            //[implement it into while cycle]
            string input;
            cin.ignore();
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
            int index;
            std::cout << "Choose which entry to delete: ";
            manager.printWIthIndex();
            std::cin >> index;
            manager.deleteEntry(index);
            break;
        }
        case preview:
        {
            int index;
            std::cout << "Which entry code you want to see?";
            manager.printWIthIndex();
            std::cin >> index;
            manager.previewCode(index);
            break;
        }
        }
    }
    return 0;
}