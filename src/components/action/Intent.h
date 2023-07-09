#pragma once

#include "components/entity/Entity.h"
#include <utility>

using namespace std;

enum class IntentType {
    Move,
    Attack
};

struct Intent {
    Entity entity;
    IntentType type;
    pair<int, int> direction;//only used for movement
    Entity target; // Only used for attack
    int damage; // Only used for attack
};
