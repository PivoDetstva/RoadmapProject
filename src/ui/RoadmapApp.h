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
    add = 1,
    show = 2,
    gofind = 3,
    dont = 4,
    preview = 5,
    edit = 6,
    stats = 7,
    exportmd = 8,
    sexit = 0
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
};