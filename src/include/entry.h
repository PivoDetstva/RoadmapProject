#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <sstream>
#pragma once

class JournalEntry
{
private:
    int id;
    std::string date;
    std::string title;
    std::string text;
    std::string path;

public:
    JournalEntry() {};

    JournalEntry(int id, std::string date, std::string title, std::string text, std::string path)
        : id(id),
          date(std::move(date)),
          title(std::move(title)),
          text(std::move(text)),
          path(std::move(path))
    {
    }

    std::string serialize() const;
    void deserialize(std::string data);
    int getID() const;
    std::string getDate() const;
    std::string getTitle() const;
    std::string getText() const;
    std::string getPath() const;
    void setDate(const std::string &d);
    void setTitle(const std::string &t);
    void setText(const std::string &a);
    void setPath(const std::string &p);
};