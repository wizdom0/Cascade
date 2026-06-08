//
// Created by wizdom on 2026-06-08.
//

#include "simulation.h"

void UpdateCells(std::vector<Cell> &grid)
{
    std::vector<bool> moved(grid.size(), false);
    for (int i = static_cast<int>(grid.size()) - 1; i >= 0; i--)
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
                else if (barrierX < static_cast<int>(gridWidth) - 1 && hasBelow &&
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

void HandleInput(std::vector<Cell> &grid) {
    if (ImGui::GetIO().WantCaptureMouse) return; // If user is using GUI, don't detect input.
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        auto [x, y] = GetMousePosition();
        const int mouseGridPosX = static_cast<int>(x) / scale;
        const int mouseGridPosY = static_cast<int>(y) / scale;

        if (mouseGridPosX >= 0 && mouseGridPosX < static_cast<int>(gridWidth) && mouseGridPosY >= 0 &&
            mouseGridPosY < static_cast<int>(gridHeight))
        {
            for (int dy = -brushRadius; dy <= brushRadius; dy++) { // For every row...
                for (int dx = -brushRadius; dx <= brushRadius; dx++) { // check every column.
                    const int cellX = mouseGridPosX + dx;
                    const int cellY = mouseGridPosY + dy;
                    if (cellX < 0 || cellX >= gridWidth || cellY < 0 || cellY >= gridHeight)
                        continue;

                    // Reverse formula to get index based on position
                    if (const unsigned int index = cellY * gridWidth + cellX; grid[index] == Cell::EMPTY || currentMaterial == Cell::EMPTY)
                    {
                        grid[index] = currentMaterial;
                    }
                }
            }
        }
    }

    if (const float wheel = GetMouseWheelMove(); wheel != 0)
        brushRadius = std::clamp(brushRadius + static_cast<int>(wheel), 0, brushLimit);

    if (IsKeyPressed(KEY_ZERO)) {
        currentMaterial = Cell::EMPTY;
    }
    if (IsKeyPressed(KEY_ONE)) {
        currentMaterial = Cell::SAND;
    }
    if (IsKeyPressed(KEY_TWO)) {
        currentMaterial = Cell::WATER;
    }
    if (IsKeyPressed(KEY_THREE)) {
        currentMaterial = Cell::STONE;
    }
}