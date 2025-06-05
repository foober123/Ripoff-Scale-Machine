#ifndef CONFIG_H
#define CONFIG_H


#define REFRESH_RATE 100
#define FRAME_DELAY_US (1000000 / REFRESH_RATE)

#define KEY_QUIT 'q'
#define KEY_NOTE 'k'
#define KEY_TIME 't'
#define KEY_RESET_TIME 'r'
#define KEY_LIST 'l'
#define KEY_VOICE 'v'
#define KEY_CYCLE_PARENT 'm'
#define KEY_CYCLE_ROOT 'n'

//MUSIC THEORY. DO NOT TOUCH
#define NOTES_PER_OCTAVE 12

static const char* CHROMATICSCALE[NOTES_PER_OCTAVE] = {"A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"};
static const int MAJOR_SCALE_INTERVALS[] = {0, 2, 4, 5, 7, 9, 11};

#endif
