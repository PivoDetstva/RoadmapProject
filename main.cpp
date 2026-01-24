#include "entry.h"
#include "manager.h"

string filename = "data.txt";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
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
};

int main()
{
    JournalManager manager;
    manager.loadFromFile(filename);

    std::cout << "Welcome to our Roadmap menu, you have 3 choices: \n"
              << "1.Add Entry\n"
              << "2.Show all\n"
              << "3.Save and exit\n"
              << "4.Search by date(not working for now, will be fixed in futue)\n"
              << "5.Delete an entry\n"
              << "6.Preview code\n"
              << "If you want to see this menu again press '9'\n";
    // I'll need to repeat this after every action made for better UI understanding, but in console
    // it would be a mess, so better have this idea for
    int switchinput;
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

        switch (switchinput)
        {
        case add:
        {
            string date, args, path;

            cin.ignore();

            std::cout << "Make an entry! Write date, arguments and path, to divide it use commas.\n";
            while (true)
            {
                std::cout << "Let's start with a date: ";
                std::getline(std::cin, date);
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

                std::cout << "And where does it lies: ";
                std::getline(std::cin, path);
                path = manager.trim(path);
                if (manager.isValidPath(path) == false)
                {
                    std::cout << RED << "Invalid path!" << RESET << "\n";
                }
                else
                {
                    break;
                }
            }
            JournalEntry newEntry(date, args, path);
            manager.addEntry(newEntry);

            std::cout << "Added new entry!\n";
            break;
        }
        case show:
        {
            manager.printAll();
            break;
        }
        case menu:
        {
            std::cout << "1.Add Entry\n"
                      << "2.Show all entries\n"
                      << "3.Save and Exit\n"
                      << "4. Find by date\n"
                      << "5.Delete entry\n";
            break;
        }
        case gofind:
        {
            string date;
            cin.ignore();
            std::cout << "Enter a date of the entry: ";
            std::getline(std::cin, date);
            date = manager.trim(date);
            if (!date.empty() && date[0] == ' ')
            {
                date.erase(0, 1);
            }
            manager.searchByDate(date);
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