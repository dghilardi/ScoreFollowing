#include <AL/alut.h>
#include <iostream>
#include <string>

#include <opencv2/highgui/highgui.hpp>

#include "audioStream/ogg_stream.h"
#include "audioStream/mysndfileio.h"
#include "audioStream/pcmStream/oggdecoder.h"
#include "audioStream/pitchStream/midi_stream.h"
#include "audioStream/pitchStream/pitchdetect.h"
#include "audioStream/featureStream/featurestream.h"
#include "dtw.h"
#include "odtw/pitchodtw.h"
#include "odtw/featureodtw.h"
#include "Utils/printutils.h"
#include "audioStream/Input/micinput.h"

#define ODTW_WINSIZE 500
#define ODTW_MAXRUN 3

using namespace std;

void micODTW(string trackPath);
void dtwFiles(string trackPath, string inputPath, bool useOnlineAlgorithm=false);
void midiDTW(string midiPath, string inputPath);

int main(int argc, char* argv[])
{
    //midiDTW("../paganini24.mid", "../paganini24.ogg");
    //dtwFiles("../paganini24.ogg", "../jpaganini24.ogg", true);
    micODTW("../paganini24.ogg");
    return 0;
}

void micODTW(string trackPath){
    OggDecoder decoder(trackPath);
    FeatureStream featuresB(decoder);
    FeatureODTW ftodtw(featuresB, ODTW_WINSIZE, ODTW_MAXRUN);

    MicInput microphone(ftodtw);
    cout << "READY" << endl;
    cin.get();
    microphone.start();
    cin.get();
    microphone.stop();
    ftodtw.showMatrix();
}

void dtwFiles(string trackPath, string inputPath, bool useOnlineAlgorithm){
    OggDecoder trackDec(trackPath);
    OggDecoder inputDec(inputPath);

    PitchDetect trackPitch(trackDec);
    PitchDetect inputPitch(inputDec);

    if(useOnlineAlgorithm){
        double time=0;
        vector<int> input;
        PitchODTW odtw(trackPitch, ODTW_WINSIZE, ODTW_MAXRUN);
        while(time<inputPitch.getLength()){
            PrintUtils::printPercentage(time, inputPitch.getLength());
            input.clear();
            for(int i=1; i<10; ++i){
                if(time>=inputPitch.getLength()) break;
                input.push_back(inputPitch.getPitch(time));
                time+=1;
            }
            odtw.appendPitch(input);
        }
        odtw.showMatrix();
    }else{
        DTW dTimeWarping(trackPitch, inputPitch);
    }
}

void midiDTW(string midiPath, string inputPath){
    Midi_Stream midiPitch(midiPath);

    OggDecoder inputDec(inputPath);
    PitchDetect inputPitch(inputDec);

    DTW dTimeWarping(midiPitch, inputPitch);
}
