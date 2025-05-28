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

typedef struct {
    int NoteOrder[12];
    int NoteCounter;
    int isKpressed;
    const char* notes[12];

    int stopwatch_running;
    struct timespec stopwatch_start;
    double stopwatch_elapsed;
} Model;




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
    model->isKpressed = 0;

    const char* notesTemp[12] = {
        "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"
    };
    memcpy(model->notes, notesTemp, sizeof(notesTemp));

    generatePattern(model->NoteOrder, 12);


    model->stopwatch_running = 0;
    model->stopwatch_elapsed = 0.0;
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


void render(Model* model) {
    double elapsed = get_stopwatch_time(model);
    int mins = (int)(elapsed / 60);
    int secs = (int)elapsed % 60;
    int centisecs = (int)((elapsed - (int)elapsed) * 100);
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    char *title = "Ripoff Scale Machine";
    int title_length = strlen(title);


    mvprintw(1, (maxX - title_length) / 2, "%s", title);
    mvprintw(3, maxX - strlen("Stopwatch: %02d:%02d.%02d") , "Stopwatch: %02d:%02d.%02d", mins, secs, centisecs);
    mvprintw(maxY / 6, maxX / 20, "Select From 12");
    mvprintw(maxY / 6 + 1, maxX / 20, "Possible Keys");

    if (model->isKpressed) {
        move(maxY / 6, maxX / 4 - 2);
        clrtoeol();
        move(maxY / 6, (maxX - strlen(model->notes[model->NoteOrder[model->NoteCounter]])) - maxX/20);
        clrtoeol();
        mvprintw(maxY / 6, (maxX - strlen(model->notes[model->NoteOrder[model->NoteCounter]]) - maxX/20), "%s", model->notes[model->NoteOrder[model->NoteCounter]]);
        mvprintw(maxY / 6, maxX / 2 - 7, "%d", model->NoteCounter + 1);
        mvprintw(maxY / 6, (maxX-strlen("out of 12")) / 2, "out of 12");
    }

    attron(A_REVERSE);
    mvprintw(maxY - 3, 1, "k");
    mvprintw(maxY - 2, 1, "q");
    mvprintw(maxY - 3, maxX/6, "t");
    mvprintw(maxY - 2, maxX/6, "r");
    mvprintw(maxY - 3, maxX/3, "l");
    attroff(A_REVERSE);

    mvprintw(maxY - 3, 3, "Next Note");
    mvprintw(maxY - 2, 3, "Quit");
    mvprintw(maxY - 3, maxX/6 + 2, "Time");
    mvprintw(maxY - 2, maxX/6 + 2, "Reset");
    mvprintw(maxY - 3, maxX/3 + 2, "List Notes");

    refresh();
}

void listNotes(Model* model){
    generatePattern(model->NoteOrder, 12);
    int maxY, maxX;
    const char *title = "Notes:";
    getmaxyx(stdscr, maxY, maxX);
    int height = 17, width = 10, starty = (maxY-height)/3, startx = (maxX-width)/2;
    WINDOW *win = newwin(height, width, starty, startx);
    mvwprintw(win, height-15, (width-strlen(title))/2, "%s", title);
    box(win, 0, 0);
    for (int i = 0; i < 12; i++) {
        mvwprintw(win, 3+i, (width-strlen(model->notes[model->NoteOrder[i]]) )/2, "%s", model->notes[model->NoteOrder[i]] );
    
    }


    wrefresh(win);    

    wgetch(win);
    delwin(win);
    model->isKpressed = 0;
    clear();
}

void update(Model* model, int input) {

    switch (input) {
        case 'k':
            if (model->isKpressed == 0) {
                model->NoteCounter = 0;
                model->isKpressed = 1;
            } else {
                if (model->NoteCounter < 11) {
                    model->NoteCounter++;
                } else {
                    model->NoteCounter = 0;
                    generatePattern(model->NoteOrder, 12);
                }
            }
            break;
         case 't':
            if (!model->stopwatch_running) {
            clock_gettime(CLOCK_MONOTONIC, &model->stopwatch_start);
            model->stopwatch_running = 1;
            } else {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            model->stopwatch_elapsed +=
                (now.tv_sec - model->stopwatch_start.tv_sec) +
                (now.tv_nsec - model->stopwatch_start.tv_nsec) / 1e9;
            model->stopwatch_running = 0;
            }
            break;
        case 'r':
            model->stopwatch_running = 0;
            model->stopwatch_elapsed = 0;
            break;
        case 'l':
            listNotes(model);
            break;
        default:
            break;
    }
}

int main() {
    srand(time(NULL));

    Model model;
    initModel(&model);

    initscr();
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    nodelay(stdscr, TRUE);
    signal(SIGWINCH, handle_winch);

    int input = 0;
    while (input != 'q') {
        if(resize_flag){
            resize_flag = 0;
            endwin();
            refresh();
            clear();

        }
        render(&model);
        input = getch();
        update(&model, input);
    }

    endwin();
    return 0;
}



