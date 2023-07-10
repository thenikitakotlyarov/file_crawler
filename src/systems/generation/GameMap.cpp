#include "GameMap.h"

noise::module::Perlin perlin;

MapSystem::MapSystem() {
    bool init_out = Initialize();
    if (!init_out) {
        running = false;
    } else {
        running = true;
    }

}

MapSystem::~MapSystem() {
    CleanUp();
}

bool MapSystem::Initialize() {
    // Initialize systems


    return true;
}

void MapSystem::CleanUp() {
    // Cleanup on tear-down


}


Color MapSystem::get_tile_color(const vector<Color> &swatch) {
    return swatch[get_random_int(0, swatch.size() - 1)];
}


GameMap MapSystem::genCave(int height, int width) {
    vector<vector<Tile>> data(width, vector<Tile>(height));


    vector<Color> ground_swatch = get_ground_swatch();
    vector<Color> trap_swatch = get_trap_swatch();
    vector<Color> wall_swatch = get_wall_swatch();


    // Generate open areas using Perlin noise
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            wstring ch = get_random_character(VOID_TILES);
            Color color = NCOLOR_BLACK;
            int tile_z;
            Tile void_tile = {ch, color, false, false, 0, true, false};
            data[x][y] = void_tile;
            double value = perlin.GetValue(x / 10.0, y / 10.0, 0.0);


            if (value < 0.2) {
                ch = get_random_character(GROUND_TILES);
                color = get_tile_color(ground_swatch);
                tile_z = 0;
            } else if (value < 0.21) {
                ch = get_random_character(TRAP_TILES);
                color = get_tile_color(trap_swatch);
                tile_z = 0;
            } else if (value < 0.8) {
                ch = get_random_character(WALL_TILES);
                color = get_tile_color(wall_swatch);
                tile_z = get_random_int(3, 7);
            }

            Tile this_tile = {ch, color, false, false, tile_z, true, false};
            data[x][y] = this_tile;
        }
    }

    // Flood fill to ensure all open areas are connected to an edge
    vector<vector<bool>> visited(width, vector<bool>(height, false));
    queue<pair<int, int>> q;

    // Start flood fill from a random point on top edge
    int initial_x = get_random_int(1, width - 1);
    int initial_y = get_random_int(1, height - 1);
    while (!check_if_in(GROUND_TILES, data[initial_x][initial_y].ch)) {
        initial_x += max(1, min(width - 1, get_random_int(-1, 1)));
        initial_y += max(1, min(height - 1, get_random_int(-1, 1)));
    }
    visited[initial_x][initial_y] = true;
    q.emplace(initial_x, initial_y);

    // Perform flood fill
    while (!q.empty()) {
        int x = q.front().first;
        int y = q.front().second;
        q.pop();

        // Check neighboring cells
        const vector<pair<int, int>> neighbors = {{-1, 0},
                                                  {1,  0},
                                                  {0,  -1},
                                                  {0,  1}};
        for (const auto &neighbor: neighbors) {
            int nx = x + neighbor.first;
            int ny = y + neighbor.second;

            // Check if neighbor is within bounds and an open area
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && check_if_in(GROUND_TILES, data[nx][ny].ch) &&
                !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.emplace(nx, ny);
            }
        }
    }

    // Convert unvisited cells to closed spaces
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < height; ++x) {
            if (!visited[x][y] && check_if_in(GROUND_TILES, data[x][y].ch)) {
                wstring ch = get_random_character(WALL_TILES);
                Color color = get_tile_color(wall_swatch);
                int tile_z = get_random_int(0, 5);
                Tile this_tile = {ch, color, false, false, tile_z,true, false};
                data[x][y] = this_tile;
            }
        }
    }

    // Draw a border around the data
    for (int x = 0; x < height; ++x) {
        wstring ch = get_random_character(WALL_TILES);
        Color color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false, 0,true, false};
        data[x][0] = this_tile;
        data[x][height - 1] = this_tile;
    }
    for (int y = 0; y < height; ++y) {
        wstring ch = get_random_character(WALL_TILES);
        Color color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false, 0,true, false};
        data[0][y] = this_tile;
        data[height - 1][y] = this_tile;
    }

    //add structures to map
    int structure_count = get_random_int(30, 90);
    for (int i = 0; i < structure_count; ++i) {
        int x_bounds = get_random_int(7, 21);
        int y_bounds = get_random_int(7, 21);

        Position start_pos = {
                get_random_int(30, width - 30),
                get_random_int(30, height - 30)};


        for (int x = 0; x <= x_bounds; ++x) {
            for (int y = 0; y <= y_bounds; ++y) {
                if (start_pos.x + x >= width || start_pos.y + y >= height) continue;
                if ((x == 0 || x == x_bounds || y == 0 || y == y_bounds)
                    && (x != x_bounds / 2)
                    && (y != y_bounds / 2)) {
                    data[start_pos.x + x][start_pos.y + y].ch = L"#";
                    data[start_pos.x + x][start_pos.y + y].z = 3;
                } else {
                    data[start_pos.x + x][start_pos.y + y].ch = L".";
                    data[start_pos.x + x][start_pos.y + y].z = 0;
                }
                data[start_pos.x + x][start_pos.y + y].color = NCOLOR_LGREY;
                int wait = break_point();
            }
        }
    }


    GameMap game_map = {
            {
                    "Cave",
                    {
                            getPotionTemplate(),
                            getSerumTemplate(),
                            getSalveTemplate(),
                            getVitalityOrbTemplate(),
                            getPowerOrbTemplate(),
                            getAgiltyOrbTemplate(),
                            getFocusOrbTemplate(),
                            getInsightOrbTemplate(),
                            getFaithOrbTemplate(),
                            getSpecialOrbTemplate()
                    },
                    {
                            getBasicMonster(),
                            getBasicMonster(),
                            getBasicMonster(),
                    }
            },
            data
    };

    return game_map;
}

