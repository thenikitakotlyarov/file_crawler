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
// v0.3b

const string DEV_LOG_FILE = "dev_log.txt";
const string GAME_LOG_FILE = "game_log.txt";

const int WIDTH = 1024, HEIGHT = 1024;
const int PLAYER_COLOR = COLOR_WHITE;
const char PLAYER = '@';
const int FOV_RADIUS = 27;
const int MEMORY_RADIUS = 54;

const vector<char> ground_tiles = {'.',',','`','"'};
const vector<char> wall_tiles = {'[',']','{','}','=','|','&','#'};
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
    combat_log.push_back(message);
    if (combat_log.size() > 3) {
        combat_log.erase(combat_log.begin());
    }
}

// COLOR SETUP

short get_color_pair_index(short foreground, short background) {
    return ((background & 7) << 4 | (foreground & 15)) + 1;
}

void init_all_color_pairs() {
    const vector<int> all_colors = {
        COLOR_BLACK, COLOR_RED,     COLOR_GREEN, COLOR_YELLOW,
        COLOR_BLUE,  COLOR_MAGENTA, COLOR_CYAN,  COLOR_WHITE
    };

    for (short background : all_colors)
    for (short foreground = 0; foreground < 16; foreground++) {
        init_pair(
            get_color_pair_index(foreground, background),
            foreground, background);
        log(DEV_LOG_FILE, "get_color_pair_index(", foreground, ", ", background, ") = ",
                           get_color_pair_index(   foreground   ,    background   ));
    }
}

