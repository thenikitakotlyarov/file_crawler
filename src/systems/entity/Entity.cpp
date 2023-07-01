#include "Entity.h"


EntitySystem::EntitySystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

EntitySystem::~EntitySystem() {
    CleanUp();
}

bool EntitySystem::Initialize() {
    // Initialize systems
    //Positions[player_start_coords.first][player_start_coords.second] = 0;
    vacantEntity = {nextEntityId};
    nextEntityId++;


    return true;
}


// Use std::move to avoid copying
void EntitySystem::setPlayer(Player player) {
    currentPlayer = std::move(player);
};

// Use std::move to avoid copying
void EntitySystem::setGamemap(GameMap game_map) {
    currentMap = std::move(game_map);
    entityMap = {
            currentMap.meta,
            vector<vector<Entity>>(currentMap.data.size(),
                                   vector<Entity>(currentMap.data[0].size(), vacantEntity)
            )
    };

}


// Use reserve for known size vectors
Entity EntitySystem::createEntity(Position pos) {
    Entity entity = {nextEntityId,
                     true};
    nextEntityId++;
    entityMap.data[pos.x][pos.y] = entity;
    positionMap[entity] = pos;
    return entity;
}

void EntitySystem::destroyEntity(Entity entity) {
    Position entityPosition = positionMap[entity];
    entityMap.data[entityPosition.x][entityPosition.y] = vacantEntity;
    positionMap.erase(entity);
    if (monsters.find(entity) != monsters.end()) monsters.erase(entity);
    if (items.find(entity) != items.end()) items.erase(entity);
}


void EntitySystem::spawnPlayer() {
    int x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);


    //TODO: spawn player in sensible location
    while (!check_if_in(GROUND_TILES, currentMap.data[x][y].ch)) {
        x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);

    }


    Position playerPosition = {x, y};
    currentPlayerPosition = playerPosition;
    createEntity(playerPosition);


}


void EntitySystem::spawnMonsters() {
    int count = round(0.001333 * (currentMap.data.size() * currentMap.data[0].size()));
    vector<Monster> monsterTemplates = {
            getBasicMonster()
    };

    for (const auto &monster: monsterTemplates) {
        for (int i = 0; i < monster.rarity * count; ++i) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!check_if_in(GROUND_TILES, currentMap.data[x][y].ch));
            Entity monsterEntity = createEntity(Position{x, y});
            monsters[monsterEntity] = monster;
        }
    }

}


void EntitySystem::spawnItems() {
    int count = round(0.001333 * (currentMap.data.size() * currentMap.data[0].size()));
    vector<Item> itemTemplates = {
            getPotionTemplate()
    };

    for (const auto &item: itemTemplates) {
        for (int i = 0; i < item.rarity * count; ++i) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!check_if_in(GROUND_TILES, currentMap.data[x][y].ch));
            Entity itemEntity = createEntity(Position{x, y});
            entityMap.data[x][y] = itemEntity;
            items[itemEntity] = item;
        }
    }
}


void EntitySystem::Update() {
    unsigned long start_x = max(0, currentPlayerPosition.x - 50);
    unsigned long start_y = max(0, currentPlayerPosition.y - 50);
    unsigned long end_x = min((int) entityMap.data.size(), currentPlayerPosition.x + 50);
    unsigned long end_y = min((int) entityMap.data[0].size(), currentPlayerPosition.y + 50);

    for (int i = start_y; i < end_y; i++) {
        for (int j = start_x; j < end_x; j++) {
            Entity &entity_ref = entityMap.data[j][i];
            if (entity_ref.id > 2 && monsters.find(entity_ref) != monsters.end()) {
                if (get_random_int(1, 3) == 3) {//randomly

                    Monster monster = monsters[entity_ref];

                    Position intent_d = monster.Update(monster, positionMap[entity_ref], currentPlayer,
                                                       currentPlayerPosition,
                                                       currentMap);
                    pair<int, int> move_d = make_pair(intent_d.x - positionMap[entity_ref].x,
                                                      intent_d.y - positionMap[entity_ref].y);
                    moveEntity(entity_ref, move_d);
                }
            }
        }
    }
}


