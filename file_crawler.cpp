#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libnoise/noise.h>
#include <ncurses.h>
#include <vector>
#include <cmath>
#include <random>
#include <unordered_map>
#include <functional>
#include <cstdio>
#include <memory>
#include <queue>
#include <set>
#include <utility>
#include <chrono>
#include <thread>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "helpers.cpp"


using namespace std;


/*
:::::::::: ::::::::::: :::        ::::::::::
:+:            :+:     :+:        :+:
+:+            +:+     +:+        +:+
:#::+::#       +#+     +#+        +#++:++#
+#+            +#+     +#+        +#+
#+#            #+#     #+#        #+#
###        ########### ########## ##########

 ::::::::  :::::::::      :::     :::       ::: :::        :::::::::: :::::::::
:+:    :+: :+:    :+:   :+: :+:   :+:       :+: :+:        :+:        :+:    :+:
+:+        +:+    +:+  +:+   +:+  +:+       +:+ +:+        +:+        +:+    +:+
+#+        +#++:++#:  +#++:++#++: +#+  +:+  +#+ +#+        +#++:++#   +#++:++#:
+#+        +#+    +#+ +#+     +#+ +#+ +#+#+ +#+ +#+        +#+        +#+    +#+
#+#    #+# #+#    #+# #+#     #+#  #+#+# #+#+#  #+#        #+#        #+#    #+#
 ########  ###    ### ###     ###   ###   ###   ########## ########## ###    ###
*/
// v0.6x

const string DEV_LOG_FILE = "dev_log.txt";
const string GAME_LOG_FILE = "game_log.txt";

const int UI_LOG_COUNT = 7;

const int FPS = 30;
const int WIDTH = 1024, HEIGHT = 1024;
const int PLAYER_MAX_HP = 100;
const int PLAYER_FOV_RADIUS = 3;
const int PLAYER_MEMORY_RADIUS = pow(PLAYER_FOV_RADIUS,2);

const bool sound = false;
Mix_Chunk* PLAYER_FOOTSTEP_SOUND = nullptr;
const int PLAYER_FOOTSTEP_SOUND_COUNT = 6;

const vector<char> GROUND_TILES = {'.',',','`','"'};
const vector<char> WALL_TILES = {'[',']','{','}','=','|'};
const vector<char> TRAP_TILES = {'&','#'};
const vector<char> VOID_TILES = {' '};


const char PLAYER_TILE = '@';
const vector<char> MONSTER_TILES = {
    'M','N','m','n','r','i'
};
const vector<char> ITEM_TILES = {
    'o','O','0','s','S','7','d','D','8','a','A','9'
};

vector<char> get_entity_tiles() {
    vector<char> tiles;
    tiles.insert(tiles.end(), MONSTER_TILES.begin(), MONSTER_TILES.end());
    tiles.insert(tiles.end(), ITEM_TILES.begin(), ITEM_TILES.end());
    tiles.push_back(PLAYER_TILE);
    return tiles;
}
vector<char> ENTITY_TILES = get_entity_tiles();


vector<string> combat_log;



template<typename... Args>
void log(const string filename, Args&&... args) {
    ofstream logfile(filename, ios::app);  // Open the file in append mode

    if (!logfile.is_open()) return;

    // Redirect cout to the log file
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(logfile.rdbuf());

    // Create a string stream
    ostringstream oss;
    (oss << ... << forward<Args>(args));
    // Log the formatted message
    cout << oss.str() << endl;

    // Restore cout
    cout.rdbuf(coutbuf);
    // Close the log file
    logfile.close();
}


void add_combat_log(const string& message) {
    log(DEV_LOG_FILE, message);
    log(GAME_LOG_FILE, message);
    combat_log.push_back(message);
    if (combat_log.size() > UI_LOG_COUNT) {
        combat_log.erase(combat_log.begin());
    }
}


// AUDIO
void playFootstepSound() {

    // Generate a random number between 1 and maximum sound file index
    pair<int,int> range = { 1, PLAYER_FOOTSTEP_SOUND_COUNT };

    // Construct the file name with the randomly generated number
    string file_id = to_string(get_random_int(range.first,range.second));
    string fileName = "data/sound/footstep"+file_id+".wav";

    // Load the random sound file
    Mix_Chunk* footstepSound = Mix_LoadWAV(fileName.c_str());
    if (!footstepSound) {
        log(DEV_LOG_FILE, fileName + "  does not exist");
        // Handle sound loading error
        // Example: cerr << "Failed to load footstep sound: " << Mix_GetError() << endl;
        return;
    }

    // Play the random sound file
    Mix_PlayChannel(-1, footstepSound, 0);

    // Free the sound chunk after playing
    // Mix_FreeChunk(footstepSound);
}

// COLOR SETUP
vector<short> generate_palette(int seed) {
    log(DEV_LOG_FILE, "called generate_palette with seed as ", seed);
    // Assumes all color pairs have been initialized.
    vector<short> palette(5);

    // 1..6 are red, green, yellow, blue, magenta and cyan
    // 9..14 are their "intensified" versions
    vector<int> available_colors = {1, 2, 3, 4, 5, 6, 9, 10, 11, 12, 13, 14};

    //{COLOR_RED, COLOR_GREEN, COLOR_YELLOW, COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN};

    for (int i = 0; i < 5; ++i) {
        int color_index = get_random_int(0, available_colors.size() - 1);
        short foreground = available_colors[color_index];
        short background = COLOR_BLACK;

        palette[i] = get_color_pair_index(foreground, background);

        available_colors.erase(available_colors.begin() + color_index);
    }

    return palette;
}

vector<int> get_ground_swatch() {
    vector<vector<int>> options = {
        {3,4},  //green, yellow
        {2,4}   //red, yellow
    };
    return options[get_random_int(0,options.size() - 1)];
}

vector<int> get_trap_swatch() {
    vector<vector<int>> options= {
        {5,7}   //blue, cyan
    };
    return options[get_random_int(0,options.size() - 1)];
}

