#pragma once

using namespace std;

struct Light {
    Color color;
    //this ^ is the color the light would be if sampled directly

    char temp;
    //this ^ is the temperature that dictates the falloff of the light
    // can be `n`: neutral, `w`: warm, or `c`: cold

    unsigned short radius;
    // this ^ is the radius that dictates the max size of the light

    double intensity;
    // this ^ is the mix rate of the sample from the light and what's being lit
    // intensity ranges from 0.0 (no mix) to 1.0 (replace)

    double flicker;
    // this ^  is the current flicker offset for the color/intensity when drawn
    // flicker ranges from 0.0 (completely visible) to 1.0 (completely out)

    double falloff;
    // this ^  is the rate at which the light falls off
    // falloff ranges from 0.0 (no falloff)

};