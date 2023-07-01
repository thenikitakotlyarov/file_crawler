#pragma once

#include <string>
#include <vector>
#include "components/player/Player.h"
#include "components/Position.h"
#include "components/GameMap.h"
#include "helpers.h"


using namespace std;


struct Monster{
    string name;
    wstring characterFullHealth;
    wstring characterMidHealth;
    wstring characterLowHealth;
    int color;

    int rarity;

    int attackPower;
    int attackRadius;
    int chaseRadius;
    int cooldown;
    int total_health;
    int health;
    function<Position(Monster&,Position&, Player&, Position&, GameMap&)> Update;


};