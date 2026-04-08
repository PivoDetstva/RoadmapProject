#include "entry.h"
#include <gtest/gtest.h>

TEST(JournalEntryTest, ConstructorSetsValues)
{
    JournalEntry entry(1, "2024-01-01", "Title", "Text", "none");

    EXPECT_EQ(entry.getID(), 1);
    EXPECT_EQ(entry.getDate(), "2024-01-01");
    EXPECT_EQ(entry.getTitle(), "Title");
    EXPECT_EQ(entry.getText(), "Text");
    EXPECT_EQ(entry.getPath(), "none");
}
// It's just small tests for me to learn how it works. So far so good!
TEST(JournalEntryTest, SettersWork)
{
    JournalEntry entry(1, "2024-01-01", "Title", "Text", "none");

    entry.setTitle("New Title");
    entry.setText("New Text");
    entry.setDate("2024-12-31");
    entry.setPath("/path/to/code.cpp");

    EXPECT_EQ(entry.getTitle(), "New Title");
    EXPECT_EQ(entry.getText(), "New Text");
    EXPECT_EQ(entry.getDate(), "2024-12-31");
    EXPECT_EQ(entry.getPath(), "/path/to/code.cpp");
}