#include "raylib.h"
#include <cstdint>
#include <iostream>
#include <vector>
#include <algorithm>
#include "rlImGui.h"
#include "imgui.h"


int winWidth = 1280;
int winHeight = 720;

int simFrameRate = 60;
bool paused = false;
bool stepOnce = false;

int scale = 4;
int gridWidth = winWidth / scale;
int gridHeight = winHeight / scale;

enum class Cell : uint8_t
{
    EMPTY,
    SAND,
    WATER,
    STONE
};

auto currentMaterial = Cell::SAND;
int brushLimit = 50;
int brushRadius = 6;

char* getStringFromCell(const Cell c) {
    switch (c) {
        case Cell::EMPTY: {
            return "Erase";
            break;
        }
        case Cell::SAND: {
            return "Sand";
            break;
        }
        case Cell::WATER: {
            return "Water";
            break;
        }
        case Cell::STONE: {
            return "Stone";
            break;
        }
    }
    return "???";
}

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
                        int cellX = mouseGridPosX + dx, cellY = mouseGridPosY + dy;
                        if (cellX < 0 || cellX >= gridWidth || cellY < 0 || cellY >= gridHeight)
                            continue;

                        // Reverse formula to get index based on position
                        const unsigned int index = cellY * gridWidth + cellX;
                        if (grid[index] == Cell::EMPTY || currentMaterial == Cell::EMPTY)
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

void DrawUI(std::vector<Cell> &grid) {
    rlImGuiBegin();
    ImGui::Begin("Controls");

    ImGui::Text("FPS (lim. %d): %d", simFrameRate, GetFPS());
    ImGui::Text("Grid size: %d", static_cast<int>(grid.size()));
    ImGui::SliderInt("Brush", &brushRadius, 0, brushLimit);

    ImGui::Separator();
    ImGui::Checkbox("Pause Simulation", &paused);
    if (ImGui::Button("Step")) {
        if (paused) {
            stepOnce = true;
        }
    }

    ImGui::Separator();
    ImGui::Text("Current material: %s", getStringFromCell(currentMaterial));
    ImGui::Text("Select material:");
    if (ImGui::Button("Erase"))
        currentMaterial = Cell::EMPTY;
    ImGui::SameLine();
    if (ImGui::Button("Sand"))
        currentMaterial = Cell::SAND;
    ImGui::SameLine();
    if (ImGui::Button("Water"))
        currentMaterial = Cell::WATER;
    ImGui::SameLine();
    if (ImGui::Button("Stone"))
        currentMaterial = Cell::STONE;

    if (ImGui::Button("Clear"))
        std::fill(grid.begin(), grid.end(), Cell::EMPTY);

    ImGui::End();
    rlImGuiEnd();
}

int main()
{
    InitWindow(winWidth, winHeight, "Cascade | Interactive Cell Simulator");
    SetTargetFPS(simFrameRate);
    std::cout << "Simulation started. Target FPS: " << simFrameRate << '\n';

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
