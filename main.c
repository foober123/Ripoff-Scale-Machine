#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>


volatile sig_atomic_t resize_flag = 0;
void handle_winch(int sig) {
    resize_flag = 1;
}

typedef enum{
keyQuit = (int)'q',
keyNote = (int)'k',
keyTime = (int)'t',
keyResetTime = (int)'r',
keyList = (int)'l'

} controlKey ;

typedef enum {
    noteON = 1,
    noteOFF = 0
} notePressState ;

typedef enum {
    stopwatchON = 1,
    stopwatchOFF = 0
} stopwatchRunning ;

typedef enum {
    listON = 1,
    listOFF = 0
} listToggle;

typedef struct {
    controlKey keybind;
    notePressState isKpressed;
    stopwatchRunning stopwatch_running;
    listToggle toggleList;

    int NoteOrder[12];
    int NoteCounter;
    
    const char* notes[12];

    struct timespec stopwatch_start;
    double stopwatch_elapsed;
} Model;

typedef struct{
int maxY;
int maxX;

} screenInfo;



void generatePattern(int* arr, int size){
    for (int i = size - 1;i > 0;i--){
    int j = rand() % (i + 1);

 
    int temp = arr[i];  
    arr[i] = arr[j];    
    arr[j] = temp;      
    }

}

void initModel(Model* model) {
    for (int i = 0; i < 12; i++) {
        model->NoteOrder[i] = i;
    }
    model->NoteCounter = 0;
    model->isKpressed = noteOFF;
    model->toggleList = listOFF;

    const char* notesTemp[12] = {
        "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"
    };
    memcpy(model->notes, notesTemp, sizeof(notesTemp));

    generatePattern(model->NoteOrder, 12);


    model->stopwatch_running = 0;
    model->stopwatch_elapsed = 0.0;
}

void initScreenInfo(screenInfo* screenInfo) {
    getmaxyx(stdscr, screenInfo->maxY, screenInfo->maxX);
}

double get_time_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

double get_stopwatch_time(Model* model) {
    if (model->stopwatch_running) {
        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);
        return model->stopwatch_elapsed +
            (now.tv_sec - model->stopwatch_start.tv_sec) +
            (now.tv_nsec - model->stopwatch_start.tv_nsec) / 1e9;
    } else {
        return model->stopwatch_elapsed;
    }
}

void listNotes(Model* model, screenInfo* screenInfo){
    int maxY, maxX;
    const char *title = "Notes:";
    int height = 17, width = 10, starty = (screenInfo->maxY-height)/3, startx = (screenInfo->maxX-width)/2;
    WINDOW *win = newwin(height, width, starty, startx);
    mvwprintw(win, height-15, (width-strlen(title))/2, "%s", title);
    box(win, 0, 0);
    for (int i = 0; i < 12; i++) {
        mvwprintw(win, 3+i, (width-strlen(model->notes[model->NoteOrder[i]]) )/2, "%s", model->notes[model->NoteOrder[i]] );
    
    }


    wrefresh(win);    
    delwin(win);

}

void drawHelpBar(screenInfo* screenInfo){
    attron(A_REVERSE);
    mvprintw(screenInfo->maxY - 3, 1, "%c", keyNote);
    mvprintw(screenInfo->maxY - 2, 1, "%c", keyQuit);
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/6, "%c", keyTime);
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/6, "%c", keyResetTime);
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/3, "%c", keyList);
    attroff(A_REVERSE);

    mvprintw(screenInfo->maxY - 3, 3, "Next Note");
    mvprintw(screenInfo->maxY - 2, 3, "Quit");
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/6 + 2, "Time");
    mvprintw(screenInfo->maxY - 2, screenInfo->maxX/6 + 2, "Reset");
    mvprintw(screenInfo->maxY - 3, screenInfo->maxX/3 + 2, "List Notes");

}

void drawNoteCount(Model *model, screenInfo* screenInfo){
        move(screenInfo->maxY / 6, screenInfo->maxX / 4 - 2);
        clrtoeol();
        move(screenInfo->maxY / 6, (screenInfo->maxX - strlen(model->notes[model->NoteOrder[model->NoteCounter]])) - screenInfo->maxX/20);
        clrtoeol();

        attron(COLOR_PAIR(1));
        mvprintw(screenInfo->maxY / 6, (screenInfo->maxX - strlen(model->notes[model->NoteOrder[model->NoteCounter]]) - screenInfo->maxX/20), "%s", model->notes[model->NoteOrder[model->NoteCounter]]);
        mvprintw(screenInfo->maxY / 6, screenInfo->maxX / 2 - 7, "%d", model->NoteCounter + 1);
        attroff(COLOR_PAIR(1));
        mvprintw(screenInfo->maxY / 6, (screenInfo->maxX-strlen("out of 12")) / 2, "out of 12");

}

void drawStatics(screenInfo* screenInfo){

    char *title = "Ripoff Scale Machine";
    int title_length = strlen(title);


    mvprintw(1, (screenInfo->maxX - title_length) / 2, "%s", title);
    mvprintw(screenInfo->maxY / 6, screenInfo->maxX / 20, "Select From 12");
    mvprintw(screenInfo->maxY / 6 + 1, screenInfo->maxX / 20, "Possible Keys");
}

void drawStopwatch(Model* model, screenInfo* screenInfo){
    double elapsed = get_stopwatch_time(model);
    int mins = (int)(elapsed / 60);
    int secs = (int)elapsed % 60;
    int centisecs = (int)((elapsed - (int)elapsed) * 100);
    mvprintw(3, screenInfo->maxX - strlen("Stopwatch: %02d:%02d.%02d") , "Stopwatch: %02d:%02d.%02d", mins, secs, centisecs);

}

void render(Model* model, screenInfo* screenInfo) {
    drawStatics(screenInfo);
    drawStopwatch(model, screenInfo);

    if (model->isKpressed) {
    drawNoteCount(model, screenInfo);
    }

    drawHelpBar(screenInfo);

    if(model->toggleList){
    listNotes(model, screenInfo);
    }

    refresh();
}

void update(Model* model, int input) {

    switch (input) {
        case keyNote:
                if (model->isKpressed == noteOFF) {
                model->NoteCounter = 0;
                model->isKpressed = noteON;
            } else {

                if (model->NoteCounter < 11) {
                    model->NoteCounter++;
                } else {
                    model->NoteCounter = 0;
                    generatePattern(model->NoteOrder, 12);
                }
            }
            break;
         case keyTime:
            if (model->stopwatch_running == stopwatchOFF) {
            clock_gettime(CLOCK_MONOTONIC, &model->stopwatch_start);
            model->stopwatch_running = stopwatchON;
            } else {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            model->stopwatch_elapsed +=
                (now.tv_sec - model->stopwatch_start.tv_sec) +
                (now.tv_nsec - model->stopwatch_start.tv_nsec) / 1e9;
            model->stopwatch_running = 0;
            }
            break;
        case keyResetTime:
            model->stopwatch_running = 0;
            model->stopwatch_elapsed = 0;
            break;
            case keyList:
            generatePattern(model->NoteOrder, 12);
            model->isKpressed = 0;
            model->toggleList = !model->toggleList;
            if(model->toggleList == 0){
            clear();
            } 
            break;
        default:
            break;
    }
}

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
    while (input != keyQuit) {
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
    }

    endwin();
    return 0;
}



