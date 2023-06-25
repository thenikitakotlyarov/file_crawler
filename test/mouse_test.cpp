#include <ncurses.h>

int main() {
    initscr();                  // Initialize the window
    noecho();                   // Do not display input characters
    keypad(stdscr, TRUE);       // Enable special keys capture
    mousemask(ALL_MOUSE_EVENTS, NULL);  // Enable all mouse events

    mvprintw(0, 0, "Click or move the mouse...");
    mvprintw(1, 0, "Press 'q' to quit the program...");

    MEVENT event;

    while (1) {
        int ch = getch();

        if (ch == KEY_MOUSE) {
            if (getmouse(&event) == OK) {    // Get the details of the event 
                clear();     // Clear the screen
                mvprintw(0, 0, "Mouse Event!");
                mvprintw(1, 0, "Coordinates: x=%d, y=%d", event.x, event.y);

                if (event.bstate & BUTTON1_PRESSED) {
                    mvprintw(2, 0, "Left button pressed");
                } else if (event.bstate & BUTTON2_PRESSED) {
                    mvprintw(2, 0, "Middle button pressed");
                } else if (event.bstate & BUTTON3_PRESSED) {
                    mvprintw(2, 0, "Right button pressed");
                }

                mvprintw(3, 0, "Press 'q' to quit the program...");
            }
        } else if (ch == 'q') {
            break;  // Exit the while loop
        }
    }

    endwin();  // End curses mode

    return 0;
}
