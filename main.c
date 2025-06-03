#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "config.h"


volatile sig_atomic_t resize_flag = 0;


typedef struct{
int maxY;
int maxX;

} screenInfo;



void handle_winch(int sig) {
    resize_flag = 1;
}

#include "model.c"
#include "view.c"
#include "controller.c"

int main() {
    srand(time(NULL));

    screenInfo screenInfo;
    Model model;
    initModel(&model);
    

    initscr();
    initScreenInfo(&screenInfo);

    start_color();
    use_default_colors();
    init_pair(1, COLOR_RED, -1);

    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    signal(SIGWINCH, handle_winch);

    int input = 0;
    while (input != KEY_QUIT) {
        if(resize_flag){
            resize_flag = 0;
            endwin();
            refresh();
            clear();
            initScreenInfo(&screenInfo);

        }
        render(&model, &screenInfo);
        input = getch();
        update(&model, input);
        usleep(FRAME_DELAY_US);
    }

    endwin();

    return 0;
}



