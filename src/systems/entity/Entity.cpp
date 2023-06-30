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


    return true;
}

void EntitySystem::Update() {
    //update entity positions based on game map
    for (auto& monster: monsters) {
        positions[monster.first] =
                monster.second.Update(monster.second,positions[monster.first],
                              currentPlayer,positions[0],
                              currentMap);
    }


}

Entity EntitySystem::createEntity() {
    Entity entity = {entities.size()};
    entities.push_back(entity);
    return entity;
}

void EntitySystem::destroyEntity(Entity entity) {
    positions.erase(entity.id);
    //currentPlayer.erase(entity.id);
    monsters.erase(entity.id);
    entities[entity.id].id = -1;

}


vector<Entity> EntitySystem::getEntities() {
    return entities;
}

unordered_map<unsigned long, Position> EntitySystem::getPositions() {
    return positions;
}


void EntitySystem::setPlayer(Player player) {
    currentPlayer = player;
};

Player EntitySystem::getPlayer() {
    return currentPlayer;
}

void EntitySystem::setGamemap(GameMap game_map) {
    currentMap = game_map;
}

void EntitySystem::spawnPlayer() {
    int x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);


    //TODO: spawn player in sensible location
    while (!check_if_in(GROUND_TILES, currentMap.data[x][y].ch)) {
        x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);

    }


    positions[0] = Position{x, y};

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
            Entity monsterEntity = createEntity();
            positions[monsterEntity.id] = Position{x, y};
            monsters[monsterEntity.id] = monster;
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
            Entity itemEntity = createEntity();
            positions[itemEntity.id] = Position{x, y};
            items[itemEntity.id] = item;
        }
    }
}


unordered_map<unsigned long, Monster> EntitySystem::getMonsters() {
    return monsters;
}


unordered_map<unsigned long, Item> EntitySystem::getItems() {
    return items;
}


void EntitySystem::CleanUp() {
    // Cleanup on tear-down
    Positions.clear();

}

void EntitySystem::movePlayer(pair<int, int> direction) {
    int distance = 1;
    pair<int, int> delta = {0, 0};
    Position playerPosition = getPositions()[0];

    for (int i = 0; i < distance; i++) {
        int dx = delta.first + direction.first;
        int dy = delta.second + direction.second;
        if (check_if_in(GROUND_TILES, currentMap.data[playerPosition.x + dx][playerPosition.y + dy].ch)) {
            delta = {dx, dy};
        } else if (check_if_in(TRAP_TILES, currentMap.data[playerPosition.x + dx][playerPosition.y + dy].ch)) {
            int damage = get_random_int(1, 5);
            currentPlayer.current_health -= damage;
            delta = {dx, dy};
            break;
        } else {
            break; // Stop the loop if the player hits a wall
        }
    }

    positions[0] = {playerPosition.x + delta.first, playerPosition.y + delta.second};


}


set<pair<int, int>> EntitySystem::calculate_fov(
        GameMap &game_map,
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


Frame EntitySystem::renderEntities2D(Frame frame, GameMap game_map,
                                     int start_y, int start_x,
                                     int end_y, int end_x) {
    Position position;


    for (const auto &entity: entities) {
        position = positions[entity.id];
        if (start_x >= position.x || position.x >= end_x || start_y >= position.y || position.y >= end_y) continue;

        if (monsters.find(entity.id) != monsters.end()) {
            frame.data[position.y - start_y][position.x - start_x].first = monsters[entity.id].characterFullHealth;
            frame.data[position.y - start_y][position.x - start_x].second.first = monsters[entity.id].color;
        } else if (items.find(entity.id) != items.end()) {
            frame.data[position.y - start_y][position.x - start_x].first = items[entity.id].character;
            frame.data[position.y - start_y][position.x - start_x].second.first = items[entity.id].color;

        }
    }


    return frame;
}

