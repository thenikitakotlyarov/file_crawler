#pragma once


#include <cstdint>

using namespace std;

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;

    bool operator==(const Color &other) const {
        return (red == other.red && green == other.green && blue == other.blue);
    }
};


namespace std {
    template<>
    struct hash<Color> {
        size_t operator()(const Color &color) const {
            size_t h1 = hash<int>{}(color.red);
            size_t h2 = hash<int>{}(color.green);
            size_t h3 = hash<int>{}(color.blue);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
