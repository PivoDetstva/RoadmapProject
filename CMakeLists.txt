cmake_minimum_required(VERSION 3.22)
project(RoadmapProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_executable(RoadmapProject
   src/main.cpp
    src/core/entry.cpp
    src/core/manager.cpp
    src/io/storage.cpp
    src/io/inputhandle.cpp
    src/search/SearchEngine.cpp
    src/ui/Display.cpp
    src/ui/RoadmapApp.cpp
    src/ui/menu.cpp
    src/valids/validation.cpp
)

target_include_directories(RoadmapProject PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/io
    ${CMAKE_SOURCE_DIR}/src/search
    ${CMAKE_SOURCE_DIR}/src/ui
    ${CMAKE_SOURCE_DIR}/src/valids
)

enable_testing()
add_subdirectory(external/googletest)

add_executable(roadmap_tests
tests/test_entry.cpp
tests/test_storage.cpp
src/core/entry.cpp
src/io/storage.cpp
src/valids/validation.cpp
)
target_include_directories(roadmap_tests PRIVATE 
    ${CMAKE_SOURCE_DIR}/src/include
    ${CMAKE_SOURCE_DIR}/src/valids
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/io

)

target_link_libraries(roadmap_tests PRIVATE gtest_main gtest)

add_test(NAME AllTests COMMAND roadmap_tests)