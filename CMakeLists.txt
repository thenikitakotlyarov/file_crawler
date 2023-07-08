cmake_minimum_required(VERSION 3.10)

# set the project name
project(Game)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_compile_options(-Wno-deprecated-declarations) # suppresses warnings
add_compile_options(-g) # debugging
# Grab all the source filesasdfasdf
file(GLOB_RECURSE SOURCES
        "src/*.cpp")

# add the executable
add_executable(Game Main.cpp Game.cpp ${SOURCES})

# Link against required libraries
target_link_libraries(Game ncurses noise ssl crypto SDL2 SDL2_mixer)

# If you have include directories, you can include them like this:
# include_directories(your_include_directory)
