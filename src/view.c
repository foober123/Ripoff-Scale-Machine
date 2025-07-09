#include <ncurses.h>
#include "model.h"
#include "config.h"
#include <string.h>

void listNotes(Model* model, screenInfo* screenInfo){
    const char *title = "Notes:";
    int height = 17, width = 10, starty = (screenInfo->maxY-height)/3, startx = (screenInfo->maxX-width)/2;
    WINDOW *win = newwin(height, width, starty, startx);
    mvwprintw(win, height-15, (width-strlen(title))/2, "%s", title);
    box(win, 0, 0);
    for (int i = 0; i < 12; i++) {
        mvwprintw(win, 3+i, (width-strlen(CHROMATICSCALE[model->noteOrder[i]]) )/2, "%s", CHROMATICSCALE[model->noteOrder[i]] );
    
    }


    wrefresh(win);    
    delwin(win);

}

void drawHelpBar(screenInfo* screenInfo){
    attron(A_REVERSE);
    mvprintw(screenInfo->maxY - 3, 1, "%c", KEY_NOTE);
    mvprintw(screenInfo->maxY - 2, 1, "%c", KEY_QUIT);
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/6, "%c", KEY_TIME);
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/6, "%c", KEY_RESET_TIME);
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/3, "%c", KEY_LIST);
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/3, "%c", KEY_CYCLE_PARENT);
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/2, "%c", KEY_CYCLE_ROOT);
    attroff(A_REVERSE);

    mvprintw(screenInfo->maxY - 3, 3, "Next Note");
    mvprintw(screenInfo->maxY - 2, 3, "Quit");
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/6 + 2, "Time");
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/6 + 2, "Reset");
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/3 + 2, "List Notes");
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/3 + 2, "Cycle Scale");
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/2 + 2, "Cycle Root Note");

}

void drawNoteCount(Model *model, screenInfo* screenInfo){
        move(screenInfo->maxY / 6, screenInfo->maxX / 4 - 2);
        clrtoeol();
        move(screenInfo->maxY / 6, (screenInfo->maxX - strlen(CHROMATICSCALE[model->noteOrder[model->noteCounter]])) - screenInfo->maxX/15);
        clrtoeol();

        attron(COLOR_PAIR(1));
        mvprintw(screenInfo->maxY / 6, (screenInfo->maxX - strlen(CHROMATICSCALE[model->noteOrder[model->noteCounter]]) - screenInfo->maxX/15), "%s", CHROMATICSCALE[model->noteOrder[model->noteCounter]]);
        if(model->noteCounter < 9){
            mvprintw(screenInfo->maxY / 6, screenInfo->maxX / 2 - 6, "%d", model->noteCounter + 1);
        }
        else{
            mvprintw(screenInfo->maxY / 6, screenInfo->maxX / 2 - 7, "%d", model->noteCounter + 1);
        }
        attroff(COLOR_PAIR(1));
        mvprintw(screenInfo->maxY / 6, (screenInfo->maxX-strlen("out of 12")) / 2, "out of 12");

}

void drawParentScale(Model *model, screenInfo* screenInfo){

        move(screenInfo->maxY / 4, screenInfo->maxX/6);
        clrtoeol();
        move(screenInfo->maxY / 4 + 1, screenInfo->maxX / 2 - 6);
        clrtoeol();

        mvprintw(screenInfo->maxY / 4, (screenInfo->maxX-strlen("out of 7")) / 2, "out of 7");
        attron(COLOR_PAIR(1));
        mvprintw(screenInfo->maxY / 4, (screenInfo->maxX - strlen(CHROMATICSCALE[model->parentScaleOrder[model->parentScaleOrderIndex]]) - screenInfo->maxX/15), "%s", CHROMATICSCALE[model->parentScaleOrder[model->parentScaleOrderIndex]]);
        mvprintw(screenInfo->maxY / 4, screenInfo->maxX / 2 - 6, "%d", model->parentScaleOrderIndex + 1);
        mvprintw(screenInfo->maxY / 4 + 1, (screenInfo->maxX-strlen(CHROMATICSCALE[model->rootNote])) / 2 - 1, "%s", CHROMATICSCALE[model->rootNote]);

        attroff(COLOR_PAIR(1));
}

void drawStatics(screenInfo* screenInfo){

    char *title = "Ripoff Scale Machine";
    int title_length = strlen(title);


    mvprintw(1, (screenInfo->maxX - title_length) / 2, "%s", title);
        
    mvprintw(screenInfo->maxY / 6, screenInfo->maxX / 15, "Select from 12");
    mvprintw(screenInfo->maxY / 6 + 1, screenInfo->maxX / 15, "Possible keys");

    mvprintw(screenInfo->maxY / 4, screenInfo->maxX / 15, "Notes from a");
    mvprintw(screenInfo->maxY / 4 + 1, screenInfo->maxX / 15, "Parent Scale");
}

void drawStopwatch(Model* model, screenInfo* screenInfo){
    double elapsed = get_stopwatch_time(model);
    int mins = (int)(elapsed / 60);
    int secs = (int)elapsed % 60;
    int centisecs = (int)((elapsed - (int)elapsed) * 100);
    mvprintw(3, screenInfo->maxX - strlen("Stopwatch: %02d:%02d.%02d") , "Stopwatch: %02d:%02d.%02d", mins, secs, centisecs);

}

void drawDebuggingValues(Model* model){
   mvprintw(9, 9, "%d", model->noteCounter);

}

void render(Model* model, screenInfo* screenInfo) {
    //drawDebuggingValues(model);    
    drawStatics(screenInfo);
    drawStopwatch(model, screenInfo);

    if(model->toggleList){
    listNotes(model, screenInfo);
    }

    if(!model->toggleList){

        if (model->isNotePressed) {
        drawNoteCount(model, screenInfo);
        }
    
        if (model->isScaleShown){
        drawParentScale(model, screenInfo);
        }

    }



    drawHelpBar(screenInfo);

    refresh();
}

