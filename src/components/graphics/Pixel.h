#pragma once

#include <string>
#include "Color.h"

using namespace std;


struct Pixel {
    wstring ch;
    Color fg_color;
    Color bg_color;

    bool operator==(const Pixel &other) const {
        return (ch == other.ch && fg_color == other.fg_color && bg_color == other.bg_color);
    }

    bool operator!=(const Pixel &other) const {
        return (ch != other.ch || fg_color != other.fg_color || bg_color != other.bg_color);
    }

};