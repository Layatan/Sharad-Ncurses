#include "sharad.hpp"

PAGE currentPage = PAGE::MAIN_MENU;
uint64_t frameCount = 0;
cords screenSize = {0,0};
//wait this might a *nix exclusive

int main() {
    // Initialize ncurses
    setlocale(LC_ALL, "");
    initscr();
    start_color();
    use_default_colors();
    noecho();
    cbreak();
    curs_set(0);
    timeout(0); // Non-blocking getch()
    keypad(stdscr, TRUE); // Enable keypad for special keys
    mousemask(ALL_MOUSE_EVENTS, NULL); // Enable all mouse events
    nodelay(stdscr, TRUE); // Make getch() non-blocking

    sharad app;
    app.run();

    endwin();
    return 0; 
}
