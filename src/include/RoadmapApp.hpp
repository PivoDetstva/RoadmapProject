#pragma once
#include "entry.hpp"
#include "manager.hpp"
#include "Constants.hpp"
#include "storage.hpp"
#include "InputClass.hpp"
#include "Display.hpp"
#include "validation.hpp"
#include "manager.hpp"

enum enums
{
    ADD = 1,
    LIST = 2,
    FIND = 3,
    DELETE = 4,
    PREVIEW = 5,
    EDIT = 6,
    STATISTICS = 7,
    EXPORT = 8,
    IMPORT = 9,
    HELP = 10,
    EXIT = 0
};

class Menu
{
private:
    JournalManager manager;
    Validator validator;
    InputHandle inputHandler;
    bool isRunning;
    Display display;

public:
    void run();
    void displayMainMenu();
    void handleAddEntry();
    void handleViewEntries();
    void handleSearch();
    void handleDelete();
    void handlePreviewCode();
    void handleEdit();
    void handleExportMarkdown();
    void handleImportMarkdown();
    void showHelp();
};