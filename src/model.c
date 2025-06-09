#include <stdlib.h>
#include <time.h>
#include "model.h"
#include "config.h"



void generatePattern(int* arr, int size){
    for (int i = size - 1;i > 0;i--){
    int j = rand() % (i + 1);

 
    int temp = arr[i];  
    arr[i] = arr[j];    
    arr[j] = temp;      
    }

}

void rootToIonian(int* arr, int root){
    for (int i = 0; i < 7; i++) {
    arr[i] = (root + MAJOR_SCALE_INTERVALS[i]) % 12; 
    }
}



void initModel(Model* model) {
    for (int i = 0; i < 12; i++) {
        model->noteOrder[i] = i;
    }

    model->noteCounter = 0;
    model->isNotePressed = OFF;
    model->isScaleShown = OFF;
    model->toggleList = OFF;

    generatePattern(model->noteOrder, 12);
    model->rootNote = 0;
    model->parentScaleOrderIndex = 0;
    rootToIonian(model->parentScaleOrder, model->rootNote);
    
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


