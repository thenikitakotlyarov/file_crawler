cmake_minimum_required(VERSION 3.10)

# set the project name
project(Game)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(src)

add_compile_options(-Wno-deprecated-declarations) # suppresses warnings
add_compile_options(-g) # debugging

# Grab all the source files
file(GLOB SOURCES
        "src/*.cpp"
        "src/utilities/*.cpp" "src/utilities/math/*.cpp" "src/utilities/noise/*.cpp" "src/utilities/pathfinding/*.cpp" "src/utilities/render/*.cpp"
        "src/templates/*.cpp" "src/templates/enemy/*.cpp" "src/templates/item/*.cpp" "src/templates/player/*.cpp"
        "src/systems/*.cpp" "src/systems/audio/*.cpp" "src/systems/entity/*.cpp" "src/systems/generation/*.cpp" "src/systems/graphics/*.cpp" "src/systems/input/*.cpp" "src/systems/interface/*.cpp" "src/systems/logging/*.cpp" "src/systems/pathfinding/*.cpp"
        "src/components/*.cpp" "src/components/enemy/*.cpp" "src/components/item/*.cpp" "src/components/player/*.cpp" "src/components/menu/*.cpp"
        )

# add the executable
add_executable(Game Main.cpp ${SOURCES} src/systems/entity/Entity.cpp src/systems/entity/Entity.h)

# Link against required libraries
target_link_libraries(Game ncurses noise ssl crypto SDL2 SDL2_mixer)

# If you have include directories, you can include them like this:
# include_directories(your_include_directory)
