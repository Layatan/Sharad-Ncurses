#pragma once
#include <ncurses.h>

#include "components/tools.hpp"
#include "components/ui/ascii_art.hpp"
#include "components/widgets/drawAscii.hpp"
#include "components/widgets/drawMenu.hpp"
#include "components/ui/credits.hpp"

#include <iostream>
#include <queue>

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
