#include <gtest/gtest.h>
#include "storage.h"
#include <filesystem>

TEST(StorageTest, ExportCreatesFile)
{
    Storage storage;
    std::vector<JournalEntry> entries;
    entries.emplace_back(1, "2024-01-01", "Test", "Content", "none");

    const std::string testFile = "test_export.md";

    ASSERT_TRUE(storage.exportToMarkdown(entries, testFile));
    EXPECT_TRUE(std::filesystem::exists(testFile));

    // Cleanup
    std::filesystem::remove(testFile);
}

TEST(StorageTest, ExportContainsTitle)
{
    Storage storage;
    std::vector<JournalEntry> entries;
    entries.emplace_back(1, "2024-01-01", "My Title", "Content", "none");

    const std::string testFile = "test_export.md";
    storage.exportToMarkdown(entries, testFile);

    std::ifstream file(testFile);
    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    EXPECT_NE(content.find("## My Title"), std::string::npos);

    std::filesystem::remove(testFile);
}