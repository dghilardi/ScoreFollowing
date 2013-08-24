#ifndef MIDI_STREAM_H
#define MIDI_STREAM_H

#include <string>
#include "../../JuceLibraryCode/JuceHeader.h"

#define ERROR_NO_FILE "Error! File not found"

#define NUM_MID_NOTES 127
#define SILENCE_DIST 8

using namespace std;
class Midi_Stream
{
    bool **songMap;
public:
    Midi_Stream(string filename);
    ~Midi_Stream();

    int getDistance(int pitch, int time);
};

#endif // MIDI_STREAM_H
