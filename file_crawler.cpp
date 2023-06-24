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
#include <locale.h>

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
// v0.8x



/* ___             _            _
  / __|___ _ _  __| |_ __ _ _ _| |_ ___
 | (__/ _ \ " \(_-<  _/ _` | " \  _(_-<
  \___\___/_||_/__/\__\__,_|_||_\__/__/
 section:constants */

const wstring DEV_LOG_FILE = "dev_log.txt";
const wstring GAME_LOG_FILE = "game_log.txt";

const int UI_LOG_COUNT = 7;

const int FPS = 15;
const int WIDTH = 1000, HEIGHT = 1000;
const int PLAYER_MAX_HP = 100;
const int PLAYER_MAX_EP = 150;
const int PLAYER_FOV_RADIUS = 12;
const int PLAYER_MEMORY_RADIUS = 3*PLAYER_FOV_RADIUS;

const bool sound = false;
Mix_Chunk* PLAYER_FOOTSTEP_SOUND = nullptr;
const int PLAYER_FOOTSTEP_SOUND_COUNT = 6;

const vector<string> GROUND_TILES = {".",",","`","\"","'"};
const vector<string> WALL_TILES = {"[","]","{","}","=","|","+","-","_","/","\\",};
const vector<string> TRAP_TILES = {"&","#"};
const vector<string> VOID_TILES = {" "};


const wstring PLAYER_TILE = "ç";


// const vector<string> MONSTER_TILES = {
//     "Z","S",
// };
// const vector<string> ITEM_TILES = {
//     "o","O","0","s","S","7","d","D","8","a","A","9"
// };

// vector<string> get_entity_tiles() {
//     vector<string> tiles;
//     tiles.insert(tiles.end(), MONSTER_TILES.begin(), MONSTER_TILES.end());
//     tiles.insert(tiles.end(), ITEM_TILES.begin(), ITEM_TILES.end());
//     tiles.push_back(PLAYER_TILE);
//     return tiles;
// }
// vector<string> ENTITY_TILES = get_entity_tiles();


/*_                   _
 | |   ___  __ _ __ _(_)_ _  __ _
 | |__/ _ \/ _` / _` | | " \/ _` |
 |____\___/\__, \__, |_|_||_\__, |
           |___/|___/       |___/
 section:logging */

vector<string> combat_log;

template<typename... Args>
void log(const wstring filename, Args&&... args) {
    ofstream logfile(filename, ios::app);  // Open the file in append mode

    if (!logfile.is_open()) return;

    // Redirect cout to the log file
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(logfile.rdbuf());

    // Create a wstring stream
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


/*  _          _ _
   /_\ _  _ __| (_)___
  / _ \ || / _` | / _ \
 /_/ \_\_,_\__,_|_\___/
 section:audio */

void playFootstepSound() {

    // Generate a random number between 1 and maximum sound file index
    pair<int,int> range = { 1, PLAYER_FOOTSTEP_SOUND_COUNT };

    // Construct the file name with the randomly generated number
    wstring file_id = to_string(get_random_int(range.first,range.second));
    wstring fileName = "data/sound/footstep"+file_id+".wav";

    // Load the random sound file
    Mix_Chunk* footstepSound = Mix_LoadWAV(fileName.c_wstr());
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


/* ___               _    _
  / __|_ _ __ _ _ __| |_ (_)__ ___
 | (_ | "_/ _` | "_ \ " \| / _(_-<
  \___|_| \__,_| .__/_||_|_\__/__/
 section:graphics */

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
        {2,4},   //red, yellow
        {12,3}   //brown, green
    };
    return options[get_random_int(0,options.size() - 1)];
}

vector<int> get_trap_swatch() {
    vector<vector<int>> options= {
        {5,7},   //blue, cyan
        {2,1},   //red, black
        {9,6}    //orange, magenta
    };
    return options[get_random_int(0,options.size() - 1)];
}

vector<int> get_wall_swatch() {
    vector<vector<int>> options= {
        {5,7},   //blue, cyan
        {8,12},  //white, grey
        {1,11},  //black, brown
        {4,10}   //yellow, purple
    };
    return options[get_random_int(0,options.size() - 1)];
}

int get_tile_color(const vector<int> swatch) {
    return swatch[get_random_int(0,swatch.size() - 1)];
}


/* ___                       _   _
  / __|___ _ _  ___ _ _ __ _| |_(_)___ _ _
 | (_ / -_) " \/ -_) "_/ _` |  _| / _ \ " \
  \___\___|_||_\___|_| \__,_|\__|_\___/_||_|
 section:generation */

// MAP GEN
struct Tile {
    wstring ch;
    short color;
    bool visited;
    bool visible;
};

bool is_a_good_map(const vector<vector<Tile>> potential_map) {
    bool looks_good, verified;

    //double-check all the basic shit
    if (!(potential_map.size() == WIDTH || potential_map[0].size() == HEIGHT)) {
        looks_good = false;
    } else {
        looks_good = true;
    }

    if (looks_good) {
        int total_tile_count, ground_tile_count = 0;
        for (vector<Tile> column : potential_map) {
            for (Tile tile: column) {
                total_tile_count++;
                if (check_if_in(GROUND_TILES, tile.ch)) ground_tile_count++;
            }
        }

        float open_space_ratio = 1.0f * ((float)ground_tile_count / total_tile_count);
        log(DEV_LOG_FILE, "this attempt has ", ground_tile_count,"/",total_tile_count,"(",open_space_ratio, ") air");
        if (open_space_ratio<0.65f) {
            verified = false;
        } else {
            verified = true;
        }
    }

    return verified;
}