vector<int> get_wall_swatch() {
    vector<vector<int>> options= {
        {5,7}   //blue, cyan
    };
    return options[get_random_int(0,options.size() - 1)];
}

int get_tile_color(const vector<int> swatch) {
    return swatch[get_random_int(0,swatch.size() - 1)];
}



// MAP GEN
struct Tile {
    char ch;
    short color;
    bool visited;
    bool visible;
};

vector<vector<Tile>> generate_game_map() {
    log(DEV_LOG_FILE, "called generate_game_map");
    vector<vector<Tile>> game_map(WIDTH, vector<Tile>(HEIGHT));

    vector<int> ground_swatch = get_ground_swatch();
    vector<int> trap_swatch = get_trap_swatch();
    vector<int> wall_swatch = get_wall_swatch();


    // Generate open areas using Perlin noise
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            char ch = get_random_character(VOID_TILES);
            short color = get_color_pair_index(COLOR_WHITE,COLOR_BLACK);
            Tile void_tile = {ch, color, false, false};
            game_map[x][y] = void_tile;
            double value = perlin.GetValue(x / 10.0, y / 10.0, 0.0);


            if (value < 0.2) {
                ch = get_random_character(GROUND_TILES);
                color = get_tile_color(ground_swatch);
            } else if (value < 0.21) {
                ch = get_random_character(TRAP_TILES);
                color = get_tile_color(trap_swatch);
            } else if (value < 0.8) {
                ch = get_random_character(WALL_TILES);
                color = get_tile_color(wall_swatch);
            }

            Tile this_tile = {ch, color, false, false};
            game_map[x][y] = this_tile;
        }
    }

    // Flood fill to ensure all open areas are connected to an edge
    vector<vector<bool>> visited(WIDTH, vector<bool>(HEIGHT, false));
    queue<pair<int, int>> q;

    // Start flood fill from a random point on top edge
    int initial_x = get_random_int(1, WIDTH-1);
    int initial_y = get_random_int(1, HEIGHT-1);
    while (!check_if_in(GROUND_TILES, game_map[initial_x][initial_y].ch)) {
        initial_x += max(1,min(WIDTH-1,get_random_int(-1,1)));
        initial_y += max(1,min(HEIGHT-1,get_random_int(-1,1)));
    }
    visited[initial_x][initial_y] = true;
    q.push({initial_x, initial_y});

    // Perform flood fill
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Check neighboring cells
        const vector<pair<int, int>> neighbors = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (const auto& neighbor : neighbors) {
            int nx = x + neighbor.first;
            int ny = y + neighbor.second;

            // Check if neighbor is within bounds and an open area
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && check_if_in(GROUND_TILES, game_map[nx][ny].ch) && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    // Convert unvisited cells to closed spaces
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (!visited[x][y] && check_if_in(GROUND_TILES, game_map[x][y].ch)) {
                char ch = get_random_character(WALL_TILES);
                short color = get_tile_color(wall_swatch);
                Tile this_tile = {ch, color, false, false};
                game_map[x][y] =  this_tile;
            }
        }
    }

    // Draw a border around the game_map
    for (int x = 0; x < WIDTH; ++x) {
        char ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false};
        game_map[x][0] = this_tile;
        game_map[x][HEIGHT - 1] = this_tile;
    }
    for (int y = 0; y < HEIGHT; ++y) {
        char ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false};
        game_map[0][y] = this_tile;
        game_map[WIDTH - 1][y] = this_tile;
    }


    return game_map;
}



// PATHING AND FOV
struct PositionComponent {
    int x;
    int y;
};

set<pair<int, int>> calculate_fov(
        vector<vector<Tile>>& game_map,
        int center_x, int center_y,
        int radius) {

    set<pair<int, int>> fov;

    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad) / 2;
        double x = center_x, y = center_y;

        for (int j = 0; j < radius; ++j) {
            x += dx;
            y += dy;
            int ix = round(x), iy = round(y);
            // ix = max(0,min(WIDTH,ix));
            // iy = max(0,min(HEIGHT,iy));

            if (ix < 0 || iy < 0 || ix >= WIDTH || iy >= HEIGHT)
                break;

            fov.insert({ix, iy});
            if (check_if_in(WALL_TILES, game_map[ix][iy].ch))
                break;

        }
    }

    return fov;
}


double heuristic(const vector<vector<Tile>>& game_map, int x1, int y1, int x2, int y2) {
    // Modify the heuristic function implementation according to your needs
    return abs(x1 - x2) + abs(y1 - y2);
}

double cost(const vector<vector<Tile>>& game_map, int x2, int y2) {
    // Modify the cost function implementation according to your needs
    return 1.0;
}

vector<Node> aStar(const PositionComponent& start, const PositionComponent& goal,
                   const vector<vector<Tile>>& game_map) {

    vector<vector<bool>> closedSet(WIDTH, vector<bool>(HEIGHT, false));
    vector<vector<Node*>> openSet(WIDTH, vector<Node*>(HEIGHT, nullptr));

    priority_queue<Node*, vector<Node*>, NodePriorityQueue> queue;

    openSet[start.x][start.y] = new Node(start.x, start.y);
    queue.push(openSet[start.x][start.y]);

    while (!queue.empty()) {
        Node* current = queue.top();
        queue.pop();

        if (current->x == goal.x && current->y == goal.y) {
            vector<Node> path;
            while (current != nullptr) {
                path.push_back(*current);
                current = current->parent;
            }
            reverse(path.begin(), path.end());

            for (int x = 0; x < WIDTH; x++) {
                for (int y = 0; y < HEIGHT; y++) {
                    delete openSet[x][y];
                }
            }

            return path;
        }

        closedSet[current->x][current->y] = true;

        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                int x = current->x + dx + get_random_int(-1,1);
                int y = current->y + dy + get_random_int(-1,1);

                if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT && !closedSet[x][y]) {
                    double tentativeG = current->g + cost(game_map, x, y);

                    if (openSet[x][y] == nullptr && tentativeG != INFINITY) {
                        openSet[x][y] = new Node(x, y, current, tentativeG, heuristic(game_map, x, y, goal.x, goal.y));
                        queue.push(openSet[x][y]);
                    } else if (tentativeG < openSet[x][y]->g) {
                        openSet[x][y]->g = tentativeG;
                    }
                }
            }
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            delete openSet[x][y];
        }
    }

    return vector<Node>();
}


