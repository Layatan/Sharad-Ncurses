#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>
#include "Widget.hpp"

enum class PAGE { MAIN_MENU, IN_GAME };

class drawMenu : public Widget {
public:
    drawMenu(int x, int y, std::vector<std::u32string>* MenuOptions, bool useSelection = false, drawMenu* summonedBy = nullptr)
        : Widget(x, y, 0, 3) { 
            pointerMenu = MenuOptions;

            for (auto entry : *pointerMenu) {
                int curr = 0;
                for (char32_t c : entry) {
                    if (c == U'\n'){
                        if (curr > width) width = curr;
                        height++;
                        curr = 0;
                    } else curr++;
                }
                if (curr > width) width = curr;
                height += 2; //two per entry, writing and bottom spacer (below the original count on widget initialisation)
            }
            int alignSpacer = 4 /* left aligning */, centerSpacer = 3; /* actual border/frame spacer */
            width += (alignSpacer+centerSpacer*2);
            
            toggleSelection(useSelection);
            wresize(win, height, width);

            //subsequent menus are anchored, tl until overflow then bl (can be better implemented :/)
            if (summonedBy != nullptr) {
                cords getPrevStart = (*summonedBy).getStart(); 
                cords getPrevSize = (*summonedBy).getSize();
                cords screenSize; getmaxyx(stdscr, screenSize.y, screenSize.x);
                
                x = getPrevStart.x+getPrevSize.x+2;
                if (getPrevStart.y + height <= screenSize.y) 
                    y = getPrevStart.y;
                else y = screenSize.y - height;
                
                prevLink = summonedBy;
    
                draw(x, y); //maybe i'm just tires but i'm getting to the point shit stops making sense and i'm taking shortcuts
            }
        }
    drawMenu(cords pos, std::vector<std::u32string>*MenuOptions) 
        : drawMenu(pos.x, pos.y, MenuOptions) {}
    
    drawMenu(): Widget(){}; //handling setting one drawMenu to another
    drawMenu(const drawMenu& other) 
        : selection(other.selection), prevLink(other.prevLink), nextLink(other.nextLink),
            pointerMenu(other.pointerMenu) {}
    drawMenu& operator=(const drawMenu& other){
        if (this == &other) return *this;
        Widget::operator=(other);
        selection = other.selection;
        prevLink = other.prevLink;
        nextLink = other.nextLink;
        pointerMenu = other.pointerMenu;
        return *this;
    }


    void draw(int drawX = 8008135, int drawY = 8008135){ //i cant set the default value to x and y wtf man
        werase(win);

        static uint8_t blinker = 0;
        blinker == 60 ? blinker = 0 : blinker++; 
        
        if(drawX != 8008135 || drawY != 8008135) {
            x = drawX; y = drawY;
            delwin(win);
            win = newwin(height, width, y, x); 
        }

        // box(win, 0, 0); //debug
        // mvwprintw(win,1,1, "h:%d w:%d x:%d y:%d",height, width, x, y); //debug
        // mvprintw(0,0, "h:%d w:%d x:%d y:%d",height, width, x, y); //debug
        // mvwprintw(win,1,1, "selection:%d count:%ld",selection, (*pointerMenu).size());

        mvwprintw(win, 0, 0, "╔══");
        mvwprintw(win, 0, width-3, "══╗");
        mvwprintw(win, height-1, 0, "╚══");
        mvwprintw(win, height-1, width-3, "══╝");
        
        int currSpacer = 2;
        for (int i = 0; i < (*pointerMenu).size(); i++){
            auto entry = (*pointerMenu)[i];
            if (i == selection){
                init_pair(1, COLOR_YELLOW, -1);
                wattron(win, COLOR_PAIR(1));
                if (blinker < 40){
                    if (nextLink == nullptr || (*nextLink).hasSelection() == false) {
                        mvwprintw(win, currSpacer+i*2-1, 3, "┎");
                        mvwprintw(win, currSpacer+i*2+1, 3, "┗─");
                    }
                    
                    entry = U"█ " + entry;
                } else entry = U"█ ";
            }

            mvwprintw(win, currSpacer+i*2, 3, "%s", convU32_U8.to_bytes(entry).c_str());
            currSpacer += std::count(entry.begin(), entry.end(), '\n');
            
            wattroff(win, COLOR_PAIR(1));
        }
        
        if (nextLink != nullptr) {
            (*nextLink).draw();
        }
        

        wrefresh(win);
    }

    int keyPressed(int key) override {
        int optionCount = (*pointerMenu).size();
        switch (key) {
        case KEY_UP: case KEY_DOWN:{
            if (selection == -1) break;
            if (nextLink != nullptr && (*nextLink).hasSelection()) {
                (*nextLink).keyPressed(key);
                break;
            }

            if (key == KEY_DOWN) 
                selection+2 > optionCount ? selection = 0 : selection++;
            else 
                selection == 0 ? selection = optionCount-1 : selection--;
        } break;
        case KEY_RIGHT: case '\n': {
            if (nextLink != nullptr){
                (*nextLink).keyPressed(key);
                break; //not current focus
            }
            if (prevLink == nullptr && selection == optionCount-1) {
                key = -1; //quit if on quit
            }
            doExcecute();
        } break;
        case KEY_LEFT: case 27 /* escape key but it lags it idk why */: {
            // if (prevLink != nullptr) { //maybe i'm just tires but this logic makes no sense. 
            //     break;   //think i meant to do if on main menu ignore back input but i needed it afterwards
            // }
            if (prevLink == nullptr && selection == optionCount-1) key = -1; //quit if on quit
            if (nextLink != nullptr) {
                if((*nextLink).hasNext() && (*nextLink).hasSelection() == false)
                    (*nextLink).keyPressed(key);
                else nextLink = nullptr;
                break;
            }

        } break;
        default:
            break;
        }
        
        return key; //success or fail (-1) (emergency quit)
    }

    void linkExecute(int index, drawMenu* menuToOpen){
        executeNewMenu.push_back(std::make_pair(index, menuToOpen));
    }
    void linkExecute(int index, PAGE pageToOpen){
        
    }
    void doExcecute(){
        for (auto toCheck: executeNewMenu) {
            if (selection == toCheck.first) {
                nextLink =  toCheck.second;
            }
        }
    }

    void toggleSelection(bool useSelection){
        useSelection ? selection = 0 : selection = -1;
    }
    bool hasSelection(){
        if (selection == -1 ) return false;
        else return true;
    }
    bool hasNext(){
        bool toRet = false;
        nextLink == nullptr ? toRet = false : toRet = true;
        return toRet;
    }
protected:
    int selection = -1;
    drawMenu* prevLink = nullptr; drawMenu* nextLink = nullptr;
    std::vector<std::pair<int, drawMenu*>> executeNewMenu; //takes index and next drawMenu pointer
    std::vector<std::pair<int, drawMenu*>> executeNewPage; //really only one page but i'd like room to grow in case
    std::vector<std::u32string>* pointerMenu;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU32_U8; //If used one more time move to Widget
};