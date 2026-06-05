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

enum class Cell : uint8_t
{
    EMPTY,
    SAND,
    WATER,
    STONE
};

Cell currentMaterial = Cell::SAND;

void UpdateCells(std::vector<Cell> &grid)
{
    std::vector<bool> moved(grid.size(), false);
    for (int i = grid.size() - 1; i >= 0; i--)
    {
        if (moved[i])
            continue;
        switch (grid[i])
        {
            case Cell::EMPTY: {
                if (grid[i] != Cell::EMPTY)
                    grid[i] = Cell::EMPTY;
                break;
            }
            case Cell::SAND:
            {
                int barrierX = i % gridWidth;
                if (i + static_cast<int>(gridWidth) >= static_cast<int>(grid.size()))
                    break;
                if (!moved[i + gridWidth] &&
                    (grid[i + gridWidth] == Cell::EMPTY || grid[i + gridWidth] == Cell::WATER))
                {
                    std::swap(grid[i], grid[i + gridWidth]); // Below
                    moved[i + gridWidth] = true;
                    if (grid[i] != Cell::EMPTY)
                        moved[i] = true;
                }
                else if (barrierX > 0 && !moved[i + gridWidth - 1] &&
                         (grid[i + gridWidth - 1] == Cell::EMPTY ||
                          grid[i + gridWidth - 1] == Cell::WATER))
                {
                    std::swap(grid[i], grid[i + gridWidth - 1]); // Below left
                    moved[i + gridWidth - 1] = true;
                    if (grid[i] != Cell::EMPTY)
                        moved[i] = true;
                }
                else if (barrierX < static_cast<int>(gridWidth) - 1 && !moved[i + gridWidth + 1] &&
                         (grid[i + gridWidth + 1] == Cell::EMPTY ||
                          grid[i + gridWidth + 1] == Cell::WATER))
                {
                    std::swap(grid[i], grid[i + gridWidth + 1]); // Below right
                    moved[i + gridWidth + 1] = true;
                    if (grid[i] != Cell::EMPTY)
                        moved[i] = true;
                }
                break;
            }
            case Cell::WATER:
            {
                bool hasBelow = i + static_cast<int>(gridWidth) < static_cast<int>(grid.size());
                int barrierX = i % gridWidth;
                if (hasBelow && grid[i + gridWidth] == Cell::EMPTY)
                {
                    std::swap(grid[i], grid[i + gridWidth]); // Below
                    moved[i + gridWidth] = true;
                }
                else if (barrierX > 0 && hasBelow && grid[i + gridWidth - 1] == Cell::EMPTY)
                {
                    std::swap(grid[i], grid[i + gridWidth - 1]); // Below left
                    moved[i + gridWidth - 1] = true;
                }
                else if (barrierX < (int)gridWidth - 1 && hasBelow &&
                         grid[i + gridWidth + 1] == Cell::EMPTY)
                {
                    std::swap(grid[i], grid[i + gridWidth + 1]); // Below right
                    moved[i + gridWidth + 1] = true;
                }
                else if (barrierX > 0 && grid[i - 1] == Cell::EMPTY)
                {
                    std::swap(grid[i], grid[i - 1]); // Left
                    moved[i - 1] = true;
                }
                else if (barrierX < static_cast<int>(gridWidth) - 1 && grid[i + 1] == Cell::EMPTY)
                {
                    std::swap(grid[i], grid[i + 1]); // Right
                    moved[i + 1] = true;
                }
                break;
            }
            case Cell::STONE:
                break;
        }
    }
}

void DrawCells(const std::vector<Cell> &grid)
{
    for (size_t i = 0; i < grid.size(); i++)
    {
        switch (grid[i])
        {
            case Cell::EMPTY:
                break;
            case Cell::SAND:
            {
                const unsigned int xPos = i % gridWidth;
                const unsigned int yPos = i / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, YELLOW);
                break;
            }
            case Cell::WATER:
            {
                const unsigned int xPos = i % gridWidth;
                const unsigned int yPos = i / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, DARKBLUE);
                break;
            }
            case Cell::STONE:
            {
                const unsigned int xPos = i % gridWidth;
                const unsigned int yPos = i / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, DARKGRAY);
                break;
            }
        }
    }
}

void HandleInput(std::vector<Cell> &grid)
{
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        Vector2 mousePos = GetMousePosition();
        int mouseGridPosX = (int)mousePos.x / scale;
        int mouseGridPosY = (int)mousePos.y / scale;

        if (mouseGridPosX >= 0 && mouseGridPosX < (int)gridWidth && mouseGridPosY >= 0 &&
            mouseGridPosY < (int)gridHeight)
        {
            // Reverse formula to get index based on position
            int index = mouseGridPosY * gridWidth + mouseGridPosX;

            if (grid[index] == Cell::EMPTY || currentMaterial == Cell::EMPTY)
            {
                grid[index] = currentMaterial;
            }
        }
    }

    if (IsKeyPressed(KEY_ZERO))
        currentMaterial = Cell::EMPTY;
    if (IsKeyPressed(KEY_ONE))
        currentMaterial = Cell::SAND;
    if (IsKeyPressed(KEY_TWO))
        currentMaterial = Cell::WATER;
    if (IsKeyPressed(KEY_THREE))
        currentMaterial = Cell::STONE;
}

int main()
{
    InitWindow(winWidth, winHeight, "Cascade | Interactive Cell Simulator");
    SetTargetFPS(simFrameRate);

    // Initialize grid
    std::vector<Cell> grid(gridWidth * gridHeight);

    // Game loop
    while (!WindowShouldClose())
    {
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
