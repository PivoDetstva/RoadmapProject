#include "entry.h"

string JournalEntry::serialize() const
{
    return date + "|" + title + "|" + text + "|" + path;
}
string JournalEntry::shortserialize() const
{
    return date + "|" + title;
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
    std::getline(iss, title, '|');
    std::getline(iss, text, '|');
    std::getline(iss, path, '|');
    std::cout << "Desearilation completed well" << endl;
}
string JournalEntry::getDate() const
{

    return date;
}
string JournalEntry::getTitle() const
{
    return title;
}
string JournalEntry::getText() const
{

    return text;
}
string JournalEntry::getPath() const
{

    return path;
}
void JournalEntry::setDate(string d)
{
    d = date;
}
void JournalEntry::setTitle(string t)
{
    t = title;
}
void JournalEntry::setText(string a)
{
    a = text;
}
void JournalEntry::setPath(string p)
{
    p = path;
}