// ENTITIES
struct PlayerStats {
    int level;
    string class_name;
    int health;
    int attack;
    int defense;
    int speed;
};

// COMPONENTS
struct AIComponent {
    int chaseRadius;
    int attackRadius;
    int cooldown;
};
enum class ItemType {
    SmallPotion,
    MediumPotion,
    LargePotion,
    WeakAttackSerum,
    NormalAttackSerum,
    StrongAttackSerum,
    WeakDefenseSerum,
    NormalDefenseSerum,
    StrongDefenseSerum,
    WeakSpeedSerum,
    NormalSpeedSerum,
    StrongSpeedSerum
};

struct ItemComponent {
    ItemType type;
    char character;
    string name;
    float rarity;
    function<void(PlayerStats&)> effect;
};
struct MonsterComponent {
    int attackPower;
    int attackRadius;
    int cooldown;
    int health;
};





using Entity = int;

class EntityManager {
private:
    vector<Entity> entities;
    unordered_map<Entity, PositionComponent> positions;
    unordered_map<Entity, PlayerStats> playerStats;
    unordered_map<Entity, AIComponent> ais;
    unordered_map<Entity, MonsterComponent> monsters;
    unordered_map<Entity, ItemComponent> items;
    int nextEntityId;

public:
    EntityManager() : nextEntityId(0) {}

    Entity createEntity() {
        Entity entity = nextEntityId++;
        entities.push_back(entity);
        return entity;
    }

    void destroyEntity(Entity entity) {
        auto it = find(entities.begin(), entities.end(), entity);
        if (it != entities.end()) {
            entities.erase(it);
            positions.erase(entity);
            playerStats.erase(entity);
            ais.erase(entity);
            monsters.erase(entity);
        }
    }

    vector<Entity>& getEntities() {
        return entities;
    }

    unordered_map<Entity, PositionComponent>& getPositions() {
        return positions;
    }

    unordered_map<Entity, PlayerStats>& getPlayerStats() {
        return playerStats;
    }

    unordered_map<Entity, AIComponent>& getAIComponents() {
        return ais;
    }

    unordered_map<Entity, MonsterComponent>& getMonsterComponents() {
        return monsters;
    }

    unordered_map<Entity, ItemComponent>& getItemComponents() {
        return items;
    }
};


// ITEMS

vector<ItemComponent> itemTemplates = {
        {ItemType::SmallPotion, 'o', "Small Health Potion",//< item identifier
            3.0, //< item rarity
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP, stats.health + 5);}},
            //item ability parms^ item ability ^
        {ItemType::MediumPotion, 'O', "Medium Health Potion", 1,
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 10);}},
        {ItemType::LargePotion, '0', "Large Health Potion", 0.5,
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 25);}},
        {ItemType::WeakAttackSerum, 'a', "Weak ATT-Serum", 1.0,
            [](PlayerStats& stats) { stats.attack += 1; }},
        {ItemType::NormalAttackSerum, 'A', "Normal ATT-Serum", 0.5,
            [](PlayerStats& stats) { stats.attack += 2; }},
        {ItemType::StrongAttackSerum, '9', "Strong ATT-Serum", 0.25,
            [](PlayerStats& stats) { stats.attack += 3; }},
        {ItemType::WeakDefenseSerum, 'd', "Weak DEF-Serum", 1.0,
            [](PlayerStats& stats) { stats.defense += 1; }},
        {ItemType::NormalDefenseSerum, 'D', "Normal DEF-Serum", 0.5,
            [](PlayerStats& stats) { stats.defense += 2; }},
        {ItemType::StrongDefenseSerum, '8', "Strong DEF-Serum", 0.25,
            [](PlayerStats& stats) { stats.defense += 3; }},
        {ItemType::WeakSpeedSerum, 's', "Weak SPD-Serum", 1.0,
            [](PlayerStats& stats) { stats.speed += 1; }},
        {ItemType::NormalSpeedSerum, 'S', "Normal SPD-Serum", 0.5,
            [](PlayerStats& stats) { stats.speed += 2; }},
        {ItemType::StrongSpeedSerum, '7', "Strong SPD-Serum", 0.25,
            [](PlayerStats& stats) { stats.speed += 3; }}
    };


void spawn_items(int count, EntityManager& entityManager, vector<vector<Tile>>& game_map) {
    log(DEV_LOG_FILE, "spawning items");

    for (const auto& item : itemTemplates) {
        for (int i = 0; i < item.rarity * count; ++i) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!check_if_in(GROUND_TILES, game_map[x][y].ch));
            Entity itemEntity = entityManager.createEntity();
            entityManager.getPositions()[itemEntity] = {x, y};
            entityManager.getItemComponents()[itemEntity] = item;
        }
    }
}

// MONSTERS
class MonsterSystem {
private:
    EntityManager& entityManager;
    vector<vector<Tile>>& game_map;
    PlayerStats& playerStats;

public:
    MonsterSystem(EntityManager& entityManager, vector<vector<Tile>>& game_map, PlayerStats& playerStats)
        : entityManager(entityManager), game_map(game_map), playerStats(playerStats) {}

