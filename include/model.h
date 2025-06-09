#ifndef MODEL_H
#define MODEL_H

#include <time.h>

typedef enum {
    ON = 1,
    OFF = 0
} PressState;

typedef struct {
    PressState isNotePressed;
    PressState isScaleShown;
    PressState toggleList;
    PressState stopwatch_running;

    int noteOrder[12];
    int noteCounter;
   
    int rootNote;
    int parentScaleOrder[7];
    int parentScaleOrderIndex;
    const char* notes[12];

    struct timespec stopwatch_start;
    double stopwatch_elapsed;
} Model;

typedef struct{
int maxY;
int maxX;

} screenInfo;

void initModel(Model* model);
void generatePattern(int* arr, int size);
void rootToIonian(int* arr, int root);
double get_time_seconds();
double get_stopwatch_time(Model* model);

#endif
