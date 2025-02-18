#include "sharad.hpp"

sharad::sharad() : running(false) {}
sharad::~sharad() { std::cout << "Bye Chummer...\n"; }

int convertRGB(int value);

void sharad::setup(){
    running = true; // keep don't del
    frameCount = 0;

    currentPage = PAGE::MAIN_MENU;
    minScreenSize = {190, 50};

    Menu.push_back({U"Continue ", U"Start New Game", U"Load Game", U"Options", U"Quit Game... Bye Chummer"});//main
    Menu.push_back({U"Game Settings", U"Theme", U"Mandem Involved <3"}); //settings
    
    Menu.push_back({asciiArt::credits.artwork}); //credits
    Menu.push_back({U"Cycle Setting^Setting1^Setting2^Setting3", U"Toggle Setting^On^Off"});

    drawMenu tmproot = {&Menu[0], true};
    rootMenu = tmproot; //not great but this is the three stage initialisation required. Now to do this(kinda) for all of em

    drawMenu tmpsett = {&Menu[1], true, &rootMenu};
    settingsMenu = tmpsett;

    drawMenu tmpcred = {&Menu[2], false, &settingsMenu};
    creditsMenu = tmpcred;

    drawMenu tmpgsett = {&Menu[3], true, &settingsMenu};
    gameSettings = tmpgsett;

    //all the page linkages. int is index of menu option and pointer is where it leads to
    rootMenu.linkExecute(3, &settingsMenu);
    rootMenu.linkExecute(0, PAGE::IN_GAME);

    settingsMenu.linkExecute(2, &creditsMenu);
    settingsMenu.linkExecute(0, &gameSettings);

    focusElements.second.push_back(&rootMenu); //n main menu focus on main rootmenu
    focusElements.first = 0;
}

void sharad::update(){
    int ch = getch(); //input handlind
    while (ch != ERR) {
        inputQueue.push(ch); // Buffer input
        ch = getch();
    }

    while (!inputQueue.empty()) {
        int key = inputQueue.front();
        inputQueue.pop();
        if (key == KEY_MOUSE) {
            MEVENT event;
            if (getmouse(&event) == OK) 
            mouseClicked(event);
        } 
        else keyPressed(key);
    }
    
    
}

void sharad::draw(){
    clear();

    if (screenSize.x >= minScreenSize.x && screenSize.y >= minScreenSize.y) {
        refresh();
        switch (currentPage) {
        case PAGE::MAIN_MENU: {
            
            drawAscii logoBox = {1, 1, asciiArt::logo.width, asciiArt::logo.height};
            logoBox.draw(asciiArt::logo.artwork);

            drawAscii titleBox = {screenSize.x-asciiArt::title.width-2, 2, asciiArt::title.width, asciiArt::title.height};
            titleBox.draw(asciiArt::title.artwork);

            if (frameCount%60 < 30) { //blinking cursor
                drawAscii titleCursorBox = {screenSize.x-asciiArt::titleCursor.width-45, 11, asciiArt::titleCursor.width, asciiArt::titleCursor.height};
                titleCursorBox.draw(asciiArt::titleCursor.artwork);
            }

            rootMenu.draw(0, screenSize.y - rootMenu.getSize().y, true);


        } break;
        case PAGE::IN_GAME: {

        } break;
        }
    }
    else {
        printw("Frame: %ld", frameCount);
        if (frameCount%60 < 30) { // 2/3 of frames it blinks on and for 1/3 its off
            init_pair(1, -1, COLOR_RED);
            attron(COLOR_PAIR(1));
            std::string softErr = "Terminal window is too small! Zoom Out [ctrl -/+]"; //declared for ez centering
            mvprintw(screenSize.y/2-1, (screenSize.x - softErr.length())/2, "%s" , softErr.c_str());
            attroff(COLOR_PAIR(1));
        }
    }

    refresh();
}

void sharad::keyPressed(int key){

    switch (key) {
    case 'q':
        running = false;
        break;
    case '\t': // 9 works too cuz ascii? idk bro
        if (focusElements.first+2 >  focusElements.second.size()) { //left is index and right is vector size, +1 to align another +1 to check if there are enough 
            focusElements.second.size() != 0 ? focusElements.first = 0 : focusElements.first = -1;
        }
    break;
    default:
        int maybeQuit = focusElements.second[focusElements.first]->keyPressed(key);
        if (maybeQuit == -1) running = false;
        
        printw("Pressed: %c, %d\n", key, key); //goes away too fast to see
        break;
    }
}

void sharad::mouseClicked(const MEVENT& event) {
    printw("Mouse clicked at (%d, %d)\n", event.x, event.y);
    if (event.bstate & BUTTON1_PRESSED) {
        printw("Left button pressed.\n");
    }
    if (event.bstate & BUTTON1_RELEASED) {
        printw("Left button released.\n");
    }
    if (event.bstate & BUTTON2_CLICKED) {
        printw("Right button pressed.\n");
    }
    if (event.bstate & BUTTON2_RELEASED) {
        printw("Right button released.\n");
    }
    
}

void sharad::run(){
    setup();

    while (running){
        getmaxyx(stdscr, screenSize.y, screenSize.x);

        update();
        draw();
        
        frameCount++;
        napms(16); //roughly 60fps apparently, according to copilot helps not miss input
    }
}