    void update() {
        log(DEV_LOG_FILE, "updating monster system");
        auto& positions = entityManager.getPositions();
        auto& monsters = entityManager.getMonsterComponents();

        for (const auto& entry : positions) {
            Entity entity = entry.first;

            // Skip non-monster entities
            if (monsters.find(entity) == monsters.end())
                continue;

            PositionComponent& monsterPosition = positions[entity];
            PositionComponent& playerPosition = positions[0];
            AIComponent& ai = entityManager.getAIComponents()[entity];
            MonsterComponent& monster = monsters[entity];

            monster.cooldown = max(0, monster.cooldown - 1);
            int dx = monsterPosition.x - playerPosition.x;
            int dy = monsterPosition.y - playerPosition.y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= ai.chaseRadius) {
                log(DEV_LOG_FILE, "monster", entity, " chases player");

                // Get the path from the monster to the player
                log(DEV_LOG_FILE, "monster", entity, " is pathing");
                vector<Node> path = aStar(monsterPosition, playerPosition, game_map);
                log(DEV_LOG_FILE, "monster", entity, " found path");
                int new_x = monsterPosition.x;
                int new_y = monsterPosition.y;
                // Move the monster to the next node in the path if it exists
                if (!path.empty()) {
                    log(DEV_LOG_FILE, "monster", entity, " taking path");
                    // introduce a little noise to monster path
                    new_x = max(1,min(WIDTH-1,path[1].x));
                    new_y = max(1,min(HEIGHT-1,path[1].y));
                } else {
                    log(DEV_LOG_FILE, "monster", entity, " wanders");
                    int wander_x, wander_y = 0;
                    // introduce a little noise to monster path
                    if ( get_random_int(1,FPS) == FPS ) {
                        int wander_x = get_random_int(-1,1);
                    }
                    if ( get_random_int(1,FPS) == FPS ) {
                        int wander_y = get_random_int(-1,1);
                    }
                    new_x = max(1,min(WIDTH-1,monsterPosition.x+wander_x));
                    new_y = max(1,min(HEIGHT-1,monsterPosition.y+wander_y));

                }


                // Get all positions
                const auto& all_positions = entityManager.getPositions();

                // Flag to indicate whether the new position is occupied
                bool is_occupied = false;

                // Check if the new position is occupied by the player or another monster
                for (const auto& pos_entry : all_positions) {
                    if ((pos_entry.second.x == new_x) && (pos_entry.second.y == new_y)) {
                        is_occupied = true;
                        break;
                    }
                }

                log(DEV_LOG_FILE, "monster", entity, " moving along path");
                // Only move the monster to the new position if it's not occupied
                if (!is_occupied and check_if_in(GROUND_TILES, game_map[new_x][new_y].ch)) {
                    monsterPosition.x = new_x;
                    monsterPosition.y = new_y;
                }

                log(DEV_LOG_FILE, "checking monster", entity, "'s attack range");
                set<pair<int, int>> monster_attack_range =
                    calculate_fov(
                        game_map,
                        monsterPosition.x, monsterPosition.y,
                        monster.attackRadius
                    );
                log(DEV_LOG_FILE, "monster", entity, "'s attack range set");


                if (distance <= ai.attackRadius && monster.cooldown == 0
                    && is_in_set({playerPosition.x, playerPosition.y}, monster_attack_range) ) {
                    log(DEV_LOG_FILE, "monster", entity, " attacks player");

                    int damage = max(0,get_random_int(1, monster.attackPower) - get_random_int(0,playerStats.defense));
                    playerStats.health -= damage;
                    monster.cooldown = get_random_int(FPS / 3, FPS * 4 / 3);

                    if (damage > 0) {
                        add_combat_log("Player took " + to_string(damage) + " damage from a monster.");
                    } else {
                        add_combat_log("Player blocked the monster's attack.");
                    }
                }
            }

        }
    }
};



void spawn_monsters(int monster_count, EntityManager& entityManager, vector<vector<Tile>>& game_map) {
    log(DEV_LOG_FILE, "spawning monsters");
    for (int i = 0; i < monster_count; ++i) {
        int x, y;
        do {
            x = get_random_int(0, WIDTH - 1);
            y = get_random_int(0, HEIGHT - 1);
        } while (!check_if_in(GROUND_TILES, game_map[x][y].ch));
        Entity monsterEntity = entityManager.createEntity();
        entityManager.getPositions()[monsterEntity] = {x, y};
        
        pair<int, int> chase_dis = {7, 25};
        pair<int, int> attack_range_dis = {1, 7};
        pair<int, int> attack_power_dis = {1, 20};
        pair<int, int> health_dis = {5, 25};
        
        entityManager.getAIComponents()[monsterEntity] = {
            get_random_int(chase_dis.first,chase_dis.second),
            get_random_int(attack_range_dis.first,attack_range_dis.second),
            FPS * 3
        };
        entityManager.getMonsterComponents()[monsterEntity] = {
            get_random_int(attack_power_dis.first,attack_power_dis.second),
            get_random_int(attack_range_dis.first,attack_range_dis.second),
            FPS * 3,
            get_random_int(health_dis.first,health_dis.second)
        };
    }
}


void render_buffer(
        const vector<pair<char, pair<int, int>>>& draw_buffer,
        const vector<vector<Tile>>& game_map,
        const vector<short>& palette,
        int start_x, int start_y) {
    
    log(DEV_LOG_FILE, "rendering buffer");

    // clear();
    int py, px;
    for (const auto& item : draw_buffer) {
        char ch = item.first;
        int y = item.second.first;
        int x = item.second.second;
        int Y = y - start_y;
        int X = x - start_x;


        short color_pair = get_color_pair_index(COLOR_BLACK, COLOR_BLACK);

        if (!check_if_in(ENTITY_TILES,ch)// visible environment
            && game_map[x][y].visible) {
            color_pair = game_map[x][y].color;
        } else if (game_map[x][y].visited && !game_map[x][y].visible) { // memory
            color_pair = get_color_pair_index(8, COLOR_BLACK);
        } else if (check_if_in(MONSTER_TILES,ch) //monsters
                   && game_map[x][y].visible) {
            color_pair = palette[2];
        } else if (check_if_in(ITEM_TILES,ch) //items
                   && game_map[x][y].visible) {
            color_pair = palette[11];
        }

        if (ch == PLAYER_TILE) {
            py = Y;
            px = X;
        } else {
            attron(COLOR_PAIR(color_pair));
            mvaddch(Y, X, ch);
            attroff(COLOR_PAIR(color_pair));
        }
    }
    // draw character last
    attron(COLOR_PAIR(palette[3]));
    mvaddch(py, px, PLAYER_TILE);
    attroff(COLOR_PAIR(palette[3]));

}



