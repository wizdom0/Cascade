//
// Created by wizdom on 2026-06-08.
//

#pragma once

#include <vector>
#include <algorithm>

#include "rlImGui.h"
#include "imgui.h"

#include "cell.h"
#include "config.h"

void UpdateCells(std::vector<Cell> &grid);
void HandleInput(std::vector<Cell> &grid);