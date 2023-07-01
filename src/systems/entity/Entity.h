#pragma once


#include "components/Frame.h"
#include "components/GameMap.h"
#include "components/EntityMap.h"
#include "components/Position.h"
#include "components/player/Player.h"
#include "components/enemy/Monster.h"
#include "components/item/Item.h"
#include "components/Entity.h"

#include "templates/item/BaseItems.h"
#include "templates/enemy/BaseEnemies.h"

#include "constants.h"
#include "helpers.h"

using namespace std;



class EntitySystem {
public:
    EntitySystem();

    ~EntitySystem();

    int running;
    vector<vector<int>> Positions;

    void CleanUp();

    void Update();

    Entity createEntity(Position pos);

    void destroyEntity(Entity entity_id);

    const EntityMap& getEntities();


    void setPlayer(Player player);

    const Entity& getPlayer();
    const Position& getPlayerPosition();

    void setGamemap(GameMap game_map);


    void spawnPlayer();

    void spawnMonsters();

    void spawnItems();

    map<Entity, Monster> getMonsters();

    map<Entity, Item> getItems();

    void moveEntity(Entity entity, pair<int,int> direction);

    set<pair<int, int>> calculate_fov(
            const GameMap& game_map,
            int center_x, int center_y,
            int radius);

    Frame renderEntities2D(Frame frame, const GameMap& game_map,
                           int start_y, int start_x,
                           int end_y, int end_x);


private:
    bool Initialize();

    Player currentPlayer;
    Position currentPlayerPosition;
    GameMap currentMap;

    EntityMap entityMap;
    map<Entity, Position> positionMap;
    map<Entity, Monster> monsters;
    map<Entity, Item> items;

    unsigned long nextEntityId = 1;

    Entity vacantEntity;



};
