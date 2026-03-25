The diary Roadmap Project. 
This console(for now) app is made as a some kind of a diary, where programmer can show his way from a some point in his road. This app have functions of adding, editing, deliting and opening entries, with code or not.
The future plan to make it a GUI app based on Qt where all the operations will have a graphic optimisation. You would see entries in a diary and would be able to see a code attached to them. Entries have 5 parts, an exclusive id, which is hiding somewhere inside, so you could sort it for your best view. A date, when it was created or when code attached to entry was written, you can sort entries either by date or an order it was added. The title, basically the key how you would find your entries. Text, explaining the entry, what is inside, in a base list this text is not shown. Path, that's the part where .cpp files can be attached to an entry, for example, if you learned new topic, except just plain text with block of code, you can attach .cpp file as to show a pure practice experience and write some debugging cases that caused with that. 
The application was made using modern c++17 features like std::filesystem for a better and safer file manipulation and std::string_view for a memory efficiency. Also using some C++ 20 methods and libraries like date.h for chrono::parse and chrono::year_month_day. 

For compiling I am using CMake. 

The project now is in v0.9.2 Added new feature. Export with .md file. Being able to insert already made journals, I will contribute my md file later with my programer diary later. But it needs to be polished as well. Started some UI refactoring as well, a few new features coming up before v1.0.

Made by me in 2026. 