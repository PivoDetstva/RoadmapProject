#include "validation.h"

std::string Validator::trim(const std::string &s)
{
    size_t first = s.find_first_not_of(" \t\n\r");
    if (first == std::string::npos)
        return "";
    size_t last = s.find_last_not_of(" \t\n\r");
    return s.substr(first, (last - first + 1));
}
bool Validator::isValidDate(const std::string &date)
{

    std::istringstream ss{date};
    std::chrono::year_month_day ymd;

    if (!(ss >> std::chrono::parse("%Y-%m-%d", ymd)))
    {
        return false;
    }
    return ymd.ok();
}
bool Validator::isValidPath(std::string_view pathStr)
{
    return std::filesystem::exists(pathStr);
}
bool Validator::isSafePath(std::string_view pathStr) const
{
    try
    {
        std::filesystem::path filePath(pathStr);

        if (!std::filesystem::exists(filePath))
        {
            std::cerr << COLOR::RED << "Error: File does not exist" << COLOR::RESET << "\n";
            return false;
        }

        if (!std::filesystem::is_regular_file(filePath))
        {
            std::cerr << COLOR::RED << "Error: Not a regular file" << COLOR::RESET << "\n";
            return false;
        }

        std::string ext = filePath.extension().string();
        if (ext != ".cpp" && ext != ".h" && ext != ".txt" && ext != ".md")
        {
            std::cerr << COLOR::RED << "Error: Unsupported file type" << COLOR::RESET << "\n";
            return false;
        } // maybe later extend the variety of acceptable formats, like bash or else

        std::filesystem::path canonicalPath = std::filesystem::canonical(filePath);

        return true;
    }
    catch (const std::filesystem::filesystem_error &e)
    {
        std::cerr << COLOR::RED << "Error: Filesystem error - " << e.what() << COLOR::RESET << "\n";
        return false;
    }
}
std::string Validator::defaultExportName()
{
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&time), "journal-%Y-%m-%d.md");
    return oss.str();
}