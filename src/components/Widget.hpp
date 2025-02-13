#pragma once

#include <ncurses.h>
#include <string>

struct cords { // used for sizing and location (su mi :p). ncurses goes y then x so i ought to follow but i have morals so no
    int x = 0, y = 0; //may as well set defaults 
    cords(int x_val, int y_val) : x(x_val), y(y_val){}
    cords(){} //welp hope this never causes an issue
};

class Widget {
public:
    Widget(int arg_x, int arg_y, int arg_width, int arg_height): x(arg_x), y(arg_y), width(arg_width), height(arg_height) {
        win = newwin(height, width, y, x);
    }
    virtual ~Widget() { delwin(win); }
    
    Widget(){};
    Widget(const Widget& other) 
        : win(other.win), x(other.x), y(other.y), width(other.width), height(other.height){}
    Widget& operator=(const Widget& other){
        if (this == &other) return *this;
        // win = other.win; //seg faults idk why
        x = other.x;
        y = other.y;
        width = other.width;
        height = other.height;
        win = newwin(height, width, y, x);
        return *this;
    }

    cords getStart(){
        cords toRet = {x, y};
        return toRet;
    }
    cords getSize(){
        cords toRet = {width, height};
        return toRet;
    }
    virtual int keyPressed(int key) = 0;
protected:
    WINDOW* win; // ncurses window for the widget
    int x=0, y=0, width=0, height=0;
};