cmake_minimum_required(VERSION 3.22)
project(RoadmapProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(RoadmapProject
    main.cpp 
    entry.cpp
    manager.cpp
    storage.cpp
    inputhandle.cpp
    Display.cpp
)