#pragma once

struct Position{
    int x;
    int y;


    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }

};


struct PositionHash {
    std::size_t operator() (const Position& position) const {
        // Use bit shifting and a prime number to attempt to evenly distribute hash values
        return ((std::hash<int>()(position.x)) << 1) ^ std::hash<int>()(position.y);
    }
};