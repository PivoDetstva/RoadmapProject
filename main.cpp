#include "entry.h"
#include "manager.h"

string filename = "data.txt";

enum enums
{
    add = 1,
    show = 2,
    sexit = 3,
};

void printAll()
{
    std::cout << "printing\n";
    std::ifstream file(filename, std::ios::app);
    if (!file.is_open())
    {
        std::cout << "error opening the file print\n";
        return;
    }
    string line;
    while (std::getline(file, line))
    {
        if (line.empty())
        {
            std::cout << "Nothing on this line\n";
            continue;
        }
        std::cout << line << std::endl;
    }
}

int main()
{
    JournalManager manager;
    manager.loadFromFile(filename);

    int switchinput;
    while (true)
    {
        std::cout << "Welcome to our Roadmap menu, you have 3 choices: \n"
                  << "1.Add Entry\n 2.Show all\n 3.Save and exit\n";

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
            std::cout << "Date: ";
            std::cin >> date;
            std::cout << "Args: ";
            std::cin >> args;
            std::cout << "Path: ";
            std::cin >> path;

            JournalEntry newEntry(date, args, path);
            manager.addEntry(newEntry);

            std::cout << "Added new entry! Not to file yet\n";
            break;
        }
        case show:
        {
            printAll();
            break;
        }
        }
    }
    return 0;
}