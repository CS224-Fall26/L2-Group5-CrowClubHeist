# CMakeLists.txt -> tells CMake how to build the game.
cmake_minimum_required(VERSION 3.10)
project(CrowClubHeist CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Look for SFML (we need the graphics, window and system parts).
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Take every .cpp file in the src folder.
file(GLOB SOURCES src/*.cpp)
add_executable(CrowClubHeist ${SOURCES})
target_link_libraries(CrowClubHeist sfml-graphics sfml-window sfml-system)
