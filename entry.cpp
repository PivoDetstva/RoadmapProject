#include "entry.h"

string JournalEntry::serialize() const
{
    return std::to_string(id) + "|" + date + "|" + title + "|" + text + "|" + path;
}
void JournalEntry::deserialize(string data)
{
    if (data.empty() || data.length() < 2)
    {
        return;
    }

    std::istringstream iss(data);
    string tempID;

    if (std::getline(iss, tempID, '|'))
    {
        try
        {
            // Пытаемся превратить в число только если в строке есть хоть одна цифра
            if (!tempID.empty() && std::isdigit(tempID[0]))
            {
                id = std::stoi(tempID);
            }
        }
        catch (...)
        {
            id = 0; // Если случилась беда, просто ставим 0, но не падаем
        }
    }

    std::getline(iss, date, '|');
    std::getline(iss, title, '|');
    std::getline(iss, text, '|');
    std::getline(iss, path, '|');
}
int JournalEntry::getID() const
{
    return id;
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
