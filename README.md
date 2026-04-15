# 🗺️ RoadmapProject

> A modern CLI journal for tracking your programming journey

![The Roadmap Menu](image.png)


![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)

## ✨ Features

- 📝 **Add, edit, delete entries** with timestamps
- 🔍 **Search by date or content**
- 💾 **Export to Markdown** for sharing or backup
- 📊 **Statistics dashboard** showing your progress
- 🔒 **XOR encryption** for data privacy
- 📎 **Attach code files** to entries
- 🎨 **Clean CLI interface** with color-coded output


### Build from Source

**Requirements:**
- C++20 compatible compiler (g++ 11+, clang 13+)
- CMake 3.22+
- Git (for submodules)

## 📖 Here is a quick guide about functionality

### Adding an Entry

Choose option 1 in menu
Enter date (YYYY-MM-DD), or simply write "today".
Enter title and content
Optionally attach a code file

### List everything

Choose option 2 in menu
Prints every saved entry
Allows to choose the sorting method, if you're searching for a perticular one
But if there is a lot of entries and you need to find and read a special one you can...


### Search Entries

Choose option 3 in menu
Parsing the user input to find any matches with logged entries, either that date, text, title.
If there is no matches, you can try again, or choose different option.

### Delete entry

Choose option 4 in menu
Deletes a chosen entry.
Again, you can sort your view how you like, either by date, or id.
To make it clear, sorting by ID is to show entries in the order they were added.
You can permanently delete an entry, but to be sure it's the right one you need!

### Preview code

Choose option 5 in menu
Read the attached code to entry.
While adding entries you can either attach code path, if your entry was about a little practice or project.
Lists only entries with attached code, if there is none, then nothing to list!

### Edit entry!

Choose option 6 in menu
Lists entries by sorting method you choose.
You can edit every detail of your entry, if you're made a typo, or you can add code if you forgot, or change the file.
Allows you to change already created entries.

### Statistics

Choose option 7 in menu
Will show you brief statistics of your diary. How many entries you made.
How long you have been doing this
How much steps you have done from the start of this programming journey.

### Exporting to Markdown

Choose option 8 in menu
Specify filename (e.g., my-journal.md)
Exports your journal to a Markdown file
You can either save it for yourself as a backup or share with friend to...

### Import Markdown

Choose option 9 in menu
Allows you to import journals from 0, or looking for other diaries locally.
Can be used to retrieve backups, but mainly because why export if there is no import hehe

### For a more detailed explanation call "-help" in the app choosing options (*in progress*)

## 🏗️ Project Architecture

RoadmapProject/
├── src/
│   ├── core/          # Business logic (Entry, Manager)
│   ├── io/            # File operations (Storage, InputHandle)
│   ├── search/        # Search engine
│   ├── ui/            # Display and Menu
│   └── valids/        # Input validation & security
├── tests/             # Google Test unit tests
├── external/          # Dependencies (googletest)
└── Dockerfile         # Container configuration

**Design Principles:**
- Object-Oriented Programming (Incapsulation and safety at the first place)
- Single Responsibility (each class has one job)
- Dependency injection (Manager uses Storage, not inherits)
- Security-first (path validation, encryption)

## 🧪 Testing

```bash
# Build and run tests
cmake -B build && cmake --build build
./build/roadmap_tests

# Or use ctest
cd build && ctest --verbose
```

**Coverage:** Core business logic (Entry, Storage)
P.S. For simple CLI it was enough for me just to learn how unit tests works, now app is almost fully working on user input, so tests now is not making a lot of effort.

## Docker (Optional)

```bash
docker build -t roadmap:v1.0 .
docker run -it roadmap:v1.0
```

## 🛠️ Technical Stack

- **Language:** C++20
- **Build System:** CMake
- **Testing:** Google Test
- **Libraries:** STL, `<filesystem>`, `<chrono>`
- **Containerization:** Docker
- **Version Control:** Git with submodules

## Project is in v0.9.8 version, README is the most valuable change for now! 
Implemented Docker into project with a new functionalities. 

Here should lie a future project roadmap!

Made by me in 2026.

**Note:** This is a portfolio project demonstrating C++ skills.