//
// Created by wizdom on 2026-06-08.
//

#pragma once
#include <cstdint>

enum class Cell : uint8_t {
    EMPTY,
    SAND,
    WATER,
    STONE
};

const char* getStringFromCell(Cell c);