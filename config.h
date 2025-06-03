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

static const char* CHROMATICSCALE[12] = {
        "A", "A#/Bb", "B", "C", "C#/Db", "D", "D#/Eb", "E", "F", "F#/Gb", "G", "G#/Ab"
    };


static const int MAJOR_SCALE_INTERVALS[] = {0, 2, 4, 5, 7, 9, 11};
