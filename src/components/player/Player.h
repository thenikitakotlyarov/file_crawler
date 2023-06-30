#pragma once

#include "helpers.h"
#include "constants.h"

using namespace std;

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
    int endurance;
    int dexterity;
    int intelligence;
    int willpower;


};