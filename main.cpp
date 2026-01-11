#include "entry.h"
#include "manager.h"

string filename = "data.txt";

enum enums
{
    add = 1,
    show = 2,
    sexit = 3,
    menu = 9,
};

int main()
{
    JournalManager manager;
    manager.loadFromFile(filename);

    std::cout << "Welcome to our Roadmap menu, you have 3 choices: \n"
              << "1.Add Entry\n 2.Show all\n 3.Save and exit\n"
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

        if (switchinput == 3)
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
            std::getline(std::cin, date, ',');
            std::getline(std::cin, args, ',');
            std::getline(std::cin, path);

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
            std::cout << "1.Add Entry\n 2.Show all entries\n"
                      << "3.Save and Exit\n";
            break;
        }
        }
    }
    return 0;
}