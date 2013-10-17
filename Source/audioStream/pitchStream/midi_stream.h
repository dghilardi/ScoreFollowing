#ifndef MIDI_STREAM_H
#define MIDI_STREAM_H

#include "../../settings.h"

#include <string>
#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../dtw.h"

#include "../../Utils/printutils.h"

#include "../../debugHeader.h"
#include "pitchstream.h"

#define ERROR_NO_FILE "Error! File not found"

#define REDUCTION_FACTOR 1500

using namespace std;
class Midi_Stream : public PitchStream
{
    bool **songMap;
    int length;
public:
    Midi_Stream(string filename);
    ~Midi_Stream();

    int getDistance(int pitch, int time);
    int getLength();

    void showNotes();
    void adaptMidi(string outputName, const DTW timeWarping);
};

#endif // MIDI_STREAM_H
