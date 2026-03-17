#include "InputClass.h"

void InputHandle::toLower(std::string &s) const
{
    for (char &c : s)
    {
        c = std::tolower(static_cast<unsigned char>(c));
    }
}
int InputHandle::getInt(std::string prompt)
{
    while (true)
    {
        std::cout << prompt << std::endl;
        int input;
        std::cin >> input;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error! It's not a number\n";
            continue;
        }
        return input;
    }
}
char InputHandle::getChar(std::string prompt) const
{
    while (true)
    {
        std::cout << prompt << std::endl;
        char input;
        std::cin >> input;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error! \n";
            continue;
        }
        return input;
    }
}
std::string InputHandle::getString(std::string prompt)
{
    while (true)
    {
        std::cout << prompt << std::endl;
        std::string input;
        std::cin >> input;
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error! \n";
            continue;
        }
        return input;
    }
}

SortType InputHandle::getSortType()
{
    while (true)
    {
        SortType type;
        InputHandle inputHandle;
        std::string input = inputHandle.getString("How do you want to sort the list?(date/id)");
        if (std::cin.fail())
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cerr << "Error! Sorting is not chosen correctly\n";
            continue;
        }
        inputHandle.toLower(input);
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
            continue;
        }
        return type;
    }
}