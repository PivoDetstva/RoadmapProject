#include "entry.h"
#include "manager.h"

string filename = "data.txt";

enum enums
{
    add = 1,
    show = 2,
    sexit = 3,
    gofind = 4,
    dont = 5,
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
              << "4.Search by date\n"
              << "5.Delete an entry\n"
              << "If you want to see this menu again press '9'\n";

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
            // remake to a single argument later
            std::getline(std::cin, date, ',');

            if (!date.empty() && date[0] == ' ')
            {
                date.erase(0, 1);
            }
            manager.trim(date);
            manager.isValidDate(date);
            std::getline(std::cin, args, ',');
            if (!args.empty() && args[0] == ' ')
            {
                args.erase(0, 1);
            }
            std::getline(std::cin, path);
            if (!path.empty() && path[0] == ' ')
            {
                path.erase(0, 1);
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
        }
    }
    return 0;
}