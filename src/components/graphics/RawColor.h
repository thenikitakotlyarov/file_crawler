#pragma once


#include <cstdint>

using namespace std;

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
        red = (double) max(0, min(255, (int) red - (int) other.red));
        green = (double) max(0, min(255, (int) green - (int) other.green));
        blue = (double) max(0, min(255, (int) blue - (int) other.blue));
        return *this;
    }

    RawColor &operator*=(const RawColor &other) {
        red = (double) max(0, min(255, (int) ((double) red * (double) other.red / 256)));
        green = (double) max(0, min(255, (int) ((double) green * (double) other.green / 256)));
        blue = (double) max(0, min(255, (int) ((double) blue * (double) other.blue / 256)));
        return *this;
    }

    RawColor &operator/=(const RawColor &other) {
        red = (double) max(0, min(255, (int) ((double) red / (double) other.red / 256)));
        green = (double) max(0, min(255, (int) ((double) green / (double) other.green / 256)));
        blue = (double) max(0, min(255, (int) ((double) blue / (double) other.blue / 256)));
        return *this;
    }

    RawColor &operator&=(const RawColor &other) {
        red = (double) max(0, min(255, (int) (((double) red + (double) other.red) / 2)));
        green = (double) max(0, min(255, (int) (((double) green + (double) other.green) / 2)));
        blue = (double) max(0, min(255, (int) (((double) blue + (double) other.blue) / 2)));
        return *this;
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
