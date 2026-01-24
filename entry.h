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
    std::string args;
    std::string path;
    std::string title;

public:
    JournalEntry() {};

    JournalEntry(std::string newDate, std::string newArgs, std::string newPath)
    {
        date = newDate;
        args = newArgs;
        path = newPath;
    }

    std::string serialize() const;
    void deserialize(std::string data);
    std::string getDate() const;
    std::string getArgs();
    std::string getPath() const;
    void setDate(string d);
    void setArgs(string a);
    void setPath(string p);
};