vector<short> generate_palette(int seed) {
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

vector<vector<char>> generate_map() {
    vector<vector<char>> map(WIDTH, vector<char>(HEIGHT));

    // Generate open areas using Perlin noise
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            map[x][y] = get_random_character(void_tiles);
            double value = perlin.GetValue(x / 10.0, y / 10.0, 0.0);

            if (value < 0.2)
                map[x][y] = get_random_character(ground_tiles);
            else if (value < 0.6)
                map[x][y] = get_random_character(wall_tiles);
            else if (value < 0.8)
                map[x][y] = get_random_character(trap_tiles);
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

    return map;
}

set<pair<int, int>> calculate_fov(
        vector<vector<char>>& map,
        int player_x, int player_y,
        int radius) {
    
    set<pair<int, int>> fov;
    
    for (int i = 0; i < 360; ++i) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad);
        double x = player_x, y = player_y;

        for (int j = 0; j < radius; ++j) {
            x += dx;
            y += dy;
            int ix = round(x), iy = round(y);

            if (ix < 0 || iy < 0 || ix >= WIDTH || iy >= HEIGHT)
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
    int cooldown; // New member to track the monster's attack cooldown
};

struct MonsterComponent {
    int attackPower;
    int attackRadius;
    int cooldown; // New member to track the monster's attack cooldown
    int health;   // New member to track the monster's health
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

class MovementSystem {
private:
    EntityManager& entityManager;
    vector<vector<char>>& map;

public:
    MovementSystem(EntityManager& entityManager, vector<vector<char>>& map)
        : entityManager(entityManager), map(map) {}

    void update() {
        auto& positions = entityManager.getPositions();
        auto& monsters = entityManager.getMonsterComponents();

        for (const auto& entry : positions) {
            Entity entity = entry.first;

            if (monsters.find(entity) != monsters.end()) {
                PositionComponent& position = positions[entity];
                AIComponent& ai = entityManager.getAIComponents()[entity];

                int new_x = position.x + get_random_int(-1, 1);
                int new_y = position.y + get_random_int(-1, 1);

                if (new_x >= 0 && new_x < WIDTH
                    && new_y >= 0 && new_y < HEIGHT
                    && check_if_in(ground_tiles, map[new_x][new_y]))
                {
                    position.x = new_x;
                    position.y = new_y;
                }
            }
        }
    }
};

class MonsterSystem {
private:
    EntityManager& entityManager;
    vector<vector<char>>& map;
    PlayerStats& playerStats;

public:
    MonsterSystem(EntityManager& entityManager, vector<vector<char>>& map, PlayerStats& playerStats)
        : entityManager(entityManager), map(map), playerStats(playerStats) {}

    void update() {
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

            int dx = monsterPosition.x - playerPosition.x;
            int dy = monsterPosition.y - playerPosition.y;
            double distance = sqrt(dx * dx + dy * dy);

            if (distance <= ai.chaseRadius) {
                int new_x = monsterPosition.x;
                int new_y = monsterPosition.y;

                if (playerPosition.x < monsterPosition.x) {
                    new_x--;
                } else if (playerPosition.x > monsterPosition.x) {
                    new_x++;
                }

                if (playerPosition.y < monsterPosition.y) {
                    new_y--;
                } else if (playerPosition.y > monsterPosition.y) {
                    new_y++;
                }

                set<pair<int, int>> monster_attack_range =
                    calculate_fov(
                        map,
                        monsterPosition.x, monsterPosition.y,
                        monster.attackRadius
                    );

                if (new_x >= 0 && new_x < WIDTH && new_y >= 0 && new_y < HEIGHT && check_if_in(ground_tiles, map[new_x][new_y])) {
                    monsterPosition.x = new_x;
                    monsterPosition.y = new_y;
                }
                if (distance <= ai.attackRadius && monster.cooldown == 0
                    && is_in_set({playerPosition.x, playerPosition.y}, monster_attack_range) ) {
                    int damage = get_random_int(0, monster.attackPower - get_random_int(0,playerStats.defense));
                    playerStats.health -= damage;
                    monster.cooldown = get_random_int(3,7);

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
    vector<ItemComponent> itemTemplates = {
        {ItemType::Potion, [](PlayerStats& stats) { stats.health += 5; }},
        {ItemType::MidPotion, [](PlayerStats& stats) { stats.health += 10; }},
        {ItemType::StrongPotion, [](PlayerStats& stats) { stats.health += 25; }},
        {ItemType::WeaponBits, [](PlayerStats& stats) { stats.attack += 1; }},
        {ItemType::ArmorBits, [](PlayerStats& stats) { stats.defense += 1; }},
        {ItemType::SpeedBoost, [](PlayerStats& stats) { stats.speed += 1; }}
    };
    for (int i = 0; i < 512; ++i) {
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
            color_pair = palette[2];
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


void draw_UI(const PlayerStats& playerStats) {
    int x = 2;
    int y = 1;
    mvprintw(y, x, "HP: %d ATT: %d DEF: %d SPD: %d",
        playerStats.health,
        playerStats.attack,
        playerStats.defense,
        playerStats.speed
    );

    y += 2;
    mvprintw(y, x, "Combat Log:");
    y++;

    int log_size = min(LINES - 3, static_cast<int>(combat_log.size()));

    for (int i = 0; i < log_size; i++) {
        mvprintw(y + i, x, combat_log[combat_log.size() - log_size + i].c_str());
    }
}

int main() {
    initscr();
    start_color();
    curs_set(0);
    noecho();

    int seed = get_random_int(0, 255);

    init_all_color_pairs();
    vector<short> palette = generate_palette(seed);

    perlin.SetSeed(seed);

    vector<vector<char>> map = generate_map();

    int player_x = WIDTH / 2;
    int player_y = HEIGHT / 2;

    vector<pair<int, int>> visited = {};

    EntityManager entityManager;

    Entity playerEntity = entityManager.createEntity();
    entityManager.getPositions()[playerEntity] = {player_x, player_y};

    // Player class selection menu
    int classChoice;

    while(true) {
        clear();
        printw("Please select a class:\n");
        printw("1. Fighter\n");
        printw("2. Rogue\n");
        printw("3. Ranger\n");
        printw("4. Sorcerer\n");
        printw("5. Wizard\n");
        printw("6. Cleric\n");
        printw("7. Adventurer\n");
        refresh();

        classChoice = getch() - '0';
        if (classChoice >= 1 && classChoice <= 7)
            break;
    }

    int playerAttack, playerDefense, playerSpeed;

    switch(classChoice) {
        case 1: // Fighter
            playerAttack  = 10;
            playerDefense = 3;
            playerSpeed   = 3;
            break;
        case 2: // Rogue
            playerAttack  = 7;
            playerDefense = 2;
            playerSpeed   = 7;
            break;
        case 3: // Ranger
            playerAttack  = 3;
            playerDefense = 3;
            playerSpeed   = 10;
            break;
        case 4: // Sorcerer
            playerAttack  = 2;
            playerDefense = 7;
            playerSpeed   = 7;
            break;
        case 5: // Wizard
            playerAttack  = 3;
            playerDefense = 10;
            playerSpeed   = 3;
            break;
        case 6: // Cleric
            playerAttack  = 7;
            playerDefense = 7;
            playerSpeed   = 2;
            break;
        case 7: // Adventurer
            playerAttack  = 5;
            playerDefense = 5;
            playerSpeed   = 5;
            break;
    }

    PlayerStats playerStats = {100, playerAttack, playerDefense, playerSpeed};
    entityManager.getPlayerStats()[playerEntity] = playerStats;

    spawnItems(entityManager, map);
    spawnMonsters(2048,entityManager, map);


    MovementSystem movementSystem(entityManager, map);
    MonsterSystem monsterSystem(entityManager, map, playerStats);

    while (true) {
        clear();

        int start_x = max(0, player_x - COLS / 2);
        start_x = min(start_x, WIDTH - COLS);
        int end_x = start_x + COLS;
        int start_y = max(0, player_y - LINES / 2);
        start_y = min(start_y, HEIGHT - LINES);
        int end_y = start_y + LINES;

        set<pair<int, int>> fov = calculate_fov(
            map, player_x, player_y, FOV_RADIUS
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
        
        // Forget distant visited tiles
        visited.erase(remove_if(visited.begin(), visited.end(),
            [player_x, player_y](const auto& position) {
                int dx = position.first  - player_x;
                int dy = position.second - player_y;
                return pow(dx, 2) + pow(dy, 2) >= pow(MEMORY_RADIUS, 2);
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
                    draw_buffer.push_back(make_pair('M', make_pair(position.y, position.x)));
                }
            } else if (items.find(entity) != items.end()) {
                const PositionComponent& position = entry.second;
                if (is_in_set({position.x, position.y}, fov)) {
                    draw_buffer.push_back(make_pair('o', make_pair(position.y, position.x)));
                }
            }
        }

        render_buffer(draw_buffer, map, palette, visited, start_x, start_y);

        refresh();

        draw_UI(playerStats);

        movementSystem.update();
        monsterSystem.update();

        int key = getch();

        if (key == 'w') {
            if (!check_if_in(wall_tiles, map[player_x][player_y - 1])) {
                    player_y -= 1;
                }
        } else if (key == 'W') {
            for (int i = 0; i < playerStats.speed; i++) {
                if (!check_if_in(wall_tiles, map[player_x][player_y - 1])) {
                    player_y -= 1;
                }
                else {
                    break; // Stop the loop if the player hits a wall
                }
            }
        } else if (key == 's') {
            if (!check_if_in(wall_tiles, map[player_x][player_y + 1])) {
                    player_y += 1;
                }
        } else if (key == 'S') {
            for (int i = 0; i < playerStats.speed; i++) {
                if (!check_if_in(wall_tiles, map[player_x][player_y + 1])) {
                    player_y += 1;
                }
                else {
                    break; // Stop the loop if the player hits a wall
                }
            }
        } else if (key == 'a') {
            if (!check_if_in(wall_tiles, map[player_x - 1][player_y])) {
                    player_x -= 1;
                }
        } else if (key == 'A') {
            for (int i = 0; i < playerStats.speed; i++) {
                if (!check_if_in(wall_tiles, map[player_x - 1][player_y])) {
                    player_x -= 1;
                }
                else {
                    break; // Stop the loop if the player hits a wall
                }
            }
        } else if (key == 'd') {
            if (!check_if_in(wall_tiles, map[player_x + 1][player_y])) {
                    player_x += 1;
                }
        } else if (key == 'D') {
            for (int i = 0; i < playerStats.speed; i++) {
                if (!check_if_in(wall_tiles, map[player_x + 1][player_y])) {
                    player_x += 1;
                }
                else {
                    break; // Stop the loop if the player hits a wall
                }
            }
        } else if (key == 'q') {
            break;
        } else if (key == ' ') {
            // Player attack
            auto& playerPosition = positions[playerEntity];
            auto& monsters = entityManager.getMonsterComponents();
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
        }

        entityManager.getPositions()[playerEntity] = {player_x, player_y};

        draw_buffer.clear();

        for (auto& entry : monsters) {
            MonsterComponent& monster = entry.second;
            monster.cooldown = max(0, monster.cooldown - 1); // Decrement the cooldown of the monster's attack
        }

        if (playerStats.health <= 0) {
            add_combat_log("Game Over. Player has died.");
            endwin();
            printf("The monster has won...");
            return 0;
        }
    }

    endwin();
    return 0;
}
