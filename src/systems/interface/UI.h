#pragma once

#include "components/graphics/Frame.h"
#include "components/entity/actors/player/Player.h"
#include "systems/entity/Entity.h"
#include "helpers.h"
#include "constants.h"

using namespace std;

class UISystem {
public:
    UISystem();

    ~UISystem();

    int running;
    int menu_position = 0;

    void CleanUp();

    void Update();


    static Frame BlankFrame(const int y, const int x, unsigned long frame_count);


    static Frame &
    addBar(Frame &frame, const int y, const int x, const int length, const double level,
           const wstring &icon, Color fg_color, Color bg_color);

    static Frame &
    addButton(Frame &frame, const int y, const int x, const int height, const int width,
              Color fg_color, Color bg_color);

    static Frame &
    addText(Frame &frame, const int y, const int x, wstring text,
            Color fg_color, Color bg_color);

    Frame addTag(Frame frame, const int x, const int y, const unsigned short raster_scale, const wstring &tag) const;


    Frame getFpsLabel(Frame &frame, const int y, const int x,
                      const int c_fps);

    Frame getTitleCard(Frame frame);

    Frame getLoadCard(Frame frame);

    Frame getGameOverCard(Frame frame);

    Frame getMainMenu(Frame frame);

    Frame getNewGameMenu(Frame frame);

    Frame getLoadGameMenu(Frame frame);

    Frame getSettingsMenu(Frame frame);

    static Frame getColorDebug(Frame frame);


    Frame getHud(Frame frame, const Player &player, const int c_fps);

    Frame getTags(Frame frame, EntitySystem &entity_system,const set<pair<int, int>> view_fov,
                  const int start_x, const int start_y, const unsigned short raster_scale);

    Frame overlayPause(Frame frame, const vector<pair<wstring, bool &>> menu);

private:
    bool Initialize();


    vector<vector<wstring>> EMPTY_ICON = {};


    static Frame &
    getUiBg(Frame &frame, const int dock_height,
            Color fg_color, Color bg_color);

    static Frame &
    getPlayerTag(Frame &frame, const int y, const int x,
                 const wstring &player_name, const wstring &player_class,
                 Color fg_color, Color bg_color);

    static Frame &
    getOrb(Frame &frame, const int y, const int x, const int diameter,
           const int level, Color fg_color, Color bg_color);

    static Frame &
    getSlot(Frame &frame, const int y, const int x, const int height, const int width,
            const wstring hotkey, const wstring label, vector<vector<wstring>> icon, Color fg_color, Color mg_color,
            Color bg_color);

    Frame &
    getMenuButton(Frame &frame, const int y, const int x, const int height, const int width,
                  vector<vector<wstring>> icon, bool active);

    Frame &
    getPotionBar(Frame &frame, const int y, const int x, const int slot_height, const int slot_width,
                 Color fg_color, Color bg_color);
};
