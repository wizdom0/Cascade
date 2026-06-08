cmake_minimum_required(VERSION 3.20)
project(Cascade)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the system-installed raylib (provides the raylib::raylib / raylib target)
find_package(raylib REQUIRED)

# --- Dear ImGui (system pkg) + rlImGui (fetched bridge) ----------------------
# Use the system ImGui package (provides imgui::imgui + headers in /usr/include).
find_package(imgui REQUIRED)

# rlImGui has no usable CMake target, so fetch its source and compile the one
# bridge .cpp into a small static lib against the system ImGui + raylib.
include(FetchContent)
FetchContent_Declare(
        rlimgui
        GIT_REPOSITORY https://github.com/raylib-extras/rlImGui.git
        GIT_TAG        main
)
FetchContent_MakeAvailable(rlimgui)

add_library(rlimgui STATIC
        ${rlimgui_SOURCE_DIR}/rlImGui.cpp
)
target_include_directories(rlimgui PUBLIC
        ${rlimgui_SOURCE_DIR}
)
target_link_libraries(rlimgui PUBLIC raylib imgui::imgui)
# -----------------------------------------------------------------------------

add_executable(Cascade
        main.cpp)

# raylib (+ transitive GL/X11/pthread/...) and rlimgui (carries ImGui + the
# rlImGui header transitively).
target_link_libraries(Cascade raylib rlimgui)