vector<vector<Tile>> generate_game_map() {
    log(DEV_LOG_FILE, "called generate_game_map");
    vector<vector<Tile>> game_map(WIDTH, vector<Tile>(HEIGHT));

    vector<int> ground_swatch = get_ground_swatch();
    vector<int> trap_swatch = get_trap_swatch();
    vector<int> wall_swatch = get_wall_swatch();


    // Generate open areas using Perlin noise
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            wstring ch = get_random_character(VOID_TILES);
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
                wstring ch = get_random_character(WALL_TILES);
                short color = get_tile_color(wall_swatch);
                Tile this_tile = {ch, color, false, false};
                game_map[x][y] =  this_tile;
            }
        }
    }

    // Draw a border around the game_map
    for (int x = 0; x < WIDTH; ++x) {
        wstring ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false};
        game_map[x][0] = this_tile;
        game_map[x][HEIGHT - 1] = this_tile;
    }
    for (int y = 0; y < HEIGHT; ++y) {
        wstring ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false};
        game_map[0][y] = this_tile;
        game_map[WIDTH - 1][y] = this_tile;
    }


    return game_map;
}


/*
vector<vector<Tile>> generate_game_map() {
    log(DEV_LOG_FILE, "called generate_game_map");
    int i = 1;





    bool made = false;

    vector<vector<Tile>> game_map(WIDTH, vector<Tile>(HEIGHT));

    while (!made) {

        log(DEV_LOG_FILE, "attempt ", i);
        vector<vector<Tile>> game_map(WIDTH, vector<Tile>(HEIGHT));


        vector<int> ground_swatch = get_ground_swatch();
        vector<int> trap_swatch = get_trap_swatch();
        vector<int> wall_swatch = get_wall_swatch();


        // Generate open areas using Perlin noise
        for (int y = 0; y < HEIGHT; ++y) {
            for (int x = 0; x < WIDTH; ++x) {
                wstring ch = get_random_character(VOID_TILES);
                short color = get_color_pair_index(COLOR_WHITE,COLOR_BLACK);
                Tile void_tile = {ch, color, false, false};
                game_map[x][y] = void_tile;
                double value = perlin.GetValue(x / 10.0, y / 10.0, 0.1);


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
                    wstring ch = get_random_character(WALL_TILES);
                    short color = get_tile_color(wall_swatch);
                    Tile this_tile = {ch, color, false, false};
                    game_map[x][y] =  this_tile;
                }
            }
        }

        // Draw a border around the game_map
        for (int x = 0; x < WIDTH; ++x) {
            wstring ch = get_random_character(WALL_TILES);
            short color = get_tile_color(wall_swatch);
            Tile this_tile = {ch, color, false, false};
            game_map[x][0] = this_tile;
            game_map[x][HEIGHT - 1] = this_tile;
        }
        for (int y = 0; y < HEIGHT; ++y) {
            wstring ch = get_random_character(WALL_TILES);
            short color = get_tile_color(wall_swatch);
            Tile this_tile = {ch, color, false, false};
            game_map[0][y] = this_tile;
            game_map[WIDTH - 1][y] = this_tile;
        }
        if (is_a_good_map(game_map)) {
            made = true;
        }

        i++;

    }

    return game_map;
}*/


