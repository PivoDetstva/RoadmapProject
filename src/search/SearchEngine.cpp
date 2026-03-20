#include "SearchEngine.h"

void Finder::searchContent(const std::string &keyword, std::vector<JournalEntry> &entries)
{

    bool found = false;

    std::string lowerKeyword = keyword;
    inputHandler.toLower(lowerKeyword);

    for (auto &entry : entries)
    {
        std::string title = entry.getTitle();
        std::string text = entry.getText();
        auto it = std::search(title.begin(), title.end(), lowerKeyword.begin(), lowerKeyword.end(),
                              [](unsigned char ch1, unsigned char ch2)
                              {
                                  return std::tolower(ch1) == ch2;
                              });
        auto it2 = std::search(text.begin(), text.end(), lowerKeyword.begin(), lowerKeyword.end(),
                               [](unsigned char ch1, unsigned char ch2)
                               {
                                   return std::tolower(ch1) == ch2;
                               });
        if (it != title.end() || it2 != text.end())
        {
            std::cout << "Found: " << entry.getDate() << " | " << entry.getTitle() << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << "Nothing found.\n";
}
void Finder::searchDate(std::string_view queryDate, std::vector<JournalEntry> &entries) const
{
    bool found = false;
    for (const auto &entry : entries)
    {
        if (entry.getDate().find(queryDate) != std::string::npos)
        {
            std::cout << entry.getDate() << " | " << entry.getTitle() << std::endl;
            found = true;
            std::cout << "Entry found\n";
        }
    }
    if (!found)
        std::cout << "Not found\n";
}