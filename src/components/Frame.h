#pragma once

#include <string>
#include <vector>

using namespace std;

struct Frame{
    string meta;
    vector<vector<pair<wstring, pair<int, int>>>> data;
};