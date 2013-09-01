#include "micinput.h"

MicInput::MicInput(FeatureODTW &odtw, int iDevice) : objAudioBuffer(odtw, (int)(60*SAMPLE_RATE))
{
    portaudio::System &sys = portaudio::System::instance();

    if(iDevice==-1){
        iDevice = sys.defaultInputDevice().index();
    }

    inParamsRecord = new portaudio::DirectionSpecificStreamParameters(sys.deviceByIndex(iDevice), 1, portaudio::FLOAT32, false, sys.deviceByIndex(iDevice).defaultLowInputLatency(), NULL);
    paramsRecord = new portaudio::StreamParameters(*inParamsRecord, portaudio::DirectionSpecificStreamParameters::null(), SAMPLE_RATE, FRAME_PER_BUFFER, paClipOff);
    streamRecord = new portaudio::MemFunCallbackStream<AudioBuffer>(*paramsRecord, objAudioBuffer, &AudioBuffer::RecordCallback);
}

MicInput::~MicInput(){
    streamRecord->close();

    delete inParamsRecord;
    delete paramsRecord;
    delete streamRecord;
}

void MicInput::start(){
    streamRecord->start();
}

void MicInput::stop(){
    streamRecord->stop();
}
