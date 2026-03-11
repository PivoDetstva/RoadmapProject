#include "entry.h"

std::string JournalEntry::serialize() const
{
    return std::to_string(id) + "|" + date + "|" + title + "|" + text + "|" + path;
}
void JournalEntry::deserialize(std::string data)
{
    if (data.empty() || data.length() < 2)
    {
        return;
    }

    std::istringstream iss(data);
    std::string tempID;

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
std::string JournalEntry::getDate() const
{

    return date;
}
std::string JournalEntry::getTitle() const
{
    return title;
}
std::string JournalEntry::getText() const
{

    return text;
}
std::string JournalEntry::getPath() const
{

    return path;
}
void JournalEntry::setDate(std::string d)
{
    d = date;
}
void JournalEntry::setTitle(std::string t)
{
    t = title;
}
void JournalEntry::setText(std::string a)
{
    a = text;
}
void JournalEntry::setPath(std::string p)
{
    p = path;
}