int get_player_color(PlayerStats& playerStats) {
    int color_index;

    if (playerStats.class_name == "Fighter") {
        color_index = 2;//red
    } else if (playerStats.class_name == "Cleric") {
        color_index = 6;//magenta
    } else if (playerStats.class_name == "Wizard") {
        color_index = 5;//blue
    } else if (playerStats.class_name == "Sorcerer") {
        color_index = 7;//cyan
    } else if (playerStats.class_name == "Ranger") {
        color_index = 3;//green
    } else if (playerStats.class_name == "Rogue") {
        color_index = 4;//yellow
    } else if (playerStats.class_name == "Adventurer") {
        color_index = 8;//white
    }

    return color_index;
}

string get_player_class_name(PlayerStats& playerStats) {
    string class_name;

    if (playerStats.attack > playerStats.defense
        && playerStats.attack > playerStats.speed) {
        class_name = "Fighter";//red
    } else if (playerStats.defense == playerStats.attack
               && playerStats.defense > playerStats.speed) {
        class_name = "Cleric";//magenta
    } else if (playerStats.defense > playerStats.attack
               && playerStats.defense > playerStats.speed) {
        class_name = "Wizard";//blue
    } else if (playerStats.defense == playerStats.speed
               && playerStats.defense > playerStats.attack) {
        class_name = "Sorcerer";//cyan
    } else if (playerStats.speed > playerStats.attack
               && playerStats.speed > playerStats.defense) {
        class_name = "Ranger";//green
    } else if (playerStats.speed == playerStats.attack
               && playerStats.speed > playerStats.defense) {
        class_name = "Rogue";//yellow
    } else if (playerStats.defense == playerStats.attack
               && playerStats.defense == playerStats.speed) {
        class_name = "Adventurer";//white
    }

    return class_name;
}

int get_player_level(PlayerStats& playerStats) {
    return round((playerStats.attack + playerStats.defense + playerStats.speed)/10);
}

void draw_UI(PlayerStats& playerStats) {

    //draw health orb
    int orb_diameter = 8;//it's actually half this value on the y
    int orb_size = (orb_diameter*orb_diameter)/2;
    int health_level =  static_cast<int>(static_cast<float>(playerStats.health) / PLAYER_MAX_HP * orb_size);
    int x = 2;
    int y = LINES - (orb_diameter/2) - 2;
    int player_level = playerStats.level;

    //draw background for hud
    for (int hy = 1; hy < orb_diameter-1; ++hy) {
        for (int hx = 1; hx < 30; ++hx) {
            mvprintw(y+hy,x+hx," ");
        }
    }

    for (int oy = 0; oy < (orb_diameter/2) + 2; ++oy) {
        for(int ox = 0; ox < orb_diameter + 2; ++ox) {

            //draw box in center
            if (   (ox != 0 && oy!=0)
                && (ox!=orb_diameter+1 && oy!=0)
                && (ox != 0 && oy!=orb_diameter/2+1)
                && (ox!=orb_diameter+1 && oy!=orb_diameter/2+1)){
                int color_pair;
                char ch;
                    if (health_level >= orb_size - (((oy) * orb_diameter) - ox)) {
                        ch = get_random_character({' ',' ',' ','~'});
                        color_pair = get_color_pair_index(COLOR_BLACK,COLOR_RED);
                    } else{
                        ch =get_random_character({' ',' ',' ',' ',' ',' ',' ',' ','.'});
                        color_pair = get_color_pair_index(8,COLOR_BLACK);
                    }

                attron(COLOR_PAIR(color_pair));
                string str_ch(1, ch);
                mvprintw(y+oy,1+ox,str_ch.c_str());
                attroff(COLOR_PAIR(color_pair));
            }
            //draw boundary
            if (   (oy==0 || oy==orb_diameter/2+1) && !(ox==0 || ox==orb_diameter+1)
                || (ox==0 || ox==orb_diameter+1) && !(oy==0 || oy==orb_diameter/2+1)
                || (ox==1 || ox==orb_diameter) && (oy==1 || oy==orb_diameter/2)){
                attron(COLOR_PAIR(get_color_pair_index(8,COLOR_BLACK)));
                mvprintw(y+oy,1+ox,"O");
                attroff(COLOR_PAIR(get_color_pair_index(8,COLOR_BLACK)));


            }
        }
    }

    x = orb_diameter + 5;
    int player_color = get_player_color(playerStats);
    string player_class = playerStats.class_name;
    attron(COLOR_PAIR(player_color));

    //draw level and class name
    mvprintw(y+1, x, "%s",
             player_class.c_str()
            );
    mvprintw(y+2, x, "LVL%d",
             player_level
            );
    attroff(COLOR_PAIR(player_color));

    //draw stats
    mvprintw(y+4, x, "ATT: %d DEF: %d SPD: %d",
             playerStats.attack,
             playerStats.defense,
             playerStats.speed
            );

    y = 1;
    x = 2;

    //draw combat log
    int log_size = min(LINES - 3, static_cast<int>(combat_log.size()));

    for (int i = 0; i < log_size; i++) {
        mvprintw(y + i, x, combat_log[combat_log.size() - log_size + i].c_str());
    }



}


