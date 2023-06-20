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
// v0.5x

const string DEV_LOG_FILE = "dev_log.txt";
const string GAME_LOG_FILE = "game_log.txt";

const int UI_LOG_COUNT = 7;

const int FPS = 15;
const int WIDTH = 2048, HEIGHT = 2048;
const char PLAYER = '@';
const int  PLAYER_MAX_HP = 100;
const int  PLAYER_FOV_RADIUS = 3;
const int  PLAYER_MEMORY_RADIUS = pow(PLAYER_FOV_RADIUS,2);

const bool sound = false;
Mix_Chunk* PLAYER_FOOTSTEP_SOUND = nullptr;
const int PLAYER_FOOTSTEP_SOUND_COUNT = 6;

const vector<char> ground_tiles = {'.',',','`','"'};
const vector<char> wall_tiles = {'[',']','{','}','=','|'};
const vector<char> trap_tiles = {'&','#'};
const vector<char> void_tiles = {' '};

noise::module::Perlin perlin;
random_device rd;
mt19937 gen(rd());

// HELPER FUNCTIONS

template<typename... Args>
void log(const string filename, Args&&... args) {
    ofstream logfile(filename, ios::app);  // Open the file in append mode

    if (!logfile.is_open()) return;

    // Redirect std::cout to the log file
    streambuf* coutbuf = cout.rdbuf();
    cout.rdbuf(logfile.rdbuf());

    // Create a string stream
    ostringstream oss;
    (oss << ... << forward<Args>(args));
    // Log the formatted message
    cout << oss.str() << endl;

    // Restore std::cout
    cout.rdbuf(coutbuf);
    // Close the log file
    logfile.close();
}

int get_random_int(const int min, const int max) {
    uniform_int_distribution<> distr(min, max);
    return distr(gen);

}

char get_random_character(const vector<char>& vec) {
    return vec[get_random_int(0,vec.size() - 1)];
}

bool check_if_in(const vector<char>& vec, const char compare) {
    return find(vec.begin(), vec.end(), compare) != vec.end();
}

template <typename T, template<typename...> class Container>
bool is_in_set(const T& element, const Container<T>& set) {
    return set.find(element) != set.end();
}


vector<string> combat_log;

void add_combat_log(const string& message) {
    log(DEV_LOG_FILE, message);
    log(GAME_LOG_FILE, message);
    combat_log.push_back(message);
    if (combat_log.size() > UI_LOG_COUNT) {
        combat_log.erase(combat_log.begin());
    }
}

// COLOR SETUP

short get_color_pair_index(short foreground, short background) {
    return ((background & 7) << 4 | (foreground & 15)) + 1;
}

void init_all_color_pairs() {
    log(DEV_LOG_FILE, "called init_all_color_pairs");
    const vector<int> all_colors = {
        COLOR_BLACK, COLOR_RED,     COLOR_GREEN, COLOR_YELLOW,
        COLOR_BLUE,  COLOR_MAGENTA, COLOR_CYAN,  COLOR_WHITE
    };

    for (short background : all_colors)
    for (short foreground = 0; foreground < 16; foreground++) {
        init_pair(
            get_color_pair_index(foreground, background),
            foreground, background);
    }
}

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
        // Example: std::cerr << "Failed to load footstep sound: " << Mix_GetError() << std::endl;
        return;
    }

    // Play the random sound file
    Mix_PlayChannel(-1, footstepSound, 0);

    // Free the sound chunk after playing
    // Mix_FreeChunk(footstepSound);
}




