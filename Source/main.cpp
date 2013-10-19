#include <AL/alut.h>
#include <iostream>
#include <string>

#include <opencv2/highgui/highgui.hpp>

#include "audioStream/ogg_stream.h"
#include "audioStream/mysndfileio.h"
#include "audioStream/pcmStream/oggdecoder.h"
#include "audioStream/pitchStream/midi_stream.h"
#include "audioStream/pitchStream/pitchdetect.h"
#include "audioStream/featureStream/pcmfeaturestream.h"
#include "dtw.h"
#include "odtw/pitchodtw.h"
#include "odtw/featureodtw.h"
#include "Utils/printutils.h"
#include "audioStream/Input/micinput.h"

#define ODTW_WINSIZE 500
#define ODTW_MAXRUN 3

#define USEONLINEALGORITHM 1
#define USEFEATURES 2
using namespace std;

void micODTW(string trackPath);
void dtwFiles(string trackPath, string inputPath, int flags = USEONLINEALGORITHM | USEFEATURES);
void midiDTW(string midiPath, string inputPath, int flags);

string testSet[] = {
    "../noct15.ogg", "../prova.ogg",                                                      //0
    "../beets51-mid.ogg", "../beets51.ogg",                                               //1
    "../jpaganini24.ogg", "../paganini24.ogg",                                            //2
    "../midpaganini.ogg", "../paganini24.ogg",                                            //3
    "../moonlight.ogg", "../moonlight2.ogg",                                              //4
    "../moonlight-mid.ogg", "../moonlight.ogg",                                           //5
    "../pagcap1a.ogg", "../pagcap1b.ogg",                                                 //6
    "../midpagcap1.ogg", "../pagcap1a.ogg",                                               //7
    "../MAPS_MUS-alb_esp2_AkPnStgb-base.ogg", "../MAPS_MUS-alb_esp2_AkPnStgb-played.ogg"  //8
};

int main(int argc, char* argv[])
{
    int testI = 3;
    midiDTW("../MAPS_MUS-alb_esp2_AkPnStgb-base.mid", "../MAPS_MUS-alb_esp2_AkPnStgb-played.ogg", USEONLINEALGORITHM);
    //dtwFiles(testSet[2*testI], testSet[2*testI+1], USEONLINEALGORITHM|USEFEATURES);
    //micODTW(testSet[2*testI]);
    return 0;
}

void micODTW(string trackPath){
    OggDecoder decoder(trackPath);
    PCMFeatureStream featuresB(decoder);
    FeatureODTW ftodtw(featuresB, ODTW_WINSIZE, ODTW_MAXRUN);

    MicInput microphone(ftodtw);
    cout << "READY" << endl;
    cin.get();
    microphone.start();
    cin.get();
    microphone.stop();
    ftodtw.printCheckSamples();
    ftodtw.showMatrix();
}

void dtwFiles(string trackPath, string inputPath, int flags){
    bool useFeatures = flags & USEFEATURES;
    bool useOnlineAlgorithm = flags & USEONLINEALGORITHM;
    OggDecoder trackDec(trackPath);
    OggDecoder inputDec(inputPath);

    if(!useFeatures){
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
            odtw.printCheckSamples();
            odtw.showROI(50, 65, 49, 62);
            odtw.showMatrix();
        }else{
            DTW dTimeWarping(trackPitch, inputPitch);
        }
    }else{
        if(!useOnlineAlgorithm){
            cerr << "ERROR! Not implemented yet" << endl;
            throw;
        }
        PCMFeatureStream trackFeatures(trackDec);
        PCMFeatureStream inputFeatures(inputDec);

        FeatureODTW odtw(trackFeatures, ODTW_WINSIZE, ODTW_MAXRUN);
        int inputLength = inputFeatures.getLength();
        for(int i=0; i<inputLength; ++i){
            PrintUtils::printPercentage(i, inputLength);
            odtw.appendSingleFeature(inputFeatures.at(i));
        }
        odtw.printCheckSamples();
        odtw.showROI(30, 49, 37, 62);
        odtw.showMatrix();
    }
}

void midiDTW(string midiPath, string inputPath, int flags){
    bool useOnlineAlgorithm = flags & USEONLINEALGORITHM;
    Midi_Stream midiPitch(midiPath);

    OggDecoder inputDec(inputPath);
    PitchDetect inputPitch(inputDec);

    if(useOnlineAlgorithm){
        PitchODTW odtw(midiPitch, ODTW_WINSIZE, ODTW_MAXRUN);
        int inputLength = inputPitch.getLength();
        for(int i=0; i<inputLength; ++i){
            PrintUtils::printPercentage(i, inputLength);
            vector<int> toPass;
            toPass.push_back(inputPitch.getPitch(i));
            odtw.appendPitch(toPass);
        }
        odtw.showMatrix();
    }else{
        DTW dTimeWarping(midiPitch, inputPitch);
    }
}
