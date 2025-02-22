# version 2
# cmake_minimum_required(VERSION 3.10)

# # Set the project name and version
# project(Sharad VERSION 1.0)

# # Specify the C++ standard
# set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_STANDARD_REQUIRED True)

# # Find the ncursesw library
# find_package(Curses REQUIRED)
# include_directories(${CURSES_INCLUDE_DIR})

# # Add the executable
# add_executable(sharad src/main.cpp src/sharad.cpp)

# # Explicitly link the ncursesw library
# target_link_libraries(sharad ncursesw)

cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(Sharad VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Platform-specific settings
if (WIN32)
    # Windows settings with PDCurses
    find_package(PDCurses REQUIRED)
    include_directories(${PDCURSES_INCLUDE_DIR})
    add_definitions(-DPDCURSES)
    set(CURSES_LIBRARIES ${PDCURSES_LIBRARY})
else ()
    # Linux and macOS settings with ncursesw
    set(CURSES_NEED_WIDE TRUE)
    find_package(Curses REQUIRED)
    include_directories(${CURSES_INCLUDE_DIR})
    set(CURSES_LIBRARIES ${CURSES_LIBRARIES})
endif ()

# Add the executable
add_executable(sharad src/main.cpp src/sharad.cpp)

# Link the ncurses or PDCurses library
target_link_libraries(sharad ${CURSES_LIBRARIES})


