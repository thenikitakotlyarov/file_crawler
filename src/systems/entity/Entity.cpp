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


// Use std::move to avoid copying
void EntitySystem::setPlayer(Player &player) {
    currentPlayer = player;
};

// Use std::move to avoid copying
void EntitySystem::setGameMap(GameMap *game_map) {
    currentMap = game_map;
    entityMap = new EntityMap {
            currentMap->meta,
            vector<vector<vector<Entity>>>(currentMap->data.size(),
                                           vector<vector<Entity>>(currentMap->data[0].size())
            )
    };

}


// Use reserve for known size vectors
Entity EntitySystem::createEntity(Position pos, bool transience) {
    Entity entity = {nextEntityId,
                     transience};
    nextEntityId++;
    entityMap->data[pos.x][pos.y].push_back(entity);
    positionMap[entity] = pos;
    return entity;
}

void EntitySystem::destroyEntity(Entity entity) {
    Position entityPosition = positionMap[entity];
    vector<Entity> &entitiesAtPosition = entityMap->data[entityPosition.x][entityPosition.y];
    entitiesAtPosition.erase(remove(entitiesAtPosition.begin(), entitiesAtPosition.end(), entity),
                             entitiesAtPosition.end());

    positionMap.erase(entity);

    monsters.erase(entity);
    items.erase(entity);
}


void EntitySystem::spawnPlayer() {
    int x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);


    //TODO: spawn player in sensible location
    while (currentMap->data[x][y].z) {
        x = get_random_int(1, WIDTH - 1), y = get_random_int(1, HEIGHT - 1);

    }


    Position playerPosition = {x, y};
    currentPlayerPosition = playerPosition;
    createEntity(playerPosition, false);


}


void EntitySystem::spawnMonsters() {
    int count = round(0.001333 * (currentMap->data.size() * currentMap->data[0].size()));
    vector<Monster> monsterTemplates = {
            getBasicMonster(),
            getBasicMonster(),
            getBasicMonster(),
            getBasicMonster(),
            getBasicMonster()
    };

    for (const auto &monster: currentMap->meta.monsterTemplates) {
        for (int i = 0; i < monster.rarity * count; ++i) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!currentMap->data[x][y].z);
            Entity monsterEntity = createEntity(Position{x, y}, monster.transient);
            monsters[monsterEntity] = monster;
        }
    }

}


void EntitySystem::spawnItems() {
    int count = round(0.001333 * (currentMap->data.size() * currentMap->data[0].size()));

    for (const auto &item: currentMap->meta.itemTemplates) {
        for (int i = 0; i < item.rarity * count; ++i) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!check_if_in(GROUND_TILES, currentMap->data[x][y].ch));
            Entity itemEntity = createEntity(Position{x, y}, true);
            items[itemEntity] = item;
        }
    }
}

void EntitySystem::Update() {
    size_t start_x = max(0, currentPlayerPosition.x - 50);
    size_t start_y = max(0, currentPlayerPosition.y - 50);
    size_t end_x = min(entityMap->data.size(), start_x + 100);
    size_t end_y = min(entityMap->data[0].size(), start_y + 100);

    vector<Intent> intents;

    for (size_t i = start_y; i < end_y; i++) {
        for (size_t j = start_x; j < end_x; j++) {
            vector<Entity> &entity_refs = entityMap->data[j][i];
            Entity playerEntity;
            for (Entity &entity_ref: entity_refs) {
                if (entity_ref.id == 1) {
                    playerEntity = entity_ref;
                    break;
                }
            }
            for (Entity &entity_ref: entity_refs) {

                if (entity_ref.id > 1 && monsters.find(entity_ref) != monsters.end()) {
                    Monster &monster = monsters[entity_ref];
                    Position &entityPosition = positionMap[entity_ref];
                    if (get_random_int(1, 3) == 3) {
                        Intent monster_intent = monster.Update(monster, entity_ref,
                                                               entityPosition, calculate_fov(entityPosition.x,
                                                                                             entityPosition.y,
                                                                                             monster.attackRadius),
                                                               currentPlayer, playerEntity, currentPlayerPosition,
                                                               currentMap);
//                        pair<int, int> move_d = make_pair(intent_d.x - entityPosition.x, intent_d.y - entityPosition.y);
//                        Intent this_intent = {
//                                entity_ref,IntentType::Move,move_d,{}
//                        };

                        intents.push_back(monster_intent);
                    }
                }
            }

        }
    }

    for (const Intent &intent: intents) {
        handleIntent(intent);
    }
}


// Use const reference to avoid copying
EntityMap *EntitySystem::getEntities() {
    return entityMap;
}

const Entity &EntitySystem::getPlayer() {

    const vector<Entity> &entitiesAtPlayerPosition = entityMap->data[currentPlayerPosition.x][currentPlayerPosition.y];
    for (const Entity &entity: entitiesAtPlayerPosition) {
        if (entity.id == 1) {
            return entity;
        }
    }

}


map<Entity, Monster> &EntitySystem::getMonsters() {
    return monsters;
}


map<Entity, Item> EntitySystem::getItems() {
    return items;
}


const Position &EntitySystem::getPlayerPosition() {
    return currentPlayerPosition;
}

const Position &EntitySystem::getPosition(Entity entity) {
    return positionMap[entity];
}

