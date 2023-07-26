#pragma once

#include <string>
#include <vector>
#include "components/entity/actors/player/Player.h"
#include "components/map/Position.h"
#include "components/map/GameMap.h"
#include "components/action/Intent.h"
#include "components/graphics/Sprite.h"
#include "helpers.h"


using namespace std;


struct Player;
struct GameMap;

struct Monster {
    wstring name;
    wstring character;
    Color color;
    Sprite sprite;

    int rarity;

    bool transient;

//    int vitality;
//    int power;
//    int agility;
//    int focus;
//    int insight;
//    int belief;

    int attackPower;
    int attackRadius;
    int chaseRadius;
    int cooldown;


    int total_health;
    int health;


    function<Intent(Monster &, Entity &, Position &, const set<pair<int, int>>,
                    Player &, Entity &, Position &,
                    GameMap *)> Update;


};