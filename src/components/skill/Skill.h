#pragma once

#include "helpers.h"
#include "constants.h"


#include "components/Entity.h"
#include "components/Intent.h"



using namespace std;

struct Player;
struct Monster;



struct Skill {
    unsigned long id;
    string name;

    vector<vector<wstring>> icon;

    function<pair<int,vector<Intent>>(Player&, Position &player_pos, const set<pair<int,int>> player_fov, map<Entity, Monster>& monsters, map<Entity,Position> positions)> Use;

};