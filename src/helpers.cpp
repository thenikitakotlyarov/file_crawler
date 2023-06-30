#include "helpers.h"


random_device rd;
mt19937 gen(rd());


wstring int_to_hex_str(int i) {
    wstringstream hex;
    hex << std::hex << i;
    return hex.str();
}


int get_random_int(const int min, const int max) {
    uniform_int_distribution<> distr(min, max);
    return distr(gen);
}


wstring get_random_character(const vector<wstring> &vec) {
    return vec[get_random_int(0, vec.size() - 1)];
}


template<typename T, class Container>
bool is_in_set(const T &element, const Container &set) {
    return set.find(element) != set.end();
}


bool check_if_in(const vector<wstring> &char_array, const wstring compare) {
    bool found = false;
    for (const std::wstring &str: char_array) {
        if (str == compare) {
            found = true;
            break;
        }
    }

    return found;
}


int get_color_pair_index(int fg_idx, int bg_idx) {
    return int((bg_idx * 16) + fg_idx + 1);
}


void init_all_color_pairs() {
    //const vector<int> all_colors = {
    //        COLOR_BLACK, COLOR_RED, COLOR_GREEN, COLOR_YELLOW,
    //        COLOR_BLUE, COLOR_MAGENTA, COLOR_CYAN, COLO   R_WHITE
    //};

    for (int bg = 0; bg < 16; bg++)
        for (int fg = 0; fg < 16; fg++) {
            init_extended_pair(
                    get_color_pair_index(fg, bg),
                    fg, bg);
        }
}


vector<int> get_ground_swatch() {
    vector<vector<int>> options = {
            {COLOR_LGREEN, COLOR_GREEN,   COLOR_YELLOW},
            {COLOR_RED,    COLOR_ORANGE,  COLOR_YELLOW},
            {COLOR_CYAN,   COLOR_AMARINE, COLOR_LGREY}
    };
    return options[get_random_int(0, options.size() - 1)];
}

vector<int> get_trap_swatch() {
    vector<vector<int>> options = {
            {COLOR_RED,    COLOR_ORANGE},
            {COLOR_ORANGE, COLOR_YELLOW},
            {COLOR_RED,    COLOR_YELLOW}
    };
    return options[get_random_int(0, options.size() - 1)];
}

vector<int> get_wall_swatch() {
    vector<vector<int>> options = {
            {COLOR_LGREY, COLOR_GREY,   COLOR_WHITE},
            {COLOR_LBLUE, COLOR_BLUE,   COLOR_CYAN},
            {COLOR_LGREY, COLOR_YELLOW, COLOR_LGREEN}
    };
    return options[get_random_int(0, options.size() - 1)];
}


double heuristic(const GameMap &game_map, int x1, int y1, int x2, int y2) {
    // Modify the heuristic function implementation according to your needs
    return abs(x1 - x2) + abs(y1 - y2);
}

double cost(const GameMap &game_map, int x2, int y2) {
    // Modify the cost function implementation according to your needs
    return 1.0;
}

vector<Node> aStar(const Position &start, const Position &goal,
                   const GameMap &game_map) {

    vector<vector<bool>> closedSet(WIDTH, vector<bool>(HEIGHT, false));
    vector<vector<Node *>> openSet(WIDTH, vector<Node *>(HEIGHT, nullptr));

    priority_queue<Node *, vector<Node *>, NodePriorityQueue> queue;

    openSet[start.x][start.y] = new Node(start.x, start.y);
    queue.push(openSet[start.x][start.y]);

    while (!queue.empty()) {
        Node *current = queue.top();
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
                int x = current->x + dx + get_random_int(-1, 1);
                int y = current->y + dy + get_random_int(-1, 1);

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


int get_input() {
    int key = getch();
    int input = 0;
    if (key != ERR) input = key;
    return input;
}


// Render frame on screen
void render(const Frame &frame) {

    clear();

    for (int i = 0; i < LINES; ++i) {
        for (int j = 0; j < COLS; ++j) {
            const wstring &pixel_char = frame.data[i][j].first;
            int pixel_color = get_color_pair_index(frame.data[i][j].second.first,
                                                   frame.data[i][j].second.second);
            attron(COLOR_PAIR(pixel_color));
            mvaddwstr(i, j, pixel_char.c_str());
            attroff(COLOR_PAIR(pixel_color));
        }
    }


    wnoutrefresh(stdscr);
    doupdate();
}





