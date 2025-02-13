#pragma once
#include <ncurses.h>

#include "components/ui/ascii_art.hpp"
#include "components/drawAscii.hpp"
#include "components/drawMenu.hpp"
#include "components/ui/credits.hpp"

#include <iostream>
#include <queue>


// struct cords { //moved to widget.hpp 
//     int x = 0, y = 0; //may as well set defaults 
//     cords(int x_val, int y_val) : x(x_val), y(y_val){}
//     cords(){} //welp hope this never causes an issue
// }; 
// enum class PAGE { MAIN_MENU, IN_GAME }; // moved to drawMenu.hpp (ik but this was the easiest i'll fix someday) 

class sharad {
public:
    sharad();
    ~sharad();
    void run();
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mouseClicked(const MEVENT &event);

    PAGE currentPage;

private: //just works ignore - initialised in sharad.cpp cuz thats how Open Frameworks does it ig.
    uint64_t frameCount = 0;
    bool running;
    std::queue<int> inputQueue;
    cords screenSize, minScreenSize;
    std::vector<std::vector <std::u32string>> Menu;
    std::pair<int, std::vector<Widget*>> focusElements;
    
    drawMenu rootMenu; 
    drawMenu settingsMenu;
    drawMenu creditsMenu; 
};