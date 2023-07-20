#pragma once


#include "components/graphics/Frame.h"
#include "components/map/GameMap.h"
#include "components/map/EntityMap.h"
#include "components/map/Position.h"
#include "components/entity/actors/player/Player.h"
#include "components/entity/actors/enemy/Monster.h"
#include "components/entity/item/Item.h"
#include "components/entity/Entity.h"
#include "components/action/Intent.h"

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

    Entity createEntity(const Position pos,const bool transience,const bool emissiveness,const Color color);

    void destroyEntity(Entity entity_id);

    EntityMap *getEntities();


    void setPlayer(Player &player);

    void setGameMap(GameMap *game_map);


    const Entity &getPlayer();

    Player &getCurrentPlayer();

    const Position &getPlayerPosition();

    const Position &getPosition(Entity);

    const map<Entity, Position> &getPositions();

    void spawnPlayer();

    void spawnMonsters(const int count, const float difficulty);

    void spawnItems(const int count);

    map<Entity, Monster> &getMonsters();

    map<Entity, Item> &getItems();


    void handleIntent(const Intent &intent);

    void moveEntity(const Intent &intent);


    void damageEntity(const Entity entity, const int damage);

    void combatEntities(const Intent &intent);

    set<pair<int, int>> calculate_fov(
            int center_x, int center_y,
            int radius);

    Frame renderEntities2D(Frame frame,
                           const set<pair<int, int>> current_fov,
                           int start_y, int start_x,
                           int end_y, int end_x);


private:
    bool Initialize();

    Player currentPlayer;
    Position currentPlayerPosition;
    GameMap *currentMap;

    EntityMap* entityMap;
    map<Entity, Position> positionMap;
    map<Entity, Monster> monsters;
    map<Entity, Item> items;

    unsigned long nextEntityId = 1;


};
