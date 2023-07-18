#pragma once


#include <cstdint>
#include <cmath>

using namespace std;

#ifndef COLOR_H
struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};
#endif

struct RawColor {
    double red;
    double green;
    double blue;


    bool operator>(const RawColor &other) const {
        if ((red - other.red + green - other.green + blue - other.blue) > 0) return true;
        else return false;
    }

    bool operator>=(const RawColor &other) const {
        if ((red - other.red + green - other.green + blue - other.blue) >= 0) return true;
        else return false;
    }

    bool operator<(const RawColor &other) const {
        if ((red - other.red + green - other.green + blue - other.blue) < 0) return true;
        else return false;
    }

    bool operator<=(const RawColor &other) const {
        if ((red - other.red + green - other.green + blue - other.blue) <= 0) return true;
        else return false;
    }


    bool operator==(const RawColor &other) const {
        return (red == other.red && green == other.green && blue == other.blue);
    }

    bool operator!=(const RawColor &other) const {
        return (red != other.red || green != other.green || blue != other.blue);
    }

    RawColor &operator+=(const RawColor &other) {
        red += other.red;
        green += other.green;
        blue += other.blue;
        return *this;
    }


    RawColor &operator-=(const RawColor &other) {
        red -= other.red;
        green -= other.green;
        blue -= other.blue;
        return *this;
    }

    RawColor &operator*=(const RawColor &other) {
        red *=  other.red;
        green *= other.green;
        blue *= other.blue;
        return *this;
    }

    RawColor &operator/=(const RawColor &other) {
        red /=  other.red;
        green /= other.green;
        blue /= other.blue;
        return *this;
    }

    RawColor &operator&=(const RawColor &other) {
        red = (red + other.red) / 2;
        green = (green + other.green) / 2;
        blue = (blue + other.blue) / 2;
        return *this;
    }

    Color toColor() {
        Color color = {
                (uint8_t) max(0, min(255, (int) (255 * pow(red, 0.5)))),
                (uint8_t) max(0, min(255, (int) (255 * pow(green, 0.5)))),
                (uint8_t) max(0, min(255, (int) (255 * pow(blue, 0.5)))),
        };
        return color;
    }

    RawColor fromColor(const Color &color) {
        RawColor rawColor = {
                pow(((double) color.red) / 255.0, 2.0),
                pow(((double) color.green) / 255.0, 2.0),
                pow(((double) color.blue) / 255.0, 2.0)
        };
        return rawColor;
    }

};


namespace std {
    template<>
    struct hash<RawColor> {
        size_t operator()(const RawColor &color) const {
            size_t h1 = hash<int>{}(color.red);
            size_t h2 = hash<int>{}(color.green);
            size_t h3 = hash<int>{}(color.blue);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
