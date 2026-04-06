#include "RoadmapApp.h"

void Menu::run()
{
    {
        manager.loadData();
        while (isRunning)
        {
            displayMainMenu();
            int switchinput = inputHandler.getInt("What do you choose?");

            if (switchinput == sexit)
            {
                manager.saveData(CONSTS::filename);
                std::cout << "Goodbye!\n";
                isRunning = false;
            }
            std::cout << "\033[A\033[2K";

            switch (switchinput)
            {
            case add:
            {
                handleAddEntry();
                break;
            }
            case show:
            {
                handleViewEntries();
                break;
            }
            case gofind:
            {
                handleSearch();
                break;
            }
            case dont:
            {
                handleDelete();
                break;
            }
            case preview:
            {
                handlePreviewCode();
                break;
            }
            case edit:
            {
                handleEdit();
                break;
            }
            case stats:
            {
                manager.showStatistics();
                break;
            }
            case exportmd:
            {
                handleExportMarkdown();
                break;
            }
            case importmd:
            {
                handleImportMarkdown();
                break;
            }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}