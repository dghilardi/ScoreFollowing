#ifndef MIDIFEATURES_H
#define MIDIFEATURES_H
#include "features.h"
#include "../pitchStream/midi_stream.h"
class MIDIFeatures : public Features
{
public:
    MIDIFeatures(const bool *tones);
};

#endif // MIDIFEATURES_H
