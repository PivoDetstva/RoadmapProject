#include "RoadmapApp.h"

int main()
{
    std::filesystem::path exePath = std::filesystem::canonical("/proc/self/exe");
    std::filesystem::path projectRoot = exePath.parent_path().parent_path();
    std::filesystem::current_path(projectRoot);
    std::filesystem::create_directories("data");
    std::filesystem::create_directories("exports");

    Menu app;
    app.run();
    return 0;
}