const map<Entity, Position> &EntitySystem::getPositions() {
    return positionMap;
}


void EntitySystem::CleanUp() {
    // Cleanup on tear-down
    Positions.clear();

}


void EntitySystem::handleIntent(const Intent &intent) {
    if (intent.type == IntentType::Move) {
        moveEntity(intent);
    } else if (intent.type == IntentType::Attack) {
        combatEntities(intent);
    }
}


void EntitySystem::moveEntity(const Intent &intent) {
    int distance = 1;
    pair<int, int> delta = {0, 0};
    Position pos = positionMap[intent.entity];

    for (int i = 0; i < distance; i++) {
        int dx = delta.first + intent.direction.first;
        int dy = delta.second + intent.direction.second;

        if (pos.x + dx < 0 || pos.x + dx >= currentMap->data.size() ||
            pos.y + dy < 0 || pos.y + dy >= currentMap->data[0].size()) {
            continue;
        }

        vector<Entity> &entitiesAtNewPosition = entityMap->data[pos.x + dx][pos.y + dy];

        if (!currentMap->data[pos.x + dx][pos.y + dy].z ||
            intent.entity.transient) {
            bool can_place = true;

            for (auto &entity: entitiesAtNewPosition) {
                if (!entity.transient) can_place = false;
                break;
            }
            if (can_place) delta = {dx, dy};

        } else if (check_if_in(TRAP_TILES, currentMap->data[pos.x + dx][pos.y + dy].ch)) {

            if (intent.entity.id == 1) {
                damageEntity(intent.entity, get_random_int(1, 5));
            }
            bool can_place = true;

            for (auto &entity: entitiesAtNewPosition) {
                if (!entity.transient) can_place = false;
                break;
            }
            if (can_place) delta = {dx, dy};
            break;
        } else {
            break;
        }
    }

    if (abs(delta.first) + abs(delta.second) > 0) {
        Position new_pos = {pos.x + delta.first, pos.y + delta.second};

        vector<Entity> &entitiesAtCurrentPosition = entityMap->data[pos.x][pos.y];
        vector<Entity> &entitiesAtNewPosition = entityMap->data[new_pos.x][new_pos.y];

        entitiesAtCurrentPosition.erase(
                remove(entitiesAtCurrentPosition.begin(), entitiesAtCurrentPosition.end(), intent.entity),
                entitiesAtCurrentPosition.end()
        );

        entitiesAtNewPosition.push_back(intent.entity);

        if (intent.entity.id == 1) {
            currentPlayerPosition = new_pos;
        }

        positionMap[intent.entity] = new_pos;
    }
}


void EntitySystem::damageEntity(const Entity entity, const int attackDamage) {
    if (entity.id == 1) {//player takes damage
        currentPlayer.current_health -= attackDamage;
        if (currentPlayer.current_health <= 0) {
            //player game over!
        }
    } else {//monster takes damage
        monsters[entity].health -= attackDamage;
        if (monsters[entity].health <= 0) {
            //monster game over!
            destroyEntity(entity);
        }

    }
}


void EntitySystem::combatEntities(const Intent &intent) {
    if (intent.entity.id == 1) {//player attacks monster
        //handle blocking, crits, etc here
        damageEntity(intent.target, intent.damage);
    } else if (intent.target.id == 1) {//monster attacks player
        //handle blocking, crits, etc here
        damageEntity(intent.target, intent.damage);

    } else {//monster attacks monster
        //handle blocking, crits, etc here
        damageEntity(intent.target, intent.damage);

    }
}


set<pair<int, int>> EntitySystem::calculate_fov(
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

            if (currentMap->data[ix][iy].z) break;

            x += dx;
            y += dy;
        }
    }
    fov.insert({center_x, center_y});

    return fov;
}


Frame
EntitySystem::renderEntities2D(Frame frame, const set<pair<int, int>> current_fov, int start_y,
                               int start_x, int end_y, int end_x) {
    for (int i = start_y; i < end_y; i++) {
        for (int j = start_x; j < end_x; j++) {
            Position this_pos = {j, i};
            if (is_position_in_fov(this_pos, current_fov)
                && !currentMap->data[j][i].z) {
                vector<Entity> &entity_refs = entityMap->data[j][i];
                for (Entity &entity_ref: entity_refs) {
                    if (items.find(entity_ref) != items.end()) {
                        frame.data[i - start_y][j - start_x].ch = items[entity_ref].character;
                        frame.data[i - start_y][j - start_x].fg_color = NCOLOR_BLACK;
                        frame.data[i - start_y][j - start_x].bg_color = items[entity_ref].color;


                    } else if (monsters.find(entity_ref) != monsters.end()) {
                        frame.data[i - start_y][j - start_x].ch = monsters[entity_ref].characterFullHealth;
                        frame.data[i - start_y][j - start_x].fg_color = NCOLOR_BLACK;
                        frame.data[i - start_y][j - start_x].bg_color = monsters[entity_ref].color;

                    } else {//tis the player
                        frame.data[i - start_y][j - start_x].ch = L"@";
                        frame.data[i - start_y][j - start_x].fg_color = NCOLOR_BLACK;
                        frame.data[i - start_y][j - start_x].bg_color = currentPlayer.color;
                    }

                }

            }
        }
    }
    return frame;
}

Player &EntitySystem::getCurrentPlayer() {
    return currentPlayer;
}
