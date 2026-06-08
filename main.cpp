#include <vector>

#include "raylib.h"
#include "rlImGui.h"

#include "cell.h"
#include "config.h"
#include "simulation.h"
#include "renderer.h"
#include "ui.h"

int main()
{
    InitWindow(winWidth, winHeight, "Cascade | Interactive Cell Simulator");
    SetTargetFPS(simFrameRate);

    rlImGuiSetup(true);

    // Initialize grid
    std::vector<Cell> grid(gridWidth * gridHeight);

    // Game loop
    while (!WindowShouldClose())
    {
        if (!paused || stepOnce) {
            UpdateCells(grid);
            stepOnce = false;
        }
        HandleInput(grid);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCells(grid);

        DrawUI(grid);

        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();
    return 0;
}