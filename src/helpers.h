#pragma once

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libnoise/noise.h>
#include <ncurses.h>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include <functional>
#include <cstdio>
#include <memory>
#include <queue>
#include <map>
#include <set>
#include <utility>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <clocale>

#include "components/Frame.h"
#include "components/Position.h"
#include "components/map/GameMap.h"
#include "components/Tile.h"
#include "constants.h"

using namespace std;

struct GameMap;

wstring int_to_hex_str(int i);

int get_random_int(int min, int max);

wstring get_random_character(const vector<wstring> &char_array);

bool check_if_in(const vector<wstring> &char_array, const wstring& compare);

template<typename T, class Container>
bool is_in_set(const T &element, const Container &set);

bool is_position_in_fov(const Position position, const set<pair<int,int>> &fov);


int get_color_pair_index(int fg_idx, int bg_idx);

void init_all_color_pairs();


vector<int> get_ground_swatch();

vector<int> get_trap_swatch();

vector<int> get_wall_swatch();

//pathfinding begin

bool isAdjacent(const Position& pos1, const Position& pos2);


struct Node {
    int x, y;
    double g, h;
    Node *parent;

    Node(int x, int y, Node *parent = nullptr, double g = 0, double h = 0)
            : x(x), y(y), parent(parent), g(g), h(h) {}

    double f() const {
        return g + h;
    }
};

class NodePriorityQueue {
public:
    bool operator()(const Node *lhs, const Node *rhs) const {
        return lhs->f() > rhs->f();
    }
};



vector<Node> aStar(const Position &start, const Position &goal,
                   const GameMap &game_map);

//pathfinding end



int get_input();

void render(const Frame &frame);

