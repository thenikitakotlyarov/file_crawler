#pragma once

#include "helpers.h"
#include "constants.h"

#include "components/Entity.h"
#include "components/Intent.h"
#include "components/enemy/Monster.h"
#include "components/skill/Skill.h"


using namespace std;

struct Monster;

struct Player {
    string name;
    string class_name;
    int color;

    short level;
    long exp;

    int max_health;
    int max_energy;
    int max_stamina;

    int current_health;
    int current_energy;
    int current_stamina;


    int constitution;
    int strength;
    int dexterity;
    int willpower;
    int intelligence;
    int faith;


    Skill* primarySkill;


};



