#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <codecvt>

#include "Widget.hpp"


class drawMenu : public Widget {
public:
    drawMenu(std::vector<std::u32string>* MenuOptions, bool useSelection = true, drawMenu* summonedBy = nullptr)
        : pointerMenu(MenuOptions), Widget(0, 0, 0, 3) { 

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
            
            prevLink = summonedBy;
            toggleSelection(useSelection);
            wresize(win, height, width);
        }
    
    drawMenu(): Widget(){}; 
    drawMenu& operator=(const drawMenu& other){
        if (this == &other) return *this;
        Widget::operator=(other);
        selection = other.selection;
        prevLink = other.prevLink;
        nextLink = other.nextLink;
        pointerMenu = other.pointerMenu;
        return *this;
    }


    void draw(int AnchorX, int AnchorY, bool forceRedraw = false){ //i cant set the default value to x and y wtf man
        werase(win);
        
        if (AnchorX != x || AnchorY != y) {
            if (forceRedraw || prevLink != nullptr) { 
                x = AnchorX;
                AnchorY + height < screenSize.y ? y = AnchorY : y = screenSize.y - height; //actively make sure new draw menu's don't overflow bottom 

                delwin(win);
                win = newwin(height, width, y, x);
            }
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
                init_pair(2, COLOR_YELLOW, -1);
                wattron(win, COLOR_PAIR(2));

                frameCount%60 < 30 ? entry = U"█ " + entry : entry = U"│ " + entry;
                if (nextLink == nullptr || (*nextLink).hasSelection() == false) {
                    mvwprintw(win, i*2+currSpacer-1, 3, "┎");
                    mvwprintw(win, i*2+currSpacer+1, 3, "┗─");
                }
            }

            // currSpacer += std::count(entry.begin(), entry.end(), '\n'); //incase any multi-lines
            mvwprintw(win, i*2+currSpacer, 3, "%s", convU32_U8.to_bytes(entry).c_str());
            wattroff(win, COLOR_PAIR(2));
            //trippy
            
        }
        
        if (nextLink != nullptr) {
            (*nextLink).draw(x+width+2, y);
        }
        

        wrefresh(win);
    }

    int keyPressed(int key) override {
        int optionCount = (*pointerMenu).size();
        switch (key) {
        case KEY_UP: case KEY_DOWN:{
            // if (selection == -1) break; //doubt it does anything anymore
            if (nextLink != nullptr && (*nextLink).hasSelection()) {
                (*nextLink).keyPressed(key);
                break;
            }

            if (nextLink != nullptr && (*nextLink).hasSelection() == false) nextLink = nullptr;
            if (key == KEY_DOWN) selection+2 > optionCount ? selection = 0 : selection++;
            else selection == 0 ? selection = optionCount-1 : selection--;
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
        case KEY_LEFT: case KEY_BACKSPACE: { //escape key lags (for me) so backspace
            if (prevLink == nullptr && selection == optionCount-1) { //quiting
                key = -1;
                break;
            } //quit if on quit
            if (nextLink != nullptr && (*nextLink).hasNext() == false){
                nextLink = nullptr;
            }
            else if (nextLink != nullptr && (*nextLink).hasSelection()) {
                // (*pointerMenu)[0] = U"Tried";
                (*nextLink).keyPressed(key);
            }
            else if (nextLink != nullptr && (*nextLink).hasSelection() == false) nextLink = nullptr;
            else nextLink = nullptr;

        } break;
        default:
            break;
        }
        
        return key; // return success or fail/quit (-1)
    }

    void linkExecute(int index, drawMenu* menuToOpen){
        executeNewMenu.push_back(std::make_pair(index, menuToOpen));
    }
    void linkExecute(int index, PAGE pageToOpen){
        executeNewPage.push_back(std::make_pair(index, pageToOpen));
    }
    void doExcecute(){
        for (auto toCheck: executeNewMenu) {
            if (selection == toCheck.first) {
                nextLink =  toCheck.second;
                return;
            }
        }
        for (auto toCheck: executeNewPage) {
            if (selection == toCheck.first) {
                currentPage =  toCheck.second;
                return;
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
    int selection = 0;
    drawMenu* prevLink = nullptr; drawMenu* nextLink = nullptr;
    std::vector<std::pair<int, drawMenu*>> executeNewMenu; //takes index and next drawMenu pointer
    std::vector<std::pair<int, PAGE>> executeNewPage; //really only one page but i'd like room to grow in case
    std::vector<std::u32string>* pointerMenu;
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU32_U8; //If used one more time move to Widget
};