vector<vector<char>> generate_map() {
    log(DEV_LOG_FILE, "called generate_map");
    vector<vector<char>> map(WIDTH, vector<char>(HEIGHT));

    // Generate open areas using Perlin noise
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[x][y] = get_random_character(void_tiles);
            double value = perlin.GetValue(x / 10.0, y / 10.0, 0.0);

            if (value < 0.2)
                map[x][y] = get_random_character(ground_tiles);
            else if (value < 0.21)
                map[x][y] = get_random_character(trap_tiles);
            else if (value < 0.8)
                map[x][y] = get_random_character(wall_tiles);
        }
    }

    // Flood fill to ensure all open areas are connected to an edge
    vector<vector<bool>> visited(WIDTH, vector<bool>(HEIGHT, false));
    queue<pair<int, int>> q;

    // Start flood fill from all edge cells
    for (int x = 0; x < WIDTH; ++x) {
        if (!visited[x][0]) {
            visited[x][0] = true;
            q.push({x, 0});
        }
        if (!visited[x][HEIGHT - 1]) {
            visited[x][HEIGHT - 1] = true;
            q.push({x, HEIGHT - 1});
        }
    }
    for (int y = 0; y < HEIGHT; ++y) {
        if (!visited[0][y]) {
            visited[0][y] = true;
            q.push({0, y});
        }
        if (!visited[WIDTH - 1][y]) {
            visited[WIDTH - 1][y] = true;
            q.push({WIDTH - 1, y});
        }
    }

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
            if (nx >= 0 && nx < WIDTH && ny >= 0 && ny < HEIGHT && check_if_in(ground_tiles, map[nx][ny]) && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    // Convert unvisited cells to closed spaces
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if (!visited[x][y] && check_if_in(ground_tiles, map[x][y])) {
                map[x][y] = get_random_character(wall_tiles);
            }
        }
    }

    // Draw a border around the map
    for (int x = 0; x < WIDTH; ++x) {
        map[x][0] = get_random_character(wall_tiles);
        map[x][HEIGHT - 1] = get_random_character(wall_tiles);
    }
    for (int y = 0; y < HEIGHT; ++y) {
        map[0][y] = get_random_character(wall_tiles);
        map[WIDTH - 1][y] = get_random_character(wall_tiles);
    }


    return map;
}

set<pair<int, int>> calculate_fov(
        vector<vector<char>>& map,
        int center_x, int center_y,
        int radius) {
    log(DEV_LOG_FILE, "called calculate_fov with at center x", center_x, ",y", center_y, "and radius ", radius);

    set<pair<int, int>> fov;

    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad);
        double x = center_x, y = center_y;

        for (int j = 0; j < radius; ++j) {
            x += dx;
            y += dy;
            int ix = round(x), iy = round(y);

            if (ix <= 0 || iy <= 0 || ix >= WIDTH || iy >= HEIGHT)
                break;

            fov.insert({ix, iy});

            if (check_if_in(wall_tiles, map[ix][iy]))
                break;
        }
    }

    return fov;
}

// ENTITIES

struct PlayerStats {
    int health;
    int attack;
    int defense;
    int speed;
};

struct PositionComponent {
    int x;
    int y;
};

struct AIComponent {
    int chaseRadius;
    int attackRadius;
    int cooldown;
};

struct MonsterComponent {
    int attackPower;
    int attackRadius;
    int cooldown;
    int health;
};


enum class ItemType {
    Potion,
    MidPotion,
    StrongPotion,
    WeaponBits,
    ArmorBits,
    SpeedBoost
};

struct ItemComponent {
    ItemType type;
    char character;
    function<void(PlayerStats&)> effect;
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


//pathfinding begin
struct Node {
    int x, y;
    double g, h;
    Node* parent;

    Node(int x, int y, Node* parent = nullptr, double g = 0, double h = 0)
        : x(x), y(y), parent(parent), g(g), h(h) {}

