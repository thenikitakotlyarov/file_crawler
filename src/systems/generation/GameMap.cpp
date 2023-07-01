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


int MapSystem::get_tile_color(const vector<int> swatch) {
    return swatch[get_random_int(0,swatch.size() - 1)];
}


GameMap MapSystem::genCave(int height, int width) {
    vector<vector<Tile>> data(width, vector<Tile>(height));


    vector<int> ground_swatch = get_ground_swatch();
    vector<int> trap_swatch = get_trap_swatch();
    vector<int> wall_swatch = get_wall_swatch();


    // Generate open areas using Perlin noise
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            wstring ch = get_random_character(VOID_TILES);
            short color = get_color_pair_index(COLOR_WHITE,COLOR_BLACK);
            int height;
            Tile void_tile = {ch, color, false, false, 0};
            data[x][y] = void_tile;
            double value = perlin.GetValue(x / 10.0, y / 10.0, 0.0);


            if (value < 0.2) {
                ch = get_random_character(GROUND_TILES);
                color = get_tile_color(ground_swatch);
                height = 0;
            } else if (value < 0.21) {
                ch = get_random_character(TRAP_TILES);
                color = get_tile_color(trap_swatch);
                height = 0;
            } else if (value < 0.8) {
                ch = get_random_character(WALL_TILES);
                color = get_tile_color(wall_swatch);
                height = get_random_int(3,7);
            }

            Tile this_tile = {ch, color, false, false, height};
            data[x][y] = this_tile;
        }
    }

    // Flood fill to ensure all open areas are connected to an edge
    vector<vector<bool>> visited(width, vector<bool>(height, false));
    queue<pair<int, int>> q;

    // Start flood fill from a random point on top edge
    int initial_x = get_random_int(1, width-1);
    int initial_y = get_random_int(1, height-1);
    while (!check_if_in(GROUND_TILES, data[initial_x][initial_y].ch)) {
        initial_x += max(1,min(width-1,get_random_int(-1,1)));
        initial_y += max(1,min(height-1,get_random_int(-1,1)));
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
            if (nx >= 0 && nx < width && ny >= 0 && ny < height && check_if_in(GROUND_TILES, data[nx][ny].ch) && !visited[nx][ny]) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }

    // Convert unvisited cells to closed spaces
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < height; ++x) {
            if (!visited[x][y] && check_if_in(GROUND_TILES, data[x][y].ch)) {
                wstring ch = get_random_character(WALL_TILES);
                short color = get_tile_color(wall_swatch);
                int height = get_random_int(0,5);
                Tile this_tile = {ch, color, false, false, height};
                data[x][y] =  this_tile;
            }
        }
    }

    // Draw a border around the data
    for (int x = 0; x < height; ++x) {
        wstring ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false, 0};
        data[x][0] = this_tile;
        data[x][height - 1] = this_tile;
    }
    for (int y = 0; y < height; ++y) {
        wstring ch = get_random_character(WALL_TILES);
        short color = get_tile_color(wall_swatch);
        Tile this_tile = {ch, color, false, false, 0};
        data[0][y] = this_tile;
        data[height - 1][y] = this_tile;
    }


    GameMap game_map = {"Cave",data};//TODO:move up to avoid copying?

    return game_map;
}

GameMap MapSystem::unveilMap(GameMap game_map, set<pair<int, int>> current_fov) {
    for (const auto coords: current_fov) {
        game_map.data[coords.first][coords.second].visible = true;
        game_map.data[coords.first][coords.second].visited = true;
    }

    return game_map;
}

GameMap MapSystem::reveilMap(GameMap game_map, set<pair<int, int>> current_fov) {
    for (const auto coords: current_fov) {
        game_map.data[coords.first][coords.second].visible = false;
    }

    return game_map;
}

Frame MapSystem::renderMap2D(Frame frame, const GameMap& current_map, int start_y, int start_x, int end_y, int end_x) {
    int max_map_y = current_map.data.size();
    int max_map_x = current_map.data[0].size();

    for (int i = 0; i < end_y - start_y; i++) {
        for (int j = 0; j < end_x - start_x; j++) {
            int map_y = max(0, min(max_map_y,i+start_y));
            int map_x = max(0, min(max_map_x,j+start_x));
            if (current_map.data[map_x][map_y].visible) {
                frame.data[i][j].first = current_map.data[map_x][map_y].ch;
                frame.data[i][j].second.first = current_map.data[map_x][map_y].color;

            } else if (current_map.data[map_x][map_y].visited) {
                frame.data[i][j].first = current_map.data[map_x][map_y].ch;
                frame.data[i][j].second.first = COLOR_LGREY;

            }

        }
    }


    return frame;
}
