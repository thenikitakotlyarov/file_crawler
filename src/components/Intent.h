#pragma once

#include "components/Entity.h"
#include <utility>

using namespace std;

enum class IntentType {
    Move,
    Attack
};

struct Intent {
    Entity entity;
    IntentType type;
    //only used for movement
    pair<int, int> direction;
    Entity target; // Only used for attack
    int damage; // Only used for attack
};
