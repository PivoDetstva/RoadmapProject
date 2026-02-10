#include "entry.h"
#include "manager.h"
#include "Constants.h"

string filename = CONSTS::DEFAULT_DATA_FILE;
string tempfile = CONSTS::TEMP_FILE_EXTENSION;
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
    // case export in markdown in future
};

int main()
{
    JournalManager manager;
    manager.loadFromFile(filename);

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
                if (path == CONSTS::NO_CODE_PATH)
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
            int index; // maybe worth switching to string, cause there is no check for user not wanting to open any entry
            // Yep there is a check for 0 but it's another check.
            manager.printWithIndex();
            if (manager.openCheck()) // seems not really good here, but good logic for me
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
            manager.openEntry(index); // maybe make it a cycle? So user would read entry and menu not pop up
            break;
        }
        case menu:
        {
            std::cout << YELLOW << "1.Add Entry" << RESET << "\n"
                      << YELLOW << "2.Show all" << RESET << "\n"
                      << YELLOW << "3.Save and exit" << RESET << "\n"
                      << YELLOW << "4.Search an entry" << RESET << "\n"
                      << YELLOW << "5.Delete an entry" << RESET << "\n"
                      << YELLOW << "6.Preview code" << RESET << "\n";
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
            if (manager.openCheck())
            {
                continue;
            }
            int index;
            std::cout << "Choose which entry to delete: ";
            manager.printWithIndex();
            std::cin >> index;
            if (std::cin.fail())
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cerr << "Input broke\n";
                break;
            }
            manager.deleteEntry(index);
            break;
        }
        case preview:
        {
            if (manager.openCheck())
            {
                continue;
            }
            int index;
            std::cout << "Which entry code you want to see?";
            manager.printWithIndex();
            std::cin >> index;
            manager.previewCode(index);
            break;
        }
        }
        std::this_thread::sleep_for(2.5s); // for better UI, cause this long message trashes the buffer
        std::cout << "Your current options: \n"
                  << YELLOW << "1.Add Entry" << RESET << "\n"
                  << YELLOW << "2.Show all" << RESET << "\n"
                  << YELLOW << "3.Save and exit" << RESET << "\n"
                  << YELLOW << "4.Search an entry" << RESET << "\n"
                  << YELLOW << "5.Delete an entry" << RESET << "\n"
                  << YELLOW << "6.Preview code" << RESET << "\n"
                  << "If you want to see this menu again press '9'\n";
    }
    return 0;
}
