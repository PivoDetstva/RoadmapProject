#include "entry.h"
#include "manager.h"
#include "Constants.h"
#include "storage.h"
#include "InputClass.h"
#include "Display.h"
#include "validation.h"
#include "manager.h"
#pragma once

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