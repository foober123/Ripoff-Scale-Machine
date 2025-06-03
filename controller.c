void update(Model* model, int input) {

    switch (input) {
        case KEY_NOTE:
                if (model->isNotePressed == OFF) {
                model->noteCounter = 0;
                model->isNotePressed = ON;
                break;
                }

                if (model->noteCounter < 11) {
                    model->noteCounter++;
                } else {
                    model->noteCounter = 0;
                    generatePattern(model->noteOrder, 12);
                }
                clear();
            break;
         case KEY_TIME:
            if (model->stopwatch_running == OFF) {
            clock_gettime(CLOCK_MONOTONIC, &model->stopwatch_start);
            model->stopwatch_running = ON;
            } else {
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            model->stopwatch_elapsed +=
                (now.tv_sec - model->stopwatch_start.tv_sec) +
                (now.tv_nsec - model->stopwatch_start.tv_nsec) / 1e9;
            model->stopwatch_running = 0;
            }
            break;
        case KEY_RESET_TIME:
            model->stopwatch_running = 0;
            model->stopwatch_elapsed = 0;
            break;
        case KEY_LIST:
            generatePattern(model->noteOrder, 12);
            model->isNotePressed = OFF;
            model->toggleList = !model->toggleList;
            if(model->toggleList == OFF){
            clear();
            } 
            break;
        case KEY_CYCLE_ROOT:
            if(model->isScaleShown){
            model->rootNote = (model->rootNote + 1) % 12;
            rootToIonian(model->parentScaleOrder, model->rootNote);
            generatePattern(model->parentScaleOrder, 7);
            model->parentScaleOrderIndex = 0;
            }
            break;
        case KEY_CYCLE_PARENT:
            if(model->isScaleShown == OFF){
            model->isScaleShown = ON;
            model->parentScaleOrderIndex = 0;
            generatePattern(model->parentScaleOrder, 7);
            break;
            }
            
            model->parentScaleOrderIndex = (model->parentScaleOrderIndex + 1) % 7;
            break;
        default:
            break;
    }
}


