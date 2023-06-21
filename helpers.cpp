using namespace std;
// HELPER FUNCTIONS


noise::module::Perlin perlin;
random_device rd;
mt19937 gen(rd());

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
    }
}


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




//pathfinding end




