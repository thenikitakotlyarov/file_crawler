#pragma once

#include "helpers.h"
#include "constants.h"

using namespace std;


class RenderSystem {
public:
    RenderSystem();

    ~RenderSystem();

    int running;

    void CleanUp();

    void Update();


    void render(const Frame &frame);


private:
    bool Initialize();

};