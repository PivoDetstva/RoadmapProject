#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <atomic>
#include <sstream>
#pragma once

using namespace std;

class JournalEntry
{
private:
    std::string date;
    std::string title;
    std::string text;
    std::string path;

public:
    JournalEntry() {};

    JournalEntry(std::string newDate, std::string newTitle, std::string newText, std::string newPath)
    {
        date = newDate;
        title = newTitle;
        text = newText;
        path = newPath;
    }

    std::string serialize() const;
    void deserialize(std::string data);
    std::string getDate() const;
    std::string getTitle() const;
    std::string getText() const;
    std::string getPath() const;
    void setDate(string d);
    void setTitle(string t);
    void setText(string a);
    void setPath(string p);
};