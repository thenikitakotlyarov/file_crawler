#pragma once

#include "helpers.h"
#include "constants.h"

#include "components/entity/Entity.h"
#include "components/action/Intent.h"
#include "components/entity/actors/enemy/Monster.h"
#include "components/action/skill/Skill.h"


using namespace std;

struct Monster;

struct Player {
    string name;
    string class_name;
    Color color;

    Sprite sprite;


    short level;
    long exp;

    int max_health;
    int max_energy;
    int max_stamina;

    int current_health;
    int current_energy;
    int current_stamina;


    int vitality;
    int power;
    int agility;
    int focus;
    int insight;
    int belief;


    Skill* primarySkill;
    Skill* secondarySkill;


};



