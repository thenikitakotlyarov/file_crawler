
#include "Quest.h"


QuestSystem::QuestSystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

QuestSystem::~QuestSystem() {
    CleanUp();
}

bool QuestSystem::Initialize() {
    // Initialize systems
    return true;
}


void QuestSystem::CleanUp() {
    // Clean up systems
}

void QuestSystem::Update() {
    // Update systems
}



