//
// Created by wizdom on 2026-06-08.
//

#include "cell.h"

const char* getStringFromCell(const Cell c) {
    switch (c) {
        case Cell::EMPTY: {
            return "Erase";
        }
        case Cell::SAND: {
            return "Sand";
        }
        case Cell::WATER: {
            return "Water";
        }
        case Cell::STONE: {
            return "Stone";
        }
    }
    return "???";
}