/*___      _   _     __ _         _ _
 | _ \__ _| |_| |_  / _(_)_ _  __| (_)_ _  __ _
 |  _/ _` |  _| " \|  _| | " \/ _` | | " \/ _` |
 |_| \__,_|\__|_||_|_| |_|_||_\__,_|_|_||_\__, |
                                          |___/
 section:pathfinding */

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

        for (int j = 0; j <= radius; ++j) {
            int ix = round(x), iy = round(y);

            if (ix  < 0 || iy < 0 || ix >= WIDTH || iy >= HEIGHT)
                break;

            // check if we should insert this tile into fov
            if (j == 0 || i % j == 0) {
                fov.insert({ix, iy});
            }

            if (check_if_in(WALL_TILES, game_map[ix][iy].ch))
                break;

            x += dx;
            y += dy;
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



/* ___                                  _
  / __|___ _ __  _ __  ___ _ _  ___ _ _| |_ ___
 | (__/ _ \ "  \| "_ \/ _ \ " \/ -_) " \  _(_-<
  \___\___/_|_|_| .__/\___/_||_\___|_||_\__/__/
                |_|
 section:components */

struct PlayerStats {
    int level;
    wstring class_name;
    int color;
    int health;
    int energy;
    int attack;
    int defense;
    int speed;
};


enum class ItemType {
    SmallHealthPotion,
    MediumHealthPotion,
    LargeHealthPotion,
    SmallEnergyPotion,
    MediumEnergyPotion,
    LargeEnergyPotion,
    LightRegenPotion,
    FullRegenPotion,
    WeakStrengthSerum,
    StrongStrengthSerum,
    WeakWillSerum,
    StrongWillSerum,
    WeakDexteritySerum,
    StrongDexteritySerum
};

struct ItemComponent {
    ItemType type;
    wstring character;
    wstring name;
    int color;
    float rarity;
    function<void(PlayerStats&)> effect;
};

struct MonsterComponent {
    wstring characterFullHealth;
    wstring characterMidHealth;
    wstring characterLowHealth;
    wstring name;
    int color;
    int attackPower;
    int attackRadius;
    int chaseRadius;
    int cooldown;
    int total_health;
    int health;
};


/*___     _   _ _   _
 | __|_ _| |_(_) |_(_)___ ___
 | _|| " \  _| |  _| / -_|_-<
 |___|_||_\__|_|\__|_\___/__/
 section:entities */


using Entity = int;

class EntityManager {
private:
    vector<Entity> entities;
    unordered_map<Entity, PositionComponent> positions;
    unordered_map<Entity, PlayerStats> playerStats;
    // unordered_map<Entity, AIComponent> ais;
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
            // ais.erase(entity);
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

    // unordered_map<Entity, AIComponent>& getAIComponents() {
        // return ais;
    // }

    unordered_map<Entity, MonsterComponent>& getMonsterComponents() {
        return monsters;
    }

    unordered_map<Entity, ItemComponent>& getItemComponents() {
        return items;
    }
};


// subsection:player

int get_player_level(PlayerStats& playerStats) {
    return round((playerStats.attack
                  + playerStats.defense
                  + playerStats.speed)
                 /(9+playerStats.level));
}

wstring get_player_class_name(PlayerStats& playerStats) {
    wstring class_name;

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


int get_player_color(PlayerStats& playerStats) {
    int color;

    if (playerStats.class_name == "Fighter") {
        color = COLOR_RED;
    } else if (playerStats.class_name == "Rogue") {
        color = COLOR_YELLOW;
    } else if (playerStats.class_name == "Ranger") {
        color = COLOR_GREEN;
    } else if (playerStats.class_name == "Sorcerer") {
        color = COLOR_CYAN;
    } else if (playerStats.class_name == "Wizard") {
        color = COLOR_BLUE;
    } else if (playerStats.class_name == "Cleric") {
        color = COLOR_MAGENTA;
    } else if (playerStats.class_name == "Adventurer") {
        color = COLOR_WHITE;
    }

    return color;
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

// subsection:items

// enum class ItemType {
//     SmallHealthPotion,
//     MediumHealthPotion,
//     LargeHealthPotion,
//     SmallEnergyPotion,
//     MediumEnergyPotion,
//     LargeEnergyPotion,
//     LightRegenPotion,
//     FullRegenPotion,
//     WeakStrengthSerum,
//     StrongStrengthSerum,
//     WeakWillSerum,
//     StrongWillSerum,
//     WeakDexteritySerum,
//     StrongDexteritySerum
// };


// struct ItemComponent {
//     ItemType type;
//     wstring character;
//     wstring name;
//     int color;
//     float rarity;
//     function<void(PlayerStats&)> effect;
// };


// we can def load this from a file or smthn
vector<ItemComponent> itemTemplates = {
        {ItemType::SmallHealthPotion, //< identifier
            "o", "Small Health Potion", COLOR_RED, 0.5, //< character, name, color, rarity
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP, stats.health + 5);}}, //< ability

        {ItemType::MediumHealthPotion,
            "O", "Medium Health Potion", COLOR_RED,0.1,
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 10);}},

        {ItemType::LargeHealthPotion,
            "0", "Large Health Potion",  COLOR_RED, 0.5,
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 25);}},

        {ItemType::SmallEnergyPotion,
            "o", "Small Energy Potion", COLOR_BLUE, 0.5,
            [](PlayerStats& stats) {stats.energy = min(PLAYER_MAX_EP, stats.energy + 5);}},

        {ItemType::MediumEnergyPotion,
            "O", "Medium Energy Potion", COLOR_BLUE,0.1,
            [](PlayerStats& stats) {stats.energy = min(PLAYER_MAX_EP,stats.energy + 10);}},

        {ItemType::LargeEnergyPotion,
            "0", "Large Energy Potion",  COLOR_BLUE, 0.05,
            [](PlayerStats& stats) {stats.energy = min(PLAYER_MAX_EP,stats.energy + 25);}},

        {ItemType::LightRegenPotion,
            "O", "Light Regen Potion", COLOR_MAGENTA,0.05,
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + PLAYER_MAX_HP/4),
                                    stats.energy = min(PLAYER_MAX_EP,stats.energy + PLAYER_MAX_EP/4);}},

        {ItemType::FullRegenPotion,
            "O", "Full Regen Potion", COLOR_MAGENTA,0.025,
            [](PlayerStats& stats) {stats.health = PLAYER_MAX_HP,
                                    stats.energy = PLAYER_MAX_EP;}},

        {ItemType::WeakStrengthSerum,
            "s", "Weak Strength Serum", COLOR_RED, 1.0,
            [](PlayerStats& stats) { stats.attack += 1; }},

        {ItemType::StrongStrengthSerum,
            "S", "Strong Strength Serum", COLOR_RED, 0.25,
            [](PlayerStats& stats) { stats.attack += 2; }},

        {ItemType::WeakWillSerum,
            "w", "Weak Will Serum", COLOR_BLUE, 1.0,
            [](PlayerStats& stats) { stats.defense += 1; }},

        {ItemType::StrongWillSerum,
            "W", "Strong  Will Serum", COLOR_BLUE, 0.25,
            [](PlayerStats& stats) { stats.defense += 2; }},

        {ItemType::WeakDexteritySerum,
            "d", "Weak Dexterity Serum", COLOR_GREEN, 1.0,
            [](PlayerStats& stats) { stats.speed += 1; }},

        {ItemType::StrongDexteritySerum,
            "D", "Strong Dexterity Serum", COLOR_GREEN, 0.25,
            [](PlayerStats& stats) { stats.speed += 2; }}
    };


void spawn_items(int count, EntityManager& entityManager, vector<vector<Tile>>& game_map) {
    log(DEV_LOG_FILE, "spawning ", count, " items");

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

// subsection:monsters
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
            if (get_random_int(1,7)%3==0) {
                Entity entity = entry.first;

                // Skip non-monster entities
                if (monsters.find(entity) == monsters.end())
                    continue;

                PositionComponent& monsterPosition = positions[entity];
                PositionComponent& playerPosition = positions[0];
                // AIComponent& ai = entityManager.getAIComponents()[entity];
                MonsterComponent& monster = monsters[entity];

                monster.cooldown = max(0, monster.cooldown - 1);
                int dx = monsterPosition.x - playerPosition.x;
                int dy = monsterPosition.y - playerPosition.y;
                double distance = sqrt(dx * dx + dy * dy);

                if (distance <= monster.chaseRadius) {
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

                        entityManager.getPositions()[entity] = {monsterPosition.x, monsterPosition.y};
                    }

                    log(DEV_LOG_FILE, "checking monster", entity, "'s attack range");
                    set<pair<int, int>> monster_attack_range =
                        calculate_fov(
                            game_map,
                            monsterPosition.x, monsterPosition.y,
                            monster.attackRadius
                        );
                    log(DEV_LOG_FILE, "monster", entity, "'s attack range set");


                    if (distance <= monster.attackRadius && monster.cooldown == 0
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
    }
};



// subsection:spawning

// struct MonsterComponent {
//     wstring character;
//     wstring name;
//     int color;
//     int attackPower;
//     int attackRadius;
//     int chaseRadius;
//     int cooldown;
//     int total_health;
//     int health;
// };

void spawn_monsters(int count, float rating, EntityManager& entityManager, vector<vector<Tile>>& game_map) {
    log(DEV_LOG_FILE, "spawning monsters");
    for (int i = 0; i < count; ++i) {
        int x, y;
        do {
            x = get_random_int(0, WIDTH - 1);
            y = get_random_int(0, HEIGHT - 1);
        } while (!check_if_in(GROUND_TILES, game_map[x][y].ch));
        Entity monsterEntity = entityManager.createEntity();
        entityManager.getPositions()[monsterEntity] = {x, y};
        
        pair<int, int> attack_power_dis = {rating*1, rating*20};
        pair<int, int> attack_range_dis = {rating*1, rating*7};
        pair<int, int> chase_dis = {rating*7, rating*25};
        pair<int, int> attack_cooldown_dis = {rating*1, rating*7};
        pair<int, int> health_dis = {rating*5, rating*25};

        const int monster_attack_power = get_random_int(attack_power_dis.first,attack_power_dis.second);
        const int monster_attack_range = get_random_int(attack_range_dis.first,attack_range_dis.second);
        const int monster_chase_range = get_random_int(chase_dis.first,chase_dis.second);
        const int monster_attack_cooldown = FPS*get_random_int(attack_cooldown_dis.first,attack_cooldown_dis.second);
        const int monster_health = get_random_int(health_dis.first,health_dis.second);



        /*
        entityManager.getAIComponents()[monsterEntity] = {
            monster_chase_range,
            monster_attack_range,
            monster_attack_cooldown
        };*/
        entityManager.getMonsterComponents()[monsterEntity] = {
            "M", "m", ";", "Monster", COLOR_RED,
            monster_attack_power,monster_attack_range, monster_chase_range, monster_attack_cooldown,
            monster_health, monster_health
        };
    }
}



/*___             _         _
 | _ \___ _ _  __| |___ _ _(_)_ _  __ _
 |   / -_) " \/ _` / -_) "_| | " \/ _` |
 |_|_\___|_||_\__,_\___|_| |_|_||_\__, |
                                  |___/
 section:rendering */

void render_buffer(
        const vector<pair<string, pair<int, int>>>& draw_buffer,
        const vector<vector<Tile>>& game_map,
        const vector<pair<pair<int, int>, pair<string, int>>> entities,
        int start_x, int start_y) {

    log(DEV_LOG_FILE, "rendering buffer");

    int py, px;
    short player_color;
    vector<vector<pair<string, int>>> frame(LINES-3, vector<pair<string, int>>(COLS, {"?",COLOR_BLACK}));


    for (const auto& entity: entities) {
        int y = entity.first.first - start_y;
        int x = entity.first.second - start_x;
        wstring ch = entity.second.first;
        short color = get_color_pair_index(COLOR_BLACK,entity.second.second);
        if (ch == PLAYER_TILE) py = y, px = x, player_color = color;
        if (x < 0 || x >= COLS || y < 0 || y >= LINES-3) continue;
        frame[y][x].first = ch, frame[y][x].second = color;
    }

    for (const auto& tile : draw_buffer) {
        int y = tile.second.first;
        int x = tile.second.second;
        int Y = y - start_y;
        int X = x - start_x;

        wstring ch = tile.first;
        short color = get_color_pair_index(COLOR_BLACK,COLOR_BLACK);

        if (game_map[x][y].visible) { // visible environment
            color = game_map[x][y].color;
        } else if (game_map[x][y].visited) { // memory
            color = get_color_pair_index(8, COLOR_BLACK);
        }

        if (X < 0 || X >= COLS || Y < 0 || Y >= LINES-3) continue;
        if (frame[Y][X].first == "?") frame[Y][X] = make_pair(ch, color);
    }

    //draw player last
    frame[py][px] = make_pair(PLAYER_TILE, player_color);


    // Render frame
    clear();
    wstring pixel_char;
    short pixel_color;
    for (int i = 0; i < LINES-3; ++i) {
        for (int j = 0; j < COLS; ++j) {
            pixel_char = frame[i][j].first;
            pixel_color = frame[i][j].second;
            attron(COLOR_PAIR(pixel_color));
            mvaddwstr(i,j, pixel_char.c_wstr());
            attroff(COLOR_PAIR(pixel_color));
        }
    }

    // Actually render
    doupdate();
}



/*___     _            __
 |_ _|_ _| |_ ___ _ _ / _|__ _ __ ___
  | || " \  _/ -_) "_|  _/ _` / _/ -_)
 |___|_||_\__\___|_| |_| \__,_\__\___|
 section:interface */


void draw_UI(PlayerStats& playerStats) {

    int orb_diameter = max(6,LINES*2/6);//it's actually half this value on the y, takes 1/6th of the screen, smollest orb size of 3x7
    int orb_size = (orb_diameter*orb_diameter)/2;
    int health_level = static_cast<int>(static_cast<float>(playerStats.health) / PLAYER_MAX_HP * orb_size);
    int energy_level = static_cast<int>(static_cast<float>(playerStats.energy) / PLAYER_MAX_EP * orb_size);
    int x = 2;
    int y = LINES - (orb_diameter/2) - 2;
    int player_level = playerStats.level;
    int player_color = playerStats.color;
    wstring player_class = playerStats.class_name;

    //draw background for hud
    int ui_bg = get_color_pair_index(8,8);




    attron(COLOR_PAIR(ui_bg));
    for (int hy = 0; hy <= 3; ++hy) {
        for (int hx = 0; hx < COLS; ++hx) {
            mvaddwstr(LINES-hy,hx,".");
        }
    }
    attroff(COLOR_PAIR(ui_bg));

    //draw health orb
    for (int oy = 0; oy < (orb_diameter/2) + 2; ++oy) {
        for(int ox = 0; ox < orb_diameter + 2; ++ox) {

            //draw box in center
            if (   (ox != 0 && oy!=0)
                && (ox!=orb_diameter+1 && oy!=0)
                && (ox != 0 && oy!=orb_diameter/2+1)
                && (ox!=orb_diameter+1 && oy!=orb_diameter/2+1)){
                int color_pair;
                wstring ch;
                    if (health_level >= orb_size - (((oy) * orb_diameter) - ox)) {
                        ch = get_random_character({" "," "," ","~"});
                        color_pair = get_color_pair_index(COLOR_BLACK,COLOR_RED);
                    } else{
                        ch =get_random_character({" "," "," "," "," "," "," "," ","."});
                        color_pair = get_color_pair_index(8,COLOR_BLACK);
                    }

                attron(COLOR_PAIR(color_pair));
                mvaddwstr(y+oy,1+ox,ch.c_wstr());
                attroff(COLOR_PAIR(color_pair));
            }
            //draw boundary
            if (   (oy==0 || oy==orb_diameter/2+1) && !(ox==0 || ox==orb_diameter+1)
                || (ox==0 || ox==orb_diameter+1) && !(oy==0 || oy==orb_diameter/2+1)
                || (ox==1 || ox==orb_diameter) && (oy==1 || oy==orb_diameter/2)) {
                int orb_boundary_color = get_color_pair_index(7,8);
                wstring orb_boundary_ch = " ";
                if ((ox==1 && oy==1) || (ox==orb_diameter && oy==orb_diameter/2)) {
                    orb_boundary_ch = "/";
                } else if ((ox==1 && oy==orb_diameter/2) || (ox==orb_diameter && oy==1)) {
                    orb_boundary_ch = "\\";
                } else if ((ox==0 || ox==orb_diameter+1) && (oy>1 && oy<orb_diameter/2)){
                    orb_boundary_ch = "|";
                } else if ((oy==0 || oy==orb_diameter/2+1) && (ox>1 && ox<orb_diameter)){
                    orb_boundary_ch = "-";
                }

                attron(COLOR_PAIR(orb_boundary_color));
                mvaddwstr(y+oy,1+ox,orb_boundary_ch.c_wstr());
                attroff(COLOR_PAIR(orb_boundary_color));


            }
        }
    }


    //draw energy orb
    for (int oy = 0; oy < (orb_diameter/2) + 2; ++oy) {
        for(int ox = 0; ox < orb_diameter + 2; ++ox) {

            //draw box in center
            if (   (ox != 0 && oy!=0)
                && (ox!=orb_diameter+1 && oy!=0)
                && (ox != 0 && oy!=orb_diameter/2+1)
                && (ox!=orb_diameter+1 && oy!=orb_diameter/2+1)){
                int color_pair;
                wstring ch;
                    if (energy_level >= orb_size - (((oy) * orb_diameter) - ox)) {
                        ch = get_random_character({" "," "," ","~"});
                        color_pair = get_color_pair_index(COLOR_BLACK,get_player_color(playerStats));
                    } else{
                        ch =get_random_character({" "," "," "," "," "," "," "," ","."});
                        color_pair = get_color_pair_index(8,COLOR_BLACK);
                    }

                attron(COLOR_PAIR(color_pair));
                mvaddwstr(y+oy,COLS-ox-2,ch.c_wstr());
                attroff(COLOR_PAIR(color_pair));
            }
            //draw boundary
            if (   (oy==0 || oy==orb_diameter/2+1) && !(ox==0 || ox==orb_diameter+1)
                || (ox==0 || ox==orb_diameter+1) && !(oy==0 || oy==orb_diameter/2+1)
                || (ox==1 || ox==orb_diameter) && (oy==1 || oy==orb_diameter/2)) {
                int orb_boundary_color = get_color_pair_index(7,8);
                wstring orb_boundary_ch = " ";
                if ((ox==1 && oy==1) || (ox==orb_diameter && oy==orb_diameter/2)) {
                    orb_boundary_ch = "\\";
                } else if ((ox==1 && oy==orb_diameter/2) || (ox==orb_diameter && oy==1)) {
                    orb_boundary_ch = "/";
                } else if ((ox==0 || ox==orb_diameter+1) && (oy>1 && oy<orb_diameter/2)){
                    orb_boundary_ch = "|";
                } else if ((oy==0 || oy==orb_diameter/2+1) && (ox>1 && ox<orb_diameter)){
                    orb_boundary_ch = "-";
                }

                attron(COLOR_PAIR(orb_boundary_color));
                mvaddwstr(y+oy,COLS-ox-2,orb_boundary_ch.c_wstr());
                attroff(COLOR_PAIR(orb_boundary_color));


            }
        }
    }

    x = orb_diameter + 5;


    attron(COLOR_PAIR(get_color_pair_index(COLOR_BLACK,player_color)));

    //draw level and class name
    mvprintw(LINES-3, x, "%s",
             player_class.c_wstr()
            );
    mvprintw(LINES-2, x, "LVL%d",
             player_level
            );
    attroff(COLOR_PAIR(get_color_pair_index(COLOR_BLACK,player_color)));

    //draw stats
    mvprintw(LINES-1, x, "STR:%d WIL:%d DEX:%d",
             playerStats.attack,
             playerStats.defense,
             playerStats.speed
            );

    y = 1;
    x = 2;

    //draw combat log
    int log_size = min(LINES - orb_diameter - 2, static_cast<int>(combat_log.size()));


    for (int i = 0; i < log_size; i++) {
        mvaddwstr(y + i, x, combat_log[combat_log.size() - log_size + i].c_wstr());
    }


}



/*__  __
 |  \/  |___ _ _ _  _ ___
 | |\/| / -_) " \ || (_-<
 |_|  |_\___|_||_\_,_/__/
 section:menus */
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

pair<int, int> get_player_spawn_coords(vector<vector<Tile>> game_map, EntityManager& entityManager) {
    int x = get_random_int(1, WIDTH-1), y = get_random_int(1, HEIGHT-1);
    int no_monster_radius = 15, no_monster_min = 1;

    auto& positions = entityManager.getPositions();
    auto& monsters = entityManager.getMonsterComponents();
    int nearby_monster_count = 0;
    for (auto& pos: positions) if ((sqrt(pow(abs(pos.second.x-x),2)+pow(abs(pos.second.y-y),2)) <= no_monster_radius)
                                    && (monsters.find(pos.first) != monsters.end())) ++nearby_monster_count;
                                    //no-spawn radius from player

    //TODO: spawn player in sensible location
    while (!check_if_in(GROUND_TILES,game_map[x][y].ch)
           || nearby_monster_count > no_monster_min ) {
        x = get_random_int(1, WIDTH-1), y = get_random_int(1, HEIGHT-1);
        int nearby_monster_count = 0;
        for (auto& pos: positions) if ((sqrt(pow(abs(pos.second.x-x),2)+pow(abs(pos.second.y-y),2)) <= no_monster_radius)
                                       && (monsters.find(pos.first) != monsters.end())) ++nearby_monster_count;
                                        //no-spawn radius from player
    }


    return make_pair(x, y);
}



/*__  __   _   ___ _  _    ___   _   __  __ ___   _    ___   ___  ___
 |  \/  | /_\ |_ _| \| |  / __| /_\ |  \/  | __| | |  / _ \ / _ \| _ \
 | |\/| |/ _ \ | || .` | | (_ |/ _ \| |\/| | _|  | |_| (_) | (_) |  _/
 |_|  |_/_/ \_\___|_|\_|  \___/_/ \_\_|  |_|___| |____\___/ \___/|_|
 section:main game loop */


int main() {
    log(DEV_LOG_FILE, "Running file_crawler");
    
    setlocale(LC_ALL, "");
    
    initscr();
    log(DEV_LOG_FILE, "initialized screen");
    int frame_count = 0;

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

    PlayerStats playerStats = {0,"Deprived",0,
                               PLAYER_MAX_HP,PLAYER_MAX_EP,
                               playerAttack, playerDefense, playerSpeed};
    playerStats.level = get_player_level(playerStats);
    playerStats.class_name = get_player_class_name(playerStats);
    playerStats.color = get_player_color(playerStats);

    int seed = get_random_int(0, 255);
    log(DEV_LOG_FILE, "generated seed ", seed, " for rngesus");
    perlin.SetSeed(seed);
    log(DEV_LOG_FILE, "rngesus gives us his blessing");

    vector<short> palette = generate_palette(seed);
    log(DEV_LOG_FILE, "generated palette");


    vector<vector<Tile>> game_map = generate_game_map();
    log(DEV_LOG_FILE, "generated game_map");


    EntityManager entityManager;
    log(DEV_LOG_FILE, "generated entity manager");



    Entity playerEntity = entityManager.createEntity();
    entityManager.getPlayerStats()[playerEntity] = playerStats;
    entityManager.getPositions()[playerEntity] = {0,0};//put player at origin so that spawning them doesn"t freak out later
    log(DEV_LOG_FILE, "initialized entity manager");

    int item_count = round(0.001333 * (HEIGHT * WIDTH));
    int monster_count = round(0.00125 *  (HEIGHT * WIDTH));

    float monster_difficulty = 1.0;

    spawn_items(item_count ,entityManager, game_map);
    spawn_monsters(monster_count, monster_difficulty, entityManager, game_map);
    item_count = item_count/10;
    monster_count = monster_count/3;//rescale for difficulty progression handled in player-level-up logic later
    MonsterSystem monsterSystem(entityManager, game_map, playerStats);
    log(DEV_LOG_FILE, "spawned items and monsters");


    log(DEV_LOG_FILE, "spawning player");
    pair<int,int> player_spawn_coords = get_player_spawn_coords(game_map, entityManager);
    log(DEV_LOG_FILE, "found spawn for player");

    int player_x = player_spawn_coords.first, player_y = player_spawn_coords.second;
    entityManager.getPositions()[playerEntity] = {player_x, player_y};
    log(DEV_LOG_FILE, "spawned player");





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
        start_y = min(start_y, HEIGHT - LINES - 3);
        int end_y = start_y + LINES - 3;

        // get player fov
        set<pair<int, int>> player_fov = calculate_fov(
            game_map, player_x, player_y, PLAYER_FOV_RADIUS + playerStats.speed
        );
        // // Set player fov to visible and visited
        // for ( pair<int,int> coords : player_fov) {
        //     game_map[coords.first][coords.second].visible = true;
        //     game_map[coords.first][coords.second].visited = true;
        // }


        // initialize empty draw buffer
        vector<pair<string, pair<int, int>>> draw_buffer;

        for (int y = start_y; y < end_y; ++y) {
            for (int x = start_x; x < end_x; ++x) {
                if (x < 0 || x >= WIDTH)     continue;
                if (y < 0 || y >= HEIGHT)    continue;
                
                wstring ch = game_map[x][y].ch;
                draw_buffer.push_back(make_pair(ch, make_pair(y, x)));

                if (is_in_set({x, y}, player_fov)) {
                    if (check_if_in(WALL_TILES, game_map[x][y].ch)
                        || check_if_in(GROUND_TILES, game_map[x][y].ch)) game_map[x][y].visited = true;//
                    game_map[x][y].visible = true;

                }

            }
        }
        
        // draw_buffer.push_back(make_pair(PLAYER_TILE, make_pair(player_y, player_x)));

        auto& positions = entityManager.getPositions();
        PositionComponent playerPosition = {player_x,player_y};
        auto& monsters = entityManager.getMonsterComponents();
        auto& items = entityManager.getItemComponents();

        vector<pair<pair<int, int>, pair<string, int>>> visible_entities;

        //draw player as a visible character
        visible_entities.push_back(make_pair(make_pair(player_y, player_x),make_pair(PLAYER_TILE, playerStats.color)));

        for (const auto& entry : positions) {
            Entity entity = entry.first;
            if (monsters.find(entity) != monsters.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, player_fov)) {
                    wstring entity_tile;
                    int entity_color;
                    if (monsters[entity].health > monsters[entity].total_health/2) {
                        entity_tile = monsters[entity].characterFullHealth;
                        entity_color = monsters[entity].color;
                    } else if (monsters[entity].health > monsters[entity].total_health/5 ) {
                        entity_tile = monsters[entity].characterMidHealth;
                        entity_color = monsters[entity].color;
                    } else {
                        entity_tile = monsters[entity].characterLowHealth;
                        entity_color = monsters[entity].color;
                    }
                    //add entities visible to player
                    visible_entities.push_back(make_pair(make_pair(position.y, position.x),make_pair(entity_tile, entity_color)));
                }
            } else if (items.find(entity) != items.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, player_fov)) {
                    wstring entity_tile = items[entity].character;
                    int entity_color = items[entity].color;
                    visible_entities.push_back(make_pair(make_pair(position.y, position.x), make_pair(entity_tile, entity_color)));
                }
            }
        }

        // palette[3] = get_player_color(playerStats, false);

        render_buffer(draw_buffer, game_map, visible_entities, start_x, start_y);
        frame_count ++;
        wnoutrefresh(stdscr);

        // movementSystem.update();
        // log(DEV_LOG_FILE, "updated movement system");


        //clear fov from map tiles
        for (pair<int, int> fov_coords : player_fov ) {
            game_map[fov_coords.first][fov_coords.second].visible = false;
        }

        entityManager.getPlayerStats()[playerEntity] = playerStats;
        int memory_radius = PLAYER_MEMORY_RADIUS * playerStats.defense;

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
            playerStats.color = get_player_color(playerStats);
            entityManager.getPlayerStats()[playerEntity] = playerStats;
            item_count = item_count * 11/10;
            monster_count = monster_count * 12/10;
            monster_difficulty = monster_difficulty * 11/10;

            spawn_items(item_count,entityManager,game_map);
            spawn_monsters(monster_count,monster_difficulty,entityManager,game_map);//congrats the player with friends!

        }

        monsterSystem.update();
        log(DEV_LOG_FILE, "updated monster system");

        draw_UI(playerStats);
        mvprintw(0,0,"(x%d,y%d)",player_x,player_y);


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

                    if (monsterPosition.x == player_x + delta.first
                        && monsterPosition.y == player_y + delta.second) {
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
                if (!occupied) {
                    player_x += delta.first;
                    player_y += delta.second;
                    playerPosition.x = player_x;
                    playerPosition.y = player_y;
                    positions[0] = playerPosition;
                }
            } else if (key == 'q') {
                break;
            } else if (key == ' ') {
                // Player attack
                auto& playerPosition = positions[playerEntity];
                auto& monsters = entityManager.getMonsterComponents();
                if (playerStats.energy >=2) {
                    playerStats.energy -= 2;
                    for (const auto& entry : monsters) {
                        const int player_max_arc = playerStats.attack/10;
                        int hit_monsters = 0;
                        Entity monsterEntity = entry.first;
                        PositionComponent& monsterPosition = positions[monsterEntity];
                        MonsterComponent& monster = monsters[monsterEntity];

                        if (abs(playerPosition.x - monsterPosition.x) <= playerStats.speed
                        && abs(playerPosition.y - monsterPosition.y) <= playerStats.speed
                        && is_in_set({monsterPosition.x, monsterPosition.y}, player_fov)
                        && playerStats.energy >= 5 ) {
                            playerStats.energy -= 5;
                            int damage = get_random_int(0, playerStats.attack);
                            monster.health -= damage;
                            hit_monsters++;

                            if (damage > 0) {
                                add_combat_log("Player dealt " + to_string(damage) + " damage to a monster.");
                            } else {
                                add_combat_log("Player's attack was blocked by a monster.");
                            }

                            if (monster.health <= 0) {
                                add_combat_log("Player has defeated a monster.");
                                entityManager.destroyEntity(monsterEntity);
                            }


                            if (hit_monsters>player_max_arc) break;
                        }
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

        entityManager.getPositions()[playerEntity] = {player_x, player_y};

        auto frameEnd = chrono::steady_clock::now();
        auto frameDuration = chrono::duration_cast<chrono::milliseconds>(frameEnd - frameStart);

        // If the frame finished faster than the time per frame, sleep for the remaining time
        if (frameDuration < timePerFrame) {
            this_thread::sleep_for(timePerFrame - frameDuration);
        }

        if (frame_count % FPS == 0) {// runs every second
            //replenish health
            playerStats.health = min(PLAYER_MAX_HP, playerStats.health + min(15,playerStats.attack/10));
            //replenish mana
            playerStats.energy = min(PLAYER_MAX_EP, playerStats.energy + min(15,5+playerStats.defense/10));

        }

        if (frame_count % (275*FPS/100) == 0) {//runs every few seconds
            //clear ui
            while (combat_log.size() > UI_LOG_COUNT){
                combat_log.erase(combat_log.begin());//cull to ui log size
            }
            if (!combat_log.empty()) {
                combat_log.erase(combat_log.begin());
            }
        }

        //clear draw_buffer
        visible_entities.clear();
        draw_buffer.clear();



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
