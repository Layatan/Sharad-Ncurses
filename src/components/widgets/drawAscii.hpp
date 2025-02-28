#pragma once

#include <string_view>
#include <string>
#include <codecvt>
#include "Widget.hpp"

class drawAscii : public Widget {
public:
    drawAscii(int x, int y, int width, int height)
        : Widget(x, y, width, height) {}
    drawAscii(int x, int y, cords size)
        : Widget(x, y, size.x, size.y) {}
    drawAscii(cords pos, int width, int height) 
        : Widget(pos.x, pos.y, width, height) {}
    drawAscii(cords pos, cords size) 
        : Widget(pos.x, pos.y, size.x, size.y) {}


    void draw(std::u32string art, bool useRGB = false) {
        const static bool canChangeColor = can_change_color();
        static short rCol = 1000, bCol = 0, gCol = 0, pairIndex = 1;
        werase(win);
        // box(win, 0, 0); // extra space when using some unicode, i assume cus wide char... anyway they need an extra 1 to the width 
        if (useRGB && canChangeColor) {
            // if (bCol != 0 && rCol == 1000) bCol -= rgbStep;
            // else if (rCol == 1000 && gCol != 1000)  gCol += rgbStep;
            // else if (rCol != 0 && gCol == 1000) rCol -= rgbStep;
            // else if (gCol == 1000 && bCol != 1000) bCol += rgbStep;
            // else if (gCol != 0 && bCol == 1000) gCol -= rgbStep;
            // else if (bCol == 1000 && rCol != 1000) rCol += rgbStep;
    
            // init_color(8, 876, 375, 555);
            // init_pair(3, 8, COLOR_RED);
            // refresh();
            // for (char32_t c : art) {
            //     if (c == U'\n') 
            //         wprintw(win, "%s", convU32_U8.to_bytes(c).c_str());
            //     else
            //         wprintw(win, "%s", convU32_U8.to_bytes(U'A').c_str());
            // }
            // attron(COLOR_PAIR(1));
            printw("r:%d,g:%d,b:%d, COL:%d", rCol, gCol, bCol, COLORS);
            // attroff(COLOR_PAIR(2));
        }
        else { //your term can't change colour you bitch boy loser hahahahahah >:)
            wprintw(win, "%s", convU32_U8.to_bytes(art).c_str());
        } 

        wrefresh(win);
    }
    int keyPressed(int key) override { return key; }
protected:
const int rgbStep = 20;
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU32_U8;
};
