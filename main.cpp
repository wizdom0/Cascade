#include "raylib.h"
#include <cstdint>
#include <iostream>
#include <vector>

unsigned int winWidth = 1280;
unsigned int winHeight = 720;

unsigned int simFrameRate = 5;

unsigned int scale = 10;
unsigned int gridWidth = winWidth / 10;
unsigned int gridHeight = winHeight / 10;

enum class Cell : uint8_t { Empty, Sand };

void DrawCells(const std::vector<Cell> &grid) {
    for (size_t i = 0; i < grid.size(); i++) {
        switch (grid[i]) {
        case Cell::Sand: {
            unsigned int xPos = i % gridWidth;
            unsigned int yPos = i / gridWidth;

            DrawRectangle(xPos * scale, yPos * scale, scale, scale, YELLOW);
            break;
        }
        case Cell::Empty:
            break;
        }
    }
}

int main() {
    InitWindow(winWidth, winHeight, "Cascade | Interactive Cell Simulator");
    SetTargetFPS(simFrameRate);

    std::vector<Cell> grid(gridWidth * gridHeight);
    grid[(gridHeight / 2) * gridWidth + (gridWidth / 2)] = Cell::Sand;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCells(grid);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
