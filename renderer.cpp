//
// Created by wizdom on 2026-06-08.
//

#include "renderer.h"

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
                const int xPos = static_cast<int>(i) % gridWidth;
                const int yPos = static_cast<int>(i) / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, YELLOW);

                break;
            }
            case Cell::WATER:
            {
                const int xPos = static_cast<int>(i) % gridWidth;
                const int yPos = static_cast<int>(i) / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, DARKBLUE);
                break;
            }
            case Cell::STONE:
            {
                const int xPos = static_cast<int>(i) % gridWidth;
                const int yPos = static_cast<int>(i) / gridWidth;

                DrawRectangle(xPos * scale, yPos * scale, scale, scale, DARKGRAY);
                break;
            }
        }
    }
}
