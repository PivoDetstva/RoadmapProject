#include "entry.h"

string JournalEntry::serialize() const
{
    return date + " | " + args + " | " + path;
}

void JournalEntry::deserialize(string data)
{
    if (data.empty())
    {
        std::cout << "No data received\n";
        return;
    }
    std::istringstream iss(data);

    std::getline(iss, date, '|');
    std::getline(iss, args, '|');
    std::getline(iss, path, '|');
    std::cout << "Desearilation completed well" << endl;
}
string JournalEntry::getDate() const
{

    return date;
}
string JournalEntry::getArgs()
{

    return args;
}
string JournalEntry::getPath() const
{

    return path;
}
void JournalEntry::setDate(string d)
{
    d = date;
}
void JournalEntry::setArgs(string a)
{
    a = args;
}
void JournalEntry::setPath(string p)
{
    p = path;
}