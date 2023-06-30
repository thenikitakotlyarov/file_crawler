#include <iostream>
#include <curses.h>

int main() {
    // Initialize curses
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Enable 256-color mode
    if (has_colors() && COLORS >= 256) {
        use_default_colors();
        for (int i = 0; i < 256; ++i) {
            init_pair(i + 1, i, -1);
        }
    }

    // Create debug menu
    WINDOW* menuWin = newwin(256, 40, 0, 0);
    box(menuWin, 0, 0);
    refresh();
    wrefresh(menuWin);

    // Render color pairs
    for (int i = 0; i < 256; ++i) {
        wattron(menuWin, COLOR_PAIR(i + 1));
        wprintw(menuWin, "Pair %d\n", i + 1);
        wattroff(menuWin, COLOR_PAIR(i + 1));
    }

    // Wait for key press
    getch();

    // Clean up and exit
    delwin(menuWin);
    endwin();
    return 0;
}
