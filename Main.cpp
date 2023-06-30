#include "include.h"
#include "constants.h"
#include "helpers.h"

pair<bool, Game> init() {
    setlocale(LC_ALL, "");
    initscr();
    keypad(stdscr, TRUE); // Enable the keypad for the standard screen
    mousemask(ALL_MOUSE_EVENTS, nullptr);  // Enable mouse events
    start_color();
    init_all_color_pairs();

    curs_set(0);
    noecho();

    //turn off input delays
    nodelay(stdscr, TRUE);
    ESCDELAY = 0;
    mouseinterval(0);

    bool started;
    Game game = Game();

    if (game.running) started = true;
    else started = false;

    return make_pair(started, game);
}

int main() {
    pair<bool, Game> INIT = init();
    if (!INIT.first) exit(1);
    Game GAME = INIT.second;


    int frame_count = 0;
    auto timePerFrame = chrono::milliseconds(1000 / FPS); // Approximately 15 FPS



    while (GAME.running > 0) {

        frame_count++;
        auto frameStart = chrono::steady_clock::now();


        if (GAME.running == 1) {
            render(GAME.CARD_TITLE(LINES, COLS));

        } else if (GAME.running == 2) {
            render(GAME.MENU_MAIN(LINES, COLS));

        } else if (GAME.running == 3) {
            render(GAME.MENU_NEW_GAME(LINES, COLS));

        } else if (GAME.running == 4) {
            render(GAME.MENU_LOAD_GAME(LINES, COLS));

        } else if (GAME.running == 8) {
            render(GAME.DEBUG_COLOR(LINES, COLS));
            int key = getch();
            while (key != '0') {
                key = getch();
                this_thread::sleep_for(timePerFrame);
            }
            GAME.running = 2;

        } else if (GAME.running == 9) {
            render(GAME.MENU_SETTINGS(LINES, COLS));

        } else if (GAME.running >= 10) {//player is playing the game; states over 10 coincide with playable levels
            if (!GAME.READY_TO_PLAY) render(GAME.MENU_LOAD_GAME(LINES,COLS));
            else render(GAME.PLAY_GAME(LINES,COLS));

        }

        GAME.Update(get_input());


        auto frameEnd = chrono::steady_clock::now();
        auto frameDuration = chrono::duration_cast<chrono::milliseconds>(frameEnd - frameStart);

        // If the frame finished faster than the time per frame, sleep for the remaining time
        if (frameDuration < timePerFrame) {
            this_thread::sleep_for(timePerFrame - frameDuration);


        }


    }


    endwin(); // release screen

    return 0;
}
