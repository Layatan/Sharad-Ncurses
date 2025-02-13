# cmake_minimum_required(VERSION 3.10)

# # Proj name, C++ standard
# project(Sharad VERSION 1.0 LANGUAGES CXX)
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# find_package(Curses REQUIRED)

# add_executable(sharad src/main.cpp src/sharad.cpp)

# # Link ncurses library
# target_link_libraries(sharad ${CURSES_LIBRARIES})

# # Include ncurses headers
# target_include_directories(sharad PRIVATE ${CURSES_INCLUDE_DIRS})

cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Sharad VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the ncursesw library
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

# Add the executable
add_executable(sharad src/main.cpp src/sharad.cpp)

# Explicitly link the ncursesw library
target_link_libraries(sharad ncursesw)

