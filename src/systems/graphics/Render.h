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


    Frame upRes(const Frame &frame, const unsigned short n);

    void render(const Frame &frame);


private:
    bool Initialize();

    unordered_map<wstring, vector<vector<wstring>>> upres_map = {
            {L"@",
             {
                     {L" ", L" ", L"@", L" ", L" "},
                     {L" ", L"/", L"W", L"\\", L" "},
                     {L"|", L" ", L"B", L" ", L"|"},
                     {L" ", L"|", L"x", L"|", L" "},
                     {L" ", L"|", L" ", L"|", L" "}}},
            {L"?",
             {
                     {L"?", L"?", L"?", L"?", L"?"},
                     {L"?", L"?", L"?", L"?", L"?"},
                     {L"?", L"?", L"?", L"?", L"?"},
                     {L"?", L"?", L"?", L"?", L"?"},
                     {L"?", L"?", L"?", L"?", L"?"}}},
            {L" ",
             {
                     {L" ", L" ", L" ", L" ", L" "},
                     {L" ", L" ", L" ", L" ", L" "},
                     {L" ", L" ", L" ", L" ", L" "},
                     {L" ", L" ", L" ", L" ", L" "},
                     {L" ", L" ", L" ", L" ", L" "}}},
            {L"░",
             {
                     {L" ", L"░", L" ", L"░", L" "},
                     {L"░", L" ", L"░", L" ", L"░"},
                     {L" ", L"░", L"░", L"░", L" "},
                     {L"░", L" ", L"░", L" ", L"░"},
                     {L" ", L"░", L" ", L"░", L" "}}},
            {L"▒",
             {
                     {L"▒", L" ", L"▒", L" ", L"▒"},
                     {L" ", L"▒", L"▒", L"▒", L" "},
                     {L"▒", L"▒", L"▒", L"▒", L"▒"},
                     {L" ", L"▒", L"▒", L"▒", L" "},
                     {L"▒", L" ", L"▒", L" ", L"▒"}}},
            {L"▓",
             {
                     {L"▓", L"▓", L"▓", L"▓", L"▓"},
                     {L"▓", L"▓", L"▓", L"▓", L"▓"},
                     {L"▓", L"▓", L" ", L"▓", L"▓"},
                     {L"▓", L"▓", L"▓", L"▓", L"▓"},
                     {L"▓", L"▓", L"▓", L"▓", L"▓"}}}
    };

};