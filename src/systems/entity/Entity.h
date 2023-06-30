#pragma once


#include "components/Frame.h"
#include "components/GameMap.h"
#include "components/Position.h"
#include "components/player/Player.h"
#include "components/enemy/Monster.h"
#include "components/item/Item.h"

#include "templates/item/BaseItems.h"
#include "templates/enemy/BaseEnemies.h"

#include "constants.h"
#include "helpers.h"

using namespace std;


struct Entity {
    unsigned long id;
};

class EntitySystem {
public:
    EntitySystem();

    ~EntitySystem();

    int running;
    vector<vector<int>> Positions;

    void CleanUp();

    void Update();

    Entity createEntity();

    void destroyEntity(Entity entity_id);

    vector<Entity> getEntities();

    unordered_map<unsigned long, Position> getPositions();

    void setPlayer(Player player);

    Player getPlayer();

    void setGamemap(GameMap game_map);


    void spawnPlayer();

    void spawnMonsters();

    void spawnItems();

    unordered_map<unsigned long, Monster> getMonsters();

    unordered_map<unsigned long, Item> getItems();

    void movePlayer(pair<int,int> direction);

    set<pair<int, int>> calculate_fov(
            GameMap& game_map,
            int center_x, int center_y,
            int radius);

    Frame renderEntities2D(Frame frame, GameMap game_map,
                           int start_y, int start_x,
                           int end_y, int end_x);


private:
    bool Initialize();

    Player currentPlayer;
    GameMap currentMap;

    vector<Entity> entities;
    unordered_map<unsigned long, Position> positions;
    unordered_map<unsigned long, Monster> monsters;
    unordered_map<unsigned long, Item> items;



};
