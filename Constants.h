#pragma once
#include <string>

namespace CONSTS
{
    inline const std::string &DEFAULT_DATA_FILE = "data.txt";
    inline const std::string TEMP_FILE_EXTENSION = ".tmp";

    inline const char DEFAULT_XOR_KEY = 'K';

    inline const std::string NO_CODE_PATH = "none";

    inline const std::string filename = CONSTS::DEFAULT_DATA_FILE;
    inline const std::string tempfile = CONSTS::TEMP_FILE_EXTENSION;
}
namespace COLOR
{
    inline const std::string RED = "\033[31m";
    inline const std::string GREEN = "\033[32m";
    inline const std::string YELLOW = "\033[33m";
    inline const std::string BLUE = "\033[34m";
    inline const std::string RESET = "\033[0m";
}