GameMap &MapSystem::unveilMap(GameMap &game_map, const set<pair<int, int>> &current_fov) {
    for (const auto coords: current_fov) {
        game_map.data[coords.first][coords.second].visible = true;
        if (check_if_in(WALL_TILES, game_map.data[coords.first][coords.second].ch)) {
            game_map.data[coords.first][coords.second].visited = true;

        }
    }

    return game_map;
}

GameMap &MapSystem::veilMap(GameMap &game_map, const set<pair<int, int>> &current_fov) {
    for (const auto coords: current_fov) {
        game_map.data[coords.first][coords.second].visible = false;
    }


    return game_map;
}

GameMap &MapSystem::forgetMap(GameMap &game_map, pair<int, int> player_pos, int mem_mult) {


    // Forget distant visited tiles

    for (int i = 0; i < 360; i++) {
        double rad = i * (M_PI / 180.0);
        double dx = cos(rad), dy = sin(rad) / 2;
        double x = player_pos.first, y = player_pos.second;
        int ix, iy;



        //pass 1
        x += 3 * mem_mult * dx;
        y += 3 * mem_mult * dy;
        ix = round(x), iy = round(y);
        if (ix < 0 || ix >= game_map.data.size() || iy < 0 || iy >= game_map.data[0].size()) continue;
        if (get_random_int(0, 32)) continue;
        else game_map.data[ix][iy].visited = false;

        //pass 2
        x += (mem_mult * 7 / 2) * dx;
        y += (mem_mult * 7 / 2) * dy;
        iy = round(y);
        ix = round(x);
        if (ix < 0 || ix >= game_map.data.size() || iy < 0 || iy >= game_map.data[0].size()) continue;
        if (get_random_int(0, 16)) continue;
        else game_map.data[ix][iy].visited = false;

        //pass 3
        x += (mem_mult * 13 / 3) * dx;
        y += (mem_mult * 13 / 3) * dy;
        ix = round(x), iy = round(y);
        if (ix < 0 || ix >= game_map.data.size() || iy < 0 || iy >= game_map.data[0].size()) continue;
        if (get_random_int(0, 8)) continue;
        else game_map.data[ix][iy].visited = false;

    }

    return game_map;
}


Frame MapSystem::renderMap2D(Frame frame, const GameMap &current_map, int start_y, int start_x, int end_y, int end_x) {
    int max_map_y = current_map.data.size();
    int max_map_x = current_map.data[0].size();

    for (int i = 0; i < end_y - start_y; i++) {
        for (int j = 0; j < end_x - start_x; j++) {
            int map_y = max(0, min(max_map_y, i + start_y));
            int map_x = max(0, min(max_map_x, j + start_x));
            if (current_map.data[map_x][map_y].visible
                || current_map.data[map_x][map_y].visited) {
                frame.data[i][j].ch = current_map.data[map_x][map_y].ch;
                frame.data[i][j].fg_color = NCOLOR_GREY;

            }

        }
    }


    return frame;
}

Frame MapSystem::renderMap3D(Frame frame, const GameMap &current_map, int start_y, int start_x, int end_y, int end_x) {
    int max_map_y = current_map.data.size();
    int max_map_x = current_map.data[0].size();

    frame = renderMap2D(frame, current_map, start_y, start_x, end_y, end_x);

    for (int i = 0; i < frame.data.size(); i++) {
        for (int j = 0; j < frame.data[0].size(); j++) {
            int map_y = max(0, min(max_map_y, i + start_y));
            int map_x = max(0, min(max_map_x, j + start_x));


            int degrade = 0;
            if (current_map.data[map_x][map_y].visited) {
                if (current_map.data[map_x][map_y].z) {
                    if (i < (LINES - 3) / 2) {
                        for (int h = 1; h <= current_map.data[map_x][map_y].z; ++h) {
                            if (i - h + degrade / 2 < 0 || i - h + degrade / 2 >= LINES - 3) continue;
                            if (h > 5) {
                                frame.data[i - h + degrade / 2][j].ch = L"░";
                                frame.data[i - h + degrade / 2][j].fg_color = frame.data[i][j].fg_color;
                            } else if (h > 2) {
                                frame.data[i - h + degrade / 2][j].ch = L"▒";
                                frame.data[i - h + degrade / 2][j].fg_color = frame.data[i][j].fg_color;
                            } else {
                                frame.data[i - h + degrade / 2][j].ch = current_map.data[map_x][map_y].ch;
                                frame.data[i - h + degrade / 2][j].fg_color = frame.data[i][j].fg_color;
                            }

                            if (i >= (LINES - 3) / 2 - 7) {
                                degrade++;
                            }

                        }
                    }

                }
            }
        }
    }


    return frame;
}