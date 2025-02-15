#pragma once

struct cords { //moved to widget.hpp 
    int x = 0, y = 0; //may as well set defaults 
    cords(int x_val, int y_val) : x(x_val), y(y_val){}
    cords(){} //welp hope this never causes an issue
}; 

enum class PAGE { MAIN_MENU, IN_GAME }; // moved to drawMenu.hpp (ik but this was the easiest i'll fix someday) 

extern PAGE currentPage;
extern uint64_t frameCount;
extern cords screenSize;