pair<int,int> move_player(EntityManager& entityManager,
                          PlayerStats& playerStats,
                          const vector<vector<Tile>>& game_map,
                          const pair<int,int> direction,
                          const int sprinting) {
    log(DEV_LOG_FILE, "player opts to move x", direction.first, ",y", direction.second);

    pair<int,int> delta = {0, 0};
    int distance = 1;
    if (sprinting > 0.0f) {
        distance = min(playerStats.speed,int(pow(sprinting,1.2)));
    }

    auto& positions = entityManager.getPositions();
    PositionComponent& playerPosition = positions[0];

    for (int i = 0; i < distance; i++) {
        int dx = delta.first + direction.first;
        int dy = delta.second + direction.second;
        if (check_if_in(GROUND_TILES, game_map[playerPosition.x+dx][playerPosition.y+dy].ch)) {
            delta = {dx, dy};
        } else if (check_if_in(TRAP_TILES, game_map[playerPosition.x+dx][playerPosition.y+dy].ch)) {
            int damage = get_random_int(1,5);
            playerStats.health -= damage;
            add_combat_log("Player took " + to_string(damage) + " damage from a trap.");
            delta = {dx, dy};
            break;
        }
        else {
            break; // Stop the loop if the player hits a wall
        }
    }

    if (sound) {
        playFootstepSound();
    }

    return delta;

}

void print_class_menu() {

        clear();
        printw("Please select a class:\n\n");

        printw("1.  ");
        attron(COLOR_PAIR(2));
        printw("Fighter\n");
        attroff(COLOR_PAIR(2));

        printw("2.  ");
        attron(COLOR_PAIR(4));
        printw("Rogue\n");
        attroff(COLOR_PAIR(4));

        printw("3.  ");
        attron(COLOR_PAIR(3));
        printw("Ranger\n");
        attroff(COLOR_PAIR(3));

        printw("4.  ");
        attron(COLOR_PAIR(7));
        printw("Sorcerer\n");
        attroff(COLOR_PAIR(7));

        printw("5.  ");
        attron(COLOR_PAIR(5));
        printw("Wizard\n");
        attroff(COLOR_PAIR(5));

        printw("6.  ");
        attron(COLOR_PAIR(6));
        printw("Cleric\n");
        attroff(COLOR_PAIR(6));

        printw("7.  ");
        attron(COLOR_PAIR(8));
        printw("Adventurer\n");
        attroff(COLOR_PAIR(8));

        printw("8.  ");
        attron(COLOR_PAIR(2));
        printw("R");
        attroff(COLOR_PAIR(2));
        attron(COLOR_PAIR(4));
        printw("A");
        attroff(COLOR_PAIR(4));
        attron(COLOR_PAIR(3));
        printw("N");
        attroff(COLOR_PAIR(3));
        attron(COLOR_PAIR(7));
        printw("D");
        attroff(COLOR_PAIR(7));
        attron(COLOR_PAIR(5));
        printw("O");
        attroff(COLOR_PAIR(5));
        attron(COLOR_PAIR(6));
        printw("M");
        attroff(COLOR_PAIR(6));
        attron(COLOR_PAIR(8));
        printw("!");
        attroff(COLOR_PAIR(8));

        refresh();
}

vector<int> set_player_class(const int classChoice) {
    int a, b;
    int playerAttack, playerDefense, playerSpeed;

    switch(classChoice) {
        case 1: // Fighter
            a = get_random_int(2,3);
            b = 13 - 2 * a;
            playerAttack  = b;
            playerDefense = a;
            playerSpeed   = a;
            break;
        case 2: // Rogue
            a = get_random_int(5,6);
            b = 13 - 2 * a;
            playerAttack  = a;
            playerDefense = b;
            playerSpeed   = a;
            break;
        case 3: // Ranger
            a = get_random_int(2,3);
            b = 13 - 2 * a;
            playerAttack  = a;
            playerDefense = a;
            playerSpeed   = b;
            break;
        case 4: // Sorcerer
            a = get_random_int(5,6);
            b = 13 - 2 * a;
            playerAttack  = b;
            playerDefense = a;
            playerSpeed   = a;
            break;
        case 5: // Wizard
            a = get_random_int(2,3);
            b = 13 - 2 * a;
            playerAttack  = a;
            playerDefense = b;
            playerSpeed   = a;
            break;
        case 6: // Cleric
            a = get_random_int(5,6);
            b = 13 - 2 * a;
            playerAttack  = a;
            playerDefense = a;
            playerSpeed   = b;
            break;
        case 7: // Adventurer
            playerAttack  = 4;
            playerDefense = 4;
            playerSpeed   = 4;
            break;
        default: //random class choice
            a = get_random_int(1,6);
            b = 14 - a - get_random_int(1,6);
            int c = 14 - a - b;
            playerAttack  = a;
            playerDefense = b;
            playerSpeed   = c;
    }

    return {playerAttack, playerDefense, playerSpeed};
}

