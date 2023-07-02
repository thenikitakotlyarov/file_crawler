#pragma once

#include <string>
#include <vector>
#include "components/player/Player.h"
#include "components/Position.h"
#include "components/map/GameMap.h"
#include "components/Intent.h"
#include "helpers.h"


using namespace std;


struct Player;

struct Monster {
    string name;
    wstring characterFullHealth;
    wstring characterMidHealth;
    wstring characterLowHealth;
    int color;

    int rarity;

    bool transient;

    int attackPower;
    int attackRadius;
    int chaseRadius;
    int cooldown;


    int total_health;
    int health;


    function<Intent(Monster &, Entity &, Position &, const set<pair<int, int>>,
                    Player &, Entity &, Position &,
                    GameMap &)> Update;


};