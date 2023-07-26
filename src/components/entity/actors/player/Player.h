#pragma once

#include "helpers.h"
#include "constants.h"

#include "components/entity/Entity.h"
#include "components/action/Intent.h"
#include "components/entity/actors/enemy/Monster.h"
#include "components/entity/item/Container.h"
#include "components/action/skill/Skill.h"


using namespace std;

struct Monster;
struct Container;

struct Player {
    wstring name;
    wstring class_name;
    Color color;

    Sprite sprite;


    short level;

    pair<int, int> health;
    pair<int, int> stamina;
    pair<int, int> energy;

    int vitality;
    int power;
    int agility;
    int focus;
    int insight;
    int belief;

    double luck;

    Skill *primarySkill;
    Skill *secondarySkill;

    Container potion_belt;


};



