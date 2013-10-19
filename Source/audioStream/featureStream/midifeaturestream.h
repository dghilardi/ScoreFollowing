#ifndef MIDIFEATURESTREAM_H
#define MIDIFEATURESTREAM_H

#include "featurestream.h"
#include "../pitchStream/midi_stream.h"
#include "midifeatures.h"

class MIDIFeatureStream : public FeatureStream
{
public:
    MIDIFeatureStream(Midi_Stream &stream);
};

#endif // MIDIFEATURESTREAM_H
