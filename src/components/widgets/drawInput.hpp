#pragma once

#include <string_view>
#include <string>
#include <codecvt>
#include "Widget.hpp"

class drawInput : public Widget {
public:
    drawInput(std::u32string header, std::u32string input = U"", int width = 5, int height = 1)
        : title(header), data(input), Widget(0, 0, width, height) {}

    void draw(int col = -1) {
        werase(win);
        if (col != -1) wattroff(win, COLOR_PAIR(col))

        std::u32string toDraw;
        if (data.length() > 0) toDraw = data;
        else toDraw = title;



        mvwprintw(win, 0, 1, "%s", convU32_U8.to_bytes(toDraw).c_str());

        if (col != -1) wattroff(win, COLOR_PAIR(col))
        wrefresh(win);
    }
    int keyPressed(int key) override { return key; }
protected:
std::u32string data, title;
bool isActive = false;
std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convU32_U8;
};
