#ifndef MICINPUT_H
#define MICINPUT_H

#include "portaudiocpp/PortAudioCpp.hxx"

#include <iostream>
#include "AudioBuffer.h"
#include "../../odtw/featureodtw.h"

class MicInput
{
    const double SAMPLE_RATE = 44100.0;
    const int FRAME_PER_BUFFER = 1024;

    AudioBuffer objAudioBuffer;

    portaudio::AutoSystem autoSys;

    portaudio::DirectionSpecificStreamParameters *inParamsRecord;
    portaudio::StreamParameters *paramsRecord;
    portaudio::MemFunCallbackStream<AudioBuffer> *streamRecord;
public:
    MicInput(FeatureODTW &odtw, int iDevice=-1);
    ~MicInput();
    void start();
    void stop();
};

#endif // MICINPUT_H