// Use const reference to avoid copying
const EntityMap &EntitySystem::getEntities() {
    return entityMap;
}

const Entity &EntitySystem::getPlayer() {
    return entityMap.data[currentPlayerPosition.x][currentPlayerPosition.y];
}

const Position &EntitySystem::getPlayerPosition() {
    return currentPlayerPosition;
}

map<Entity, Monster> EntitySystem::getMonsters() {
    return monsters;
}


map<Entity, Item> EntitySystem::getItems() {
    return items;
}


void EntitySystem::CleanUp() {
    // Cleanup on tear-down
    Positions.clear();

}

void EntitySystem::moveEntity(Entity entity, pair<int, int> direction) {
    int distance = 1;
    pair<int, int> delta = {0, 0};

    Position pos = positionMap[entity];


    for (int i = 0; i < distance; i++) {
        int dx = delta.first + direction.first;
        int dy = delta.second + direction.second;

        // Check if dx and dy are within the map's bounds
        if (pos.x + dx < 0 || pos.x + dx >= currentMap.data.size() ||
            pos.y + dy < 0 || pos.y + dy >= currentMap.data[0].size()) {
            continue;
        }

        if (check_if_in(GROUND_TILES, currentMap.data[pos.x + dx][pos.y + dy].ch)) {
//            && (!entity.transient && entityMap.data[pos.x + dx][pos.y + dy].id == 1)) {
            delta = {dx, dy};
        } else if (check_if_in(TRAP_TILES, currentMap.data[pos.x + dx][pos.y + dy].ch)) {
            int damage = get_random_int(1, 5);
            if (entity.id == 2) currentPlayer.current_health -= damage;
            delta = {dx, dy};
            break;
        } else {
            break; // Stop the loop if the entity hits a wall
        }
    }
    if (abs(delta.first) + abs(delta.second) > 0) {

        Position new_pos = {pos.x + delta.first, pos.y + delta.second};
        entityMap.data[pos.x][pos.y] = vacantEntity;
        entityMap.data[new_pos.x][new_pos.y] = entity;

        if (entity.id == 2) {
            currentPlayerPosition = new_pos;
        }

        positionMap[entity] = new_pos;
    }


}


set<pair<int, int>> EntitySystem::calculate_fov(
        const GameMap &game_map,
        int center_x, int center_y,
        int radius) {

    set<pair<int, int>> fov;

    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad) / 2;
        double x = center_x, y = center_y;

        for (int j = 0; j <= radius; ++j) {
            int ix = round(x), iy = round(y);

            if (ix < 0 || iy < 0 || ix >= WIDTH || iy >= HEIGHT)
                break;

            // check if we should insert this tile into fov
            if (j == 0 || i % j == 0) {
                fov.insert({ix, iy});
            }

            if (check_if_in(WALL_TILES, game_map.data[ix][iy].ch))
                break;

            x += dx;
            y += dy;
        }
    }

    return fov;
}


Frame EntitySystem::renderEntities2D(Frame frame, const GameMap& game_map, int start_y, int start_x, int end_y, int end_x) {
    Position position;
    for (int i = start_y; i < end_y; i++) {
        for (int j = start_x; j < end_x; j++) {
            Entity entity_ref = entityMap.data[j][i];
            if (entity_ref.id > 1) {
                if (items.find(entity_ref) != items.end()) {
                    frame.data[i - start_y][j - start_x].first = items[entity_ref].character;
                    frame.data[i - start_y][j - start_x].second.first = items[entity_ref].color;

                } else if (monsters.find(entity_ref) != monsters.end()) {
                    frame.data[i - start_y][j - start_x].first = monsters[entity_ref].characterFullHealth;
                    frame.data[i - start_y][j - start_x].second.first = monsters[entity_ref].color;

                } else {//tis the player
                    frame.data[i - start_y][j - start_x].first = L"@";
                    frame.data[i - start_y][j - start_x].second.first = currentPlayer.color;
                }

            }
        }
    }
    return frame;
}