int main() {
    log(DEV_LOG_FILE, "Running file_crawler");

    initscr();
    log(DEV_LOG_FILE, "initialized screen");

    start_color();
    init_all_color_pairs();
    log(DEV_LOG_FILE, "initialized color");


    if (sound) {
        // Initialize SDL (audio)
        // Mix_FreeChunk(PLAYER_FOOTSTEP_SOUND);
        // Mix_CloseAudio();
        // Mix_Quit();
        // SDL_Quit();
        SDL_Init(SDL_INIT_AUDIO);

        // Initialize SDL Mixer
        int mixerFlags = MIX_INIT_MP3 | MIX_INIT_OGG; // Flags for the audio formats you want to support
        int initResult = Mix_Init(mixerFlags);
        if ((initResult & mixerFlags) != mixerFlags) {
            // Handle initialization error
            // Example: cerr << "Failed to initialize SDL Mixer: " << Mix_GetError() << endl;
            return 1;
        }

        // Open the audio device
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            // Handle audio device opening error
            // Example: cerr << "Failed to open audio device: " << Mix_GetError() << endl;
            return 1;
        }

        PLAYER_FOOTSTEP_SOUND = Mix_LoadWAV("data/sound/footstep.wav");  // Replace "footstep.wav" with the path to your sound file
        if (!PLAYER_FOOTSTEP_SOUND) {
            // Handle sound loading error
            // Example: cerr << "Failed to load footstep sound: " << Mix_GetError() << endl;
            return 1;
        }
        log(DEV_LOG_FILE, "initialized audio");
    }

    curs_set(0);
    noecho();
    log(DEV_LOG_FILE, "initialized curses");



    log(DEV_LOG_FILE, "hit character select");
    // Player class selection menu
    int classChoice;

    while(true) {
        print_class_menu();

        classChoice = getch() - '0';
        if (classChoice >= 1 && classChoice <= 8)
            break;
    }

    vector<int> classStats = set_player_class(classChoice);
    int playerAttack = classStats[0];
    int playerDefense = classStats[1];
    int playerSpeed = classStats[2];


    log(DEV_LOG_FILE, "selected player class");

    PlayerStats playerStats = {0,"EMPTY_PLACEHOLDER",PLAYER_MAX_HP,
                               playerAttack, playerDefense, playerSpeed};
    playerStats.level = get_player_level(playerStats);
    playerStats.class_name = get_player_class_name(playerStats);

    int seed = get_random_int(0, 255);
    log(DEV_LOG_FILE, "generated seed ", seed, " for rngesus");
    perlin.SetSeed(seed);
    log(DEV_LOG_FILE, "rngesus gives us his blessing");

    vector<short> palette = generate_palette(seed);
    log(DEV_LOG_FILE, "generated palette");


    vector<vector<Tile>> game_map = generate_game_map();
    //TODO: spawn player in sensible location
    int player_x = get_random_int(1, WIDTH-1);
    int player_y = get_random_int(1, HEIGHT-1);

    while (!check_if_in(GROUND_TILES,game_map[player_x][player_y].ch)) {
        player_x = get_random_int(1, WIDTH-1);
        player_y = get_random_int(1, HEIGHT-1);
    }
    log(DEV_LOG_FILE, "generated game_map");


    EntityManager entityManager;
    log(DEV_LOG_FILE, "generated entity manager");



    Entity playerEntity = entityManager.createEntity();
    entityManager.getPositions()[playerEntity] = {player_x, player_y};
    entityManager.getPlayerStats()[playerEntity] = playerStats;
    log(DEV_LOG_FILE, "initialized entity manager");

    int item_count = 0.7 * HEIGHT * WIDTH / ((HEIGHT+WIDTH)/2);
    int monster_count = 1.25 * HEIGHT * WIDTH / ((HEIGHT+WIDTH)/2);

    spawn_items(item_count ,entityManager, game_map);
    spawn_monsters(monster_count, entityManager, game_map);
    MonsterSystem monsterSystem(entityManager, game_map, playerStats);
    log(DEV_LOG_FILE, "spawned items and monsters");

    nodelay(stdscr, TRUE);
    auto timePerFrame = chrono::milliseconds(1000 / FPS); // Approximately 15 FPS

    float sprinting = 0.0f;
    set<pair<int, int>> player_fov = {};
    log(DEV_LOG_FILE, "starting game loop");
    while (true) {

        auto frameStart = chrono::steady_clock::now();


        int start_x = max(0, player_x - COLS / 2);
        start_x = min(start_x, WIDTH - COLS);
        int end_x = start_x + COLS;
        int start_y = max(0, player_y - LINES / 2);
        start_y = min(start_y, HEIGHT - LINES);
        int end_y = start_y + LINES;

        // get player fov
        set<pair<int, int>> player_fov = calculate_fov(
            game_map, player_x, player_y, PLAYER_FOV_RADIUS * playerStats.speed
        );
        // // Set player fov to visible and visited
        // for ( pair<int,int> coords : player_fov) {
        //     game_map[coords.first][coords.second].visible = true;
        //     game_map[coords.first][coords.second].visited = true;
        // }


        // initialize empty draw buffer
        vector<pair<char, pair<int, int>>> draw_buffer;

        for (int y = start_y; y < end_y; ++y) {
            for (int x = start_x; x < end_x; ++x) {
                if (x < 0 || x >= WIDTH)     continue;
                if (y < 0 || y >= HEIGHT)    continue;
                
                char ch = game_map[x][y].ch;
                draw_buffer.push_back(make_pair(ch, make_pair(y, x)));

                if (is_in_set({x, y}, player_fov)) {
                    game_map[x][y].visible = true;
                    game_map[x][y].visited = true;

                }

            }
        }
        
        draw_buffer.push_back(make_pair(PLAYER_TILE, make_pair(player_y, player_x)));

        auto& positions = entityManager.getPositions();
        auto& monsters = entityManager.getMonsterComponents();
        auto& items = entityManager.getItemComponents();

        for (const auto& entry : positions) {
            Entity entity = entry.first;
            if (monsters.find(entity) != monsters.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, player_fov)) {
                    char entity_tile;
                    if (monsters[entity].health > 20) {
                        entity_tile = 'M';
                    } else if (monsters[entity].health > 10) {
                        entity_tile = 'm';
                    } else if (monsters[entity].health > 5) {
                        entity_tile = 'n';
                    } else {
                        entity_tile = 'r';
                    }
                    draw_buffer.push_back(make_pair(entity_tile, make_pair(position.y, position.x)));
                }
            } else if (items.find(entity) != items.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, player_fov)) {
                    char entity_tile = items[entity].character;
                    draw_buffer.push_back(make_pair(entity_tile, make_pair(position.y, position.x)));
                }
            }
        }


        palette[3] = get_player_color(playerStats);

        render_buffer(draw_buffer, game_map, palette, start_x, start_y);

        refresh();



        //clear fov from map tiles
        for (pair<int, int> fov_coords : player_fov ) {
            game_map[fov_coords.first][fov_coords.second].visible = false;
        }

        entityManager.getPlayerStats()[playerEntity] = playerStats;
        int memory_radius = pow(PLAYER_FOV_RADIUS, 3) * playerStats.defense   ;

        // Forget distant visited tiles
        for (int i = 0; i < 360; ++i) {
            double rad = i * (M_PI / 180.0);
            double dx = cos(rad), dy = sin(rad) / 2;
            double x = player_x, y = player_y;

            x += dx * memory_radius;
            y += dy * memory_radius;
            int ix = round(x), iy = round(y);
            ix = max(1,min(WIDTH-1,ix));
            iy = max(1,min(HEIGHT-1,iy));
            game_map[ix][iy].visited = false;

        }

        int player_level = get_player_level(playerStats);
        if ( player_level > playerStats.level) {// player just leveled up
            add_combat_log("Leveled up!");
            playerStats.level = player_level;
            playerStats.class_name = get_player_class_name(playerStats);
            entityManager.getPlayerStats()[playerEntity] = playerStats;

        }

        draw_UI(playerStats);
        // mvprintw(0,0,"(x%d,y%d)",player_x,player_y);


        int key = getch();
        if (key != ERR) {
            pair<int,int> delta = {0,0};
            if (key=='w' || key=='a' || key=='s' || key=='d') {
                pair<int,int> direction;
                if (key=='w')
                    direction = {0,-1};
                else if (key=='a')
                    direction = {-1,0};
                else if (key=='s')
                    direction = {0,1};
                else if (key=='d')
                    direction = {1,0};
                delta = move_player(entityManager, playerStats, game_map, direction, 0);
                //forgive me
                bool occupied = false;
                for (const auto& entry : monsters) {
                    Entity monsterEntity = entry.first;
                    PositionComponent& monsterPosition = positions[monsterEntity];

                    if (monsterPosition.x == delta.first
                        && monsterPosition.y == delta.second) {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied){
                    player_x += delta.first;
                    player_y += delta.second;
                }

            } else if (key=='W' || key=='A' || key=='S' || key=='D') {
                pair<int,int> direction;
                if (key=='W')
                    direction = {0,-1};
                else if (key=='A')
                    direction = {-1,0};
                else if (key=='S')
                    direction = {0,1};
                else if (key=='D')
                    direction = {1,0};
                sprinting += 0.2f; // This is your acceleration rate, adjust as needed
                if (sprinting > playerStats.speed) sprinting = playerStats.speed; // Cap the sprinting speed
                delta = move_player(entityManager, playerStats, game_map, direction, sprinting);
                //forgive me
                bool occupied = false;
                for (const auto& entry : monsters) {
                    Entity monsterEntity = entry.first;
                    PositionComponent& monsterPosition = positions[monsterEntity];

                    if (monsterPosition.x == delta.first
                        && monsterPosition.y == delta.second) {
                        occupied = true;
                        break;
                    }
                }
                if (!occupied){
                    player_x += delta.first;
                    player_y += delta.second;
                }
            } else if (key == 'q') {
                break;
            } else if (key == ' ') {
                // Player attack
                auto& playerPosition = positions[playerEntity];
                auto& monsters = entityManager.getMonsterComponents();

                for (const auto& entry : monsters) {
                    Entity monsterEntity = entry.first;
                    PositionComponent& monsterPosition = positions[monsterEntity];
                    MonsterComponent& monster = monsters[monsterEntity];

                    if (abs(playerPosition.x - monsterPosition.x) <= playerStats.speed
                    && abs(playerPosition.y - monsterPosition.y) <= playerStats.speed
                    && is_in_set({monsterPosition.x, monsterPosition.y}, player_fov) ) {
                        int damage = get_random_int(0, playerStats.attack);
                        monster.health -= damage;

                        if (damage > 0) {
                            add_combat_log("Player dealt " + to_string(damage) + " damage to a monster.");
                        } else {
                            add_combat_log("Player's attack was blocked by a monster.");
                        }

                        if (monster.health <= 0) {
                            add_combat_log("Player has defeated a monster.");
                            entityManager.destroyEntity(monsterEntity);
                        }
                        break;
                    }
                }
            } else if (key == 'e' ) {
                //player interact
                auto& playerPosition = positions[playerEntity];
                auto& items = entityManager.getItemComponents();

                for (const auto& entry : items) {
                    Entity itemEntity = entry.first;
                    PositionComponent& itemPosition = positions[itemEntity];
                    ItemComponent& item = items[itemEntity];
                    if (playerPosition.x == itemPosition.x && playerPosition.y == itemPosition.y) {
                        item.effect(playerStats);
                        entityManager.destroyEntity(itemEntity);
                        add_combat_log("Picked up " + item.name);
                        break;
                    }
                }
            } 
        } else {
            sprinting -= 0.1f;
            if (sprinting < 0.0f) sprinting = 0.0f;
        }




        // movementSystem.update();
        // log(DEV_LOG_FILE, "updated movement system");

        monsterSystem.update();
        log(DEV_LOG_FILE, "updated monster system");

        entityManager.getPositions()[playerEntity] = {player_x, player_y};


        auto frameEnd = chrono::steady_clock::now();
        auto frameDuration = chrono::duration_cast<chrono::milliseconds>(frameEnd - frameStart);

        // If the frame finished faster than the time per frame, sleep for the remaining time
        if (frameDuration < timePerFrame) {
            this_thread::sleep_for(timePerFrame - frameDuration);
        }

        //clear draw_buffer
        draw_buffer.clear();
        clear();



        if (playerStats.health <= 0) {
            add_combat_log("Game Over. Player has died.");
            endwin();
            printf("The monster has won...");
            return 0;
        }



    }

    if (sound) {
        //release audio
        Mix_FreeChunk(PLAYER_FOOTSTEP_SOUND);
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
    }
    //release screen
    endwin();
    return 0;
}
