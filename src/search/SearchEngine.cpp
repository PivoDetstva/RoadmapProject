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
            std::cout << COLOR::GREEN << "✓ Found: " << entry.getDate() << " | " << entry.getTitle() << COLOR::RESET << "\n";
            found = true;
        }
    }
    if (!found)
        std::cout << COLOR::RED << "✗Nothing found." << COLOR::RESET << "\n";
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
            std::cout << COLOR::GREEN << "✓ Entry found" << COLOR::RESET << "\n";
        }
    }
    if (!found)
        std::cout << COLOR::RED << "Not found" << COLOR::RESET << "\n";
} // I believe it's better to make into a template, into one complex method using stl.