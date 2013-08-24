#ifndef MIDIFILE_H
#define MIDIFILE_H

#include <aubio/aubioext.h>
#include <iostream>
#include <list>

#include <opencv2/highgui/highgui.hpp>

#include "../debugHeader.h"

#define BAD_MIDI "Error! Midi is malformed"

#define TICK_PER_BEATS 1;
#define FRAME_PER_SECONDS 0;
#define SILENCE_DIST 5;
struct Note{
    int pitch;
    int duration;
    int start;
};

using namespace std;
class MidiFile
{
    /*
    bool timeDivType;
    unsigned int type, tracksNumber, timeDivision;
    void readNumber(ifstream &stream, long *val, int length);*/
    bool **songMap;
    aubio_midi_player_t *player;

    void computeTable();
public:
    MidiFile(char *filename);
    ~MidiFile();

    long getSongLength();
    int getDistance(int pitch, int time);
};

#endif // MIDIFILE_H
