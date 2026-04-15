#include "RoadmapApp.h"

void Menu::run()
{
    {
        manager.loadData();
        while (isRunning)
        {
            displayMainMenu();
            int switchinput = inputHandler.getInt("What do you choose?");

            if (switchinput == EXIT)
            {
                manager.saveData(CONSTS::filename);
                std::cout << "Goodbye!\n";
                isRunning = false;
            }
            std::cout << "\033[A\033[2K";

            switch (switchinput)
            {
            case ADD:
            {
                handleAddEntry();
                break;
            }
            case LIST:
            {
                handleViewEntries();
                break;
            }
            case FIND:
            {
                handleSearch();
                break;
            }
            case DELETE:
            {
                handleDelete();
                break;
            }
            case PREVIEW:
            {
                handlePreviewCode();
                break;
            }
            case EDIT:
            {
                handleEdit();
                break;
            }
            case STATISTICS:
            {
                manager.showStatistics();
                std::cin.ignore();
                display.pressEnterToContinue();
                break;
            }
            case EXPORT:
            {
                handleExportMarkdown();
                break;
            }
            case IMPORT:
            {
                handleImportMarkdown();
                break;
            }
            case HELP:
            {
                showHelp();
                break;
            }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}