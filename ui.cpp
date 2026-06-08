//
// Created by wizdom on 2026-06-08.
//

#include "ui.h"

void DrawUI(std::vector<Cell> &grid) {
    rlImGuiBegin();
    ImGui::Begin("Controls");

    ImGui::Text("FPS (lim. %d): %d", simFrameRate, GetFPS());
    if (ImGui::SliderInt("Simulation speed", &simFrameRate, 1, 250))
        SetTargetFPS(simFrameRate);
    ImGui::Text("Grid size: %d", static_cast<int>(grid.size()));
    ImGui::SliderInt("Brush", &brushRadius, 0, brushLimit);

    ImGui::Separator();
    ImGui::Checkbox("Pause Simulation", &paused);
    ImGui::BeginDisabled(!paused);
    if (ImGui::Button("Step") && paused)
        stepOnce = true;
    ImGui::EndDisabled();

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
