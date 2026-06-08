//
// Created by wizdom on 2026-06-08.
//

#pragma once
#include "cell.h"

constexpr unsigned int winWidth = 1280;
constexpr unsigned int winHeight = 720;
constexpr int scale = 10;
constexpr int gridWidth = winWidth / scale;
constexpr int gridHeight = winHeight / scale;

inline int simFrameRate = 60;
inline bool paused = false;
inline bool stepOnce = false;

inline auto currentMaterial = Cell::SAND;
constexpr int brushLimit = 50;
inline int brushRadius = 1;
