#include "raylib.h"
#include <cstdint>
#include <iostream>
#include <vector>

unsigned int winWidth = 1280;
unsigned int winHeight = 720;

unsigned int simFrameRate = 120;

unsigned int scale = 10;
unsigned int gridWidth = winWidth / scale;
unsigned int gridHeight = winHeight / scale;

enum class Cell : uint8_t { Empty, Sand };

void UpdateCells(std::vector<Cell> &grid) {
    for (int i = grid.size() - 1; i >= 0; i--) {
        switch (grid[i]) {
        case Cell::Sand: {
            int x = i % gridWidth;
            if (i + (int)gridWidth >= (int)grid.size())
                break;
            if (grid[i + gridWidth] == Cell::Empty) {
                std::swap(grid[i], grid[i + gridWidth]); // Below
            } else if (x > 0 && grid[i + gridWidth - 1] == Cell::Empty) {
                std::swap(grid[i], grid[i + gridWidth - 1]); // Below left
            } else if (x < (int)gridWidth - 1 && grid[i + gridWidth + 1] == Cell::Empty) {
                std::swap(grid[i], grid[i + gridWidth + 1]); // Below Right
            }
            break;
        }
        case Cell::Empty:
            break;
        }
    }
}

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

void HandleInput(std::vector<Cell> &grid) {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        Vector2 mousePos = GetMousePosition();
        int mouseGridPosX = (int)mousePos.x / scale;
        int mouseGridPosY = (int)mousePos.y / scale;

        if (mouseGridPosX >= 0 && mouseGridPosX < (int)gridWidth && mouseGridPosY >= 0 &&
            mouseGridPosY < (int)gridHeight) {
            // Reverse formula to get index based on position
            int index = mouseGridPosY * gridWidth + mouseGridPosX;

            if (grid[index] == Cell::Empty) {
                grid[index] = Cell::Sand;
            }
        }
    }
}

int main() {
    InitWindow(winWidth, winHeight, "Cascade | Interactive Cell Simulator");
    SetTargetFPS(simFrameRate);

    // Initialize grid
    std::vector<Cell> grid(gridWidth * gridHeight);

    // Game loop
    while (!WindowShouldClose()) {
        UpdateCells(grid);
        HandleInput(grid);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCells(grid);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
