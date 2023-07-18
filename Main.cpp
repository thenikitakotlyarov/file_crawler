#include "Game.h"
#include "include.h"
#include "constants.h"
#include "helpers.h"

// Using unique_ptr to handle dynamic memory allocation.
pair<bool, unique_ptr<Game>> init() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE);
    mousemask(ALL_MOUSE_EVENTS, nullptr);
    start_color();
    init_all_color_pairs();
    curs_set(0);
    noecho();
    nodelay(stdscr, TRUE);
    ESCDELAY = 0;
    mouseinterval(0);

    bool started;
    unique_ptr<Game> game = make_unique<Game>();
    if (game->running) started = true;
    else started = false;

    return make_pair(started, move(game));
}


int main() {
    pair<bool, unique_ptr<Game>> INIT = init();
    if (!INIT.first) exit(1);


    unique_ptr<Game> &GAME = INIT.second; // Get the reference to the unique_ptr to Game
    int fps = 0;
    int frame_count = 0;
    auto timePerFrame = chrono::milliseconds(1000 / FPS);

    while (GAME->running > 0) {
        frame_count++;
        auto frameStart = chrono::steady_clock::now();

        // Update all GAME to GAME-> in the following code
        if (GAME->running == 1) {
            GAME->CARD_TITLE(LINES, COLS);
        } else if (GAME->running == 2) {
            GAME->MENU_MAIN(LINES, COLS);
        } else if (GAME->running == 3) {
            GAME->MENU_NEW_GAME(LINES, COLS);
        } else if (GAME->running == 4) {
            GAME->MENU_LOAD_GAME(LINES, COLS);
        } else if (GAME->running == 8) {
            GAME->DEBUG_COLOR(LINES, COLS);
            int key = getch();
            while (key != '0') {
                key = getch();
                this_thread::sleep_for(timePerFrame);
            }
            GAME->running = 2;
        } else if (GAME->running == 9) {
            GAME->MENU_SETTINGS(LINES, COLS);
        } else if (GAME->running == 999) {
            GAME->GAME_OVER(LINES, COLS);
        } else if (GAME->running >= 10) {
            if (!GAME->READY_TO_PLAY) GAME->MENU_LOAD_GAME(LINES, COLS);
            else GAME->PLAY_GAME(LINES, COLS, fps);
        }

        GAME->Update(get_input());

        auto frameEnd = chrono::steady_clock::now();
        auto frameDuration = chrono::duration_cast<chrono::milliseconds>(frameEnd - frameStart);

        if (frameDuration.count() > 0) {  // To avoid division by zero
            fps = min(FPS, (int) (1000 / frameDuration.count()));  // Calculate FPS
        } else fps = 0;


        if (frameDuration < timePerFrame) {
            this_thread::sleep_for(timePerFrame - frameDuration);
        }
    }

    endwin();

    return 0;
}
