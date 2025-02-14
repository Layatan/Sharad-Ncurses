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


    void draw(std::u32string art) {
        werase(win);
        // box(win, 0, 0); // extra space when using some unicode, i assume cus wide char... anyway they need an extra 1 to the width 
        wprintw(win, "%s", convU32_U8.to_bytes(art).c_str());

        wrefresh(win);
    }
    int keyPressed(int key) override { return key; }
protected:
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU32_U8;
};
