#pragma once

#include "helpers.h"
#include "constants.h"

using namespace std;


class QuestSystem {
public:
    QuestSystem();

    ~QuestSystem();

    int running;

    void CleanUp();

    void Update();


private:
    bool Initialize();

};