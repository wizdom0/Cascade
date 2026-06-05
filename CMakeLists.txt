cmake_minimum_required(VERSION 3.20)
project(Cascade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the system-installed raylib (provides the raylib::raylib / raylib target)
find_package(raylib REQUIRED)

add_executable(Cascade
        main.cpp)

# Link raylib (pulls in its transitive deps: GL, X11, pthread, m, dl, etc.)
target_link_libraries(Cascade raylib)
