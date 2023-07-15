#pragma once

using namespace std;


struct Quest {
    unsigned long id;
    string name;

    unordered_set<unsigned long> prereqs;
    

};