    double f() const {
        return g + h;
    }
};

class NodePriorityQueue {
public:
    bool operator()(const Node* lhs, const Node* rhs) const {
        return lhs->f() > rhs->f();
    }
};


double heuristic(const vector<vector<char>>& map, int x1, int y1, int x2, int y2) {
    // Modify the heuristic function implementation according to your needs
    return abs(x1 - x2) + abs(y1 - y2);
}

double cost(const vector<vector<char>>& map, int x2, int y2) {
    // Modify the cost function implementation according to your needs
    return 1.0;
}

vector<Node> aStar(const PositionComponent& start, const PositionComponent& goal,
                   const vector<vector<char>>& map) {

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
                    double tentativeG = current->g + cost(map, x, y);

                    if (openSet[x][y] == nullptr && tentativeG != INFINITY) {
                        openSet[x][y] = new Node(x, y, current, tentativeG, heuristic(map, x, y, goal.x, goal.y));
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

//pathfinding end


class MonsterSystem {
private:
    EntityManager& entityManager;
    vector<vector<char>>& map;
    PlayerStats& playerStats;

public:
    MonsterSystem(EntityManager& entityManager, vector<vector<char>>& map, PlayerStats& playerStats)
        : entityManager(entityManager), map(map), playerStats(playerStats) {}

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
                vector<Node> path = aStar(monsterPosition, playerPosition, map);
                int new_x,new_y;
                // Move the monster to the next node in the path if it exists
                if (!path.empty()) {
                    new_x = path[1].x;
                    new_y = path[1].y;
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

                // Only move the monster to the new position if it's not occupied
                if (!is_occupied and check_if_in(ground_tiles, map[new_x][new_y])) {
                    monsterPosition.x = new_x;
                    monsterPosition.y = new_y;
                }

                log(DEV_LOG_FILE, "checking monster", entity, "'s attack range");
                set<pair<int, int>> monster_attack_range =
                    calculate_fov(
                        map,
                        monsterPosition.x, monsterPosition.y,
                        monster.attackRadius
                    );
                log(DEV_LOG_FILE, "monster", entity, "'s attack range set");


                if (distance <= ai.attackRadius && monster.cooldown == 0
                    && is_in_set({playerPosition.x, playerPosition.y}, monster_attack_range) ) {
                    log(DEV_LOG_FILE, "monster", entity, " attacks player");

                    int damage = max(0,get_random_int(1, monster.attackPower) - get_random_int(0,playerStats.defense));
                    playerStats.health -= damage;
                    monster.cooldown = FPS * get_random_int(1,2);

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


void spawnItems(EntityManager& entityManager, vector<vector<char>>& map) {
    log(DEV_LOG_FILE, "spawning items");
    vector<ItemComponent> itemTemplates = {
        {ItemType::Potion, 'o',
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP, stats.health + 5);}},
        {ItemType::MidPotion, 'O',
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 10);}},
        {ItemType::StrongPotion, '0',
            [](PlayerStats& stats) {stats.health = min(PLAYER_MAX_HP,stats.health + 25);}},
        {ItemType::WeaponBits, 'a',
            [](PlayerStats& stats) { stats.attack += 1; }},
        {ItemType::ArmorBits, 'd',
            [](PlayerStats& stats) { stats.defense += 1; }},
        {ItemType::SpeedBoost, 's',
            [](PlayerStats& stats) { stats.speed += 1; }}
    };
    for (int i = 0; i < HEIGHT * WIDTH / 2048; ++i) {
        for (const auto& item : itemTemplates) {
            int x, y;
            do {
                x = get_random_int(0, WIDTH - 1);
                y = get_random_int(0, HEIGHT - 1);
            } while (!check_if_in(ground_tiles, map[x][y]));
            Entity itemEntity = entityManager.createEntity();
            entityManager.getPositions()[itemEntity] = {x, y};
            entityManager.getItemComponents()[itemEntity] = item;
        }
    }
}

void spawnMonsters(int monster_count, EntityManager& entityManager, vector<vector<char>>& map) {
    log(DEV_LOG_FILE, "spawning monsters");
    for (int i = 0; i < monster_count; ++i) {
        int x, y;
        do {
            x = get_random_int(0, WIDTH - 1);
            y = get_random_int(0, HEIGHT - 1);
        } while (!check_if_in(ground_tiles, map[x][y]));
        Entity monsterEntity = entityManager.createEntity();
        entityManager.getPositions()[monsterEntity] = {x, y};
        
        pair<int, int> chase_dis = {7, 25};
        pair<int, int> attack_range_dis = {1, 7};
        pair<int, int> attack_power_dis = {1, 20};
        pair<int, int> cooldown_dis = {1, 5};
        pair<int, int> health_dis = {5, 25};
        
        entityManager.getAIComponents()[monsterEntity] = {
            get_random_int(chase_dis.first,chase_dis.second),
            get_random_int(attack_range_dis.first,attack_range_dis.second),
            get_random_int(cooldown_dis.first,cooldown_dis.second)
        };
        entityManager.getMonsterComponents()[monsterEntity] = {
            get_random_int(attack_power_dis.first,attack_power_dis.second),
            get_random_int(attack_range_dis.first,attack_range_dis.second),
            get_random_int(cooldown_dis.first,cooldown_dis.second),
            get_random_int(health_dis.first,health_dis.second)
        };
    }
}


void render_buffer(
        const vector<pair<char, pair<int, int>>>& draw_buffer,
        const vector<vector<char>>& map,
        const vector<short>& palette,
        const vector<pair<int, int>>& visited,
        int start_x, int start_y) {
    
    log(DEV_LOG_FILE, "rendering buffer");

    short color_pair = get_color_pair_index(8, COLOR_BLACK);
    attron(COLOR_PAIR(color_pair));

    for (const auto& position : visited) {
        int x = position.first;
        int y = position.second;
        char ch = map[x][y];
        mvaddch(y - start_y, x - start_x, ch);
    }

    attroff(COLOR_PAIR(color_pair));

    for (const auto& item : draw_buffer) {
        char ch = item.first;
        int y = item.second.first - start_y;
        int x = item.second.second - start_x;
        short color_pair = 0;

        if (check_if_in(wall_tiles, ch)) {
            color_pair = palette[1];
        } else if (check_if_in(trap_tiles, ch)) {
            color_pair = palette[1];
        } else if (check_if_in(ground_tiles, ch)) {
            color_pair = palette[0];
        } else if (ch == '@') {
            color_pair = palette[3];
        }

        attron(COLOR_PAIR(color_pair));
        mvaddch(y, x, ch);
        attroff(COLOR_PAIR(color_pair));
    }
}

string ItemTypeToString(ItemType type) {
    switch (type) {
        case ItemType::Potion: return "Potion";
        case ItemType::MidPotion: return "Medium Potion";
        case ItemType::StrongPotion: return "Strong Potion";
        case ItemType::WeaponBits: return "Weapon Bits";
        case ItemType::ArmorBits: return "Armor Bits";
        case ItemType::SpeedBoost: return "Speed Boost";
        // Include all other possible ItemType values...
        default: return "Unknown Item";
    }
}


int get_player_color(PlayerStats& playerStats) {
    int color_index;

    if (playerStats.attack > playerStats.defense
        && playerStats.attack > playerStats.speed) {
        color_index = 2;//red
    } else if (playerStats.defense == playerStats.attack
               && playerStats.defense > playerStats.speed) {
        color_index = 6;//magenta
    } else if (playerStats.defense > playerStats.attack
               && playerStats.defense > playerStats.speed) {
        color_index = 5;//blue
    } else if (playerStats.defense == playerStats.speed
               && playerStats.defense > playerStats.attack) {
        color_index = 7;//cyan
    } else if (playerStats.speed > playerStats.attack
               && playerStats.speed > playerStats.defense) {
        color_index = 3;//green
    } else if (playerStats.speed == playerStats.attack
               && playerStats.speed > playerStats.defense) {
        color_index = 4;//yellow
    } else if (playerStats.defense == playerStats.attack
               && playerStats.defense == playerStats.speed) {
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

void draw_UI(PlayerStats& playerStats) {
    int x = 2;
    int y = 1;
    int player_color = get_player_color(playerStats);
    int player_level = round((playerStats.attack + playerStats.defense + playerStats.speed)/10);
    string player_class = get_player_class_name(playerStats);
    attron(COLOR_PAIR(player_color));
    mvprintw(y, x, "LVL%d %s",
             player_level,
             player_class.c_str()
            );
    attroff(COLOR_PAIR(player_color));
    mvprintw(y+1, x, "HP: %d", playerStats.health);
    mvprintw(y+2, x, "ATT: %d DEF: %d SPD: %d",
             playerStats.attack,
             playerStats.defense,
             playerStats.speed
            );

    y += 2;
    // mvprintw(y, x, "Combat Log:");
    y++;

    int log_size = min(LINES - 3, static_cast<int>(combat_log.size()));

    for (int i = 0; i < log_size; i++) {
        mvprintw(y + i, x, combat_log[combat_log.size() - log_size + i].c_str());
    }
}


pair<int,int> move_player(EntityManager& entityManager,
                          PlayerStats& playerStats,
                          const vector<vector<char>>& map,
                          const pair<int,int> direction,
                          const bool sprinting) {
    log(DEV_LOG_FILE, "player opts to move x", direction.first, ",y", direction.second);

    pair<int,int> delta = {0, 0};
    int distance = 1;
    if (sprinting) {
        distance = playerStats.speed;
    }

    auto& positions = entityManager.getPositions();
    PositionComponent& playerPosition = positions[0];

    for (int i = 0; i < distance; i++) {
        int dx = delta.first + direction.first;
        int dy = delta.second + direction.second;
        if (check_if_in(ground_tiles, map[playerPosition.x+dx][playerPosition.y+dy])) {
            delta = {dx, dy};
        } else if (check_if_in(trap_tiles, map[playerPosition.x+dx][playerPosition.y+dy])) {
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


int main() {
    log(DEV_LOG_FILE, "Running file_crawler");

    initscr();
    log(DEV_LOG_FILE, "initialized screen");

    start_color();
    init_all_color_pairs();
    log(DEV_LOG_FILE, "initialized color");


    if (sound) {
        // Initialize SDL (audio)
        Mix_FreeChunk(PLAYER_FOOTSTEP_SOUND);
        Mix_CloseAudio();
        Mix_Quit();
        SDL_Quit();
        SDL_Init(SDL_INIT_AUDIO);

        // Initialize SDL Mixer
        int mixerFlags = MIX_INIT_MP3 | MIX_INIT_OGG; // Flags for the audio formats you want to support
        int initResult = Mix_Init(mixerFlags);
        if ((initResult & mixerFlags) != mixerFlags) {
            // Handle initialization error
            // Example: std::cerr << "Failed to initialize SDL Mixer: " << Mix_GetError() << std::endl;
            return 1;
        }

        // Open the audio device
        if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) < 0) {
            // Handle audio device opening error
            // Example: std::cerr << "Failed to open audio device: " << Mix_GetError() << std::endl;
            return 1;
        }

        PLAYER_FOOTSTEP_SOUND = Mix_LoadWAV("data/sound/footstep.wav");  // Replace "footstep.wav" with the path to your sound file
        if (!PLAYER_FOOTSTEP_SOUND) {
            // Handle sound loading error
            // Example: std::cerr << "Failed to load footstep sound: " << Mix_GetError() << std::endl;
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

        refresh();

        classChoice = getch() - '0';
        if (classChoice >= 1 && classChoice <= 7)
            break;
    }

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
    }

    log(DEV_LOG_FILE, "selected player class");

    PlayerStats playerStats = {PLAYER_MAX_HP, playerAttack, playerDefense, playerSpeed};

    int seed = get_random_int(0, 255);
    log(DEV_LOG_FILE, "generated seed ", seed, " for rngesus");
    perlin.SetSeed(seed);
    log(DEV_LOG_FILE, "rngesus gives us his blessing");

    vector<short> palette = generate_palette(seed);
    log(DEV_LOG_FILE, "generated palette");


    EntityManager entityManager;
    log(DEV_LOG_FILE, "generated entity manager");


    //TODO: spawn player in sensible location
    int player_x = WIDTH / 2;
    int player_y = HEIGHT / 2;

    Entity playerEntity = entityManager.createEntity();
    entityManager.getPositions()[playerEntity] = {player_x, player_y};
    entityManager.getPlayerStats()[playerEntity] = playerStats;
    log(DEV_LOG_FILE, "initialized entity manager");

    vector<vector<char>> map = generate_map();
    vector<pair<int, int>> visited = {};
    log(DEV_LOG_FILE, "generated map");

    spawnItems(entityManager, map);
    spawnMonsters(1.314*(HEIGHT + WIDTH),entityManager, map);
    MonsterSystem monsterSystem(entityManager, map, playerStats);
    log(DEV_LOG_FILE, "spawned items and monsters");

    nodelay(stdscr, TRUE);
    auto timePerFrame = std::chrono::milliseconds(1000 / FPS); // Approximately 15 FPS

    log(DEV_LOG_FILE, "starting game loop");
    while (true) {
        clear();
        auto frameStart = std::chrono::steady_clock::now();


        int start_x = max(0, player_x - COLS / 2);
        start_x = min(start_x, WIDTH - COLS);
        int end_x = start_x + COLS;
        int start_y = max(0, player_y - LINES / 2);
        start_y = min(start_y, HEIGHT - LINES);
        int end_y = start_y + LINES;

        set<pair<int, int>> fov = calculate_fov(
            map, player_x, player_y, PLAYER_FOV_RADIUS * playerStats.speed
        );

        vector<pair<char, pair<int, int>>> draw_buffer;

        for (int y = start_y; y < end_y; ++y) {
            for (int x = start_x; x < end_x; ++x) {
                if (x < 0 || x >= WIDTH)     continue;
                if (y < 0 || y >= HEIGHT)    continue;
                if (!is_in_set({x, y}, fov)) continue;
                
                char ch = map[x][y];
                draw_buffer.push_back(make_pair(ch, make_pair(y, x)));
                visited.push_back(make_pair(x, y));
            }
        }
        
        entityManager.getPlayerStats()[playerEntity] = playerStats;
        int memory_radius = pow(PLAYER_FOV_RADIUS, 2) * playerStats.defense   ;

        // Forget distant visited tiles
        visited.erase(remove_if(visited.begin(), visited.end(),
            [player_x, player_y, memory_radius](const auto& position) {
                int dx = position.first  - player_x;
                int dy = position.second - player_y;
                return pow(dx/2, 2) + pow(dy, 2) >= pow(memory_radius, 2);
            }),
            visited.end()
        );



        draw_buffer.push_back(make_pair(PLAYER, make_pair(player_y, player_x)));

        auto& positions = entityManager.getPositions();
        auto& monsters = entityManager.getMonsterComponents();
        auto& items = entityManager.getItemComponents();

        for (const auto& entry : positions) {
            Entity entity = entry.first;
            if (monsters.find(entity) != monsters.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, fov)) {
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
                if (is_in_set({position.x, position.y}, fov)) {
                    char entity_tile = items[entity].character;
                    draw_buffer.push_back(make_pair(entity_tile, make_pair(position.y, position.x)));
                }
            }
        }


        palette[3] = get_player_color(playerStats);

        render_buffer(draw_buffer, map, palette, visited, start_x, start_y);

        refresh();

        draw_UI(playerStats);


        int key = getch();
        if (key != ERR) {
            pair<int,int> delta = {0,0};
            if (key == 'w') {
                pair<int,int> direction = {0,-1};
                //bool represents whether or not the player is sprinting
                delta = move_player(entityManager, playerStats, map, direction, false);
                player_x += delta.first;
                player_y += delta.second;

            } else if (key == 'W') {
                pair<int,int> direction = {0,-1};
                delta = move_player(entityManager, playerStats, map, direction, true);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 's') {
                pair<int,int> direction = {0,1};
                //bool represents whether or not the player is sprinting
                delta = move_player(entityManager, playerStats, map, direction, false);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 'S') {
                pair<int,int> direction = {0,1};
                delta = move_player(entityManager, playerStats, map, direction, true);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 'a') {
                pair<int,int> direction = {-1,0};
                delta = move_player(entityManager, playerStats, map, direction, false);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 'A') {
                pair<int,int> direction = {-1,0};
                delta = move_player(entityManager, playerStats, map, direction, true);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 'd') {
                pair<int,int> direction = {1,0};
                delta = move_player(entityManager, playerStats, map, direction, false);
                player_x += delta.first;
                player_y += delta.second;
            } else if (key == 'D') {
                pair<int,int> direction = {1,0};
                delta = move_player(entityManager, playerStats, map, direction, true);
                player_x += delta.first;
                player_y += delta.second;
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
                    && is_in_set({monsterPosition.x, monsterPosition.y}, fov) ) {
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
                        add_combat_log("Picked up " + ItemTypeToString(item.type));
                        break;
                    }
                }
            }

        }



        // movementSystem.update();
        // log(DEV_LOG_FILE, "updated movement system");

        monsterSystem.update();
        log(DEV_LOG_FILE, "updated monster system");

        entityManager.getPositions()[playerEntity] = {player_x, player_y};

        draw_buffer.clear();


        if (playerStats.health <= 0) {
            add_combat_log("Game Over. Player has died.");
            endwin();
            printf("The monster has won...");
            return 0;
        }


        auto frameEnd = std::chrono::steady_clock::now();
        auto frameDuration = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd - frameStart);

        // If the frame finished faster than the time per frame, sleep for the remaining time
        if (frameDuration < timePerFrame) {
            std::this_thread::sleep_for(timePerFrame - frameDuration);
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
