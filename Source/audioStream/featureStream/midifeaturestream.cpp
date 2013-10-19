#include "midifeaturestream.h"

MIDIFeatureStream::MIDIFeatureStream(Midi_Stream &stream){
    for(int i=0; i<stream.getLength(); ++i){
        featuresList.push_back(new MIDIFeatures(stream.getPitchFrame(i)));
    }
}
