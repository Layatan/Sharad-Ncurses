#include "sharad.hpp"

sharad::sharad() : running(false) {}
sharad::~sharad() { endwin(); }

int convertRGB(int value);

void sharad::setup(){
    // Initialize ncurses
    setlocale(LC_ALL, "");
    initscr();
    getmaxyx(stdscr, screenSize.y, screenSize.x);
    start_color();
    use_default_colors();
    noecho();
    cbreak();
    curs_set(0);
    timeout(0); // Non-blocking getch()
    keypad(stdscr, TRUE); // Enable keypad for special keys
    mousemask(ALL_MOUSE_EVENTS, NULL); // Enable all mouse events
    nodelay(stdscr, TRUE); // Make getch() non-blocking
    running = true; // keep don't del
    frameCount = 0;

    currentPage = PAGE::MAIN_MENU;
    minScreenSize = {190, 50};

    Menu.push_back({U"Continue ", U"Start New Game", U"Load Game", U"Options", U"Quit Game... Bye Chummer"});
    Menu.push_back({U"Game Settings", U"Theme", U"Mandem Involved <3"}); //setting
    Menu.push_back({gameCredits});

    drawMenu tmproot = {0, 0, &Menu[0], true};
    rootMenu = tmproot; //not great but this is the three stage initialisation required. Now to do this(kinda) for all of em
    rootMenu.draw(0, screenSize.y-13); //otherwise wrong location - no it cant be set on tmp declaration (try it if you dotn believe me)

    drawMenu tmpsett = {0, 0, &Menu[1], true, &rootMenu};
    settingsMenu = tmpsett;

    drawMenu tmpcred = {0, 0, &Menu[2], false, &settingsMenu};
    creditsMenu = tmpcred;



    //all the page linkages. int is index of menu option and pointer is where it leads to
    rootMenu.linkExecute(3, &settingsMenu);

    settingsMenu.linkExecute(2, &creditsMenu);

    focusElements.second.push_back(&rootMenu);
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

            if (frameCount%60 < 30) {
                drawAscii titleCursorBox = {screenSize.x-asciiArt::titleCursor.width-45, 11, asciiArt::titleCursor.width, asciiArt::titleCursor.height};
                titleCursorBox.draw(asciiArt::titleCursor.artwork);
            }
            // mvprintw(17,screenSize.x-asciiArt::title.width+25,"based on: Sharad Ratatui by ProHallad.");

            rootMenu.draw();


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
        napms(16); //roughly 60fps apparently
    }
    
}