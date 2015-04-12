/*
    Copyright (C) 2013  Davide Ghilardi

    This file is part of reScoFo

    reScoFo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    reScoFo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with reScoFo.  If not, see <http://www.gnu.org/licenses/>.
*/

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
#include "audioStream/featureStream/midifeaturestream.h"
#include "dtw.h"
#include "odtw/pitchodtw.h"
#include "odtw/featureodtw.h"
#include "Utils/printutils.h"
#include "audioStream/Input/micinput.h"
#include "Testing/accuracytest.h"

#define ODTW_WINSIZE 100
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
    "../MAPS_MUS-alb_esp2_AkPnStgb-base.ogg", "../MAPS_MUS-alb_esp2_AkPnStgb-played.ogg", //8
    "../MAPS/MAPS_MUS-schuim-1_AkPnStgb-base.ogg", "../MAPS/MAPS_MUS-schuim-1_AkPnStgb-played.ogg",  //9
    "../MAPS/MAPS_MUS-chpn_op35_3_AkPnStgb-base.ogg", "../MAPS/MAPS_MUS-chpn_op35_3_AkPnStgb-played.ogg",  //10
    "../pambiche-.ogg", "../pambiche2.ogg",    //11
    "../pambiche-.ogg", "../pambiche_mod.ogg"  //12
};

string midiTestSet[] = {
    "../MAPS_MUS-alb_esp2_AkPnStgb-base.midi", "../MAPS_MUS-alb_esp2_AkPnStgb-played.ogg",
    "../MAPS/MAPS_MUS-chpn_op35_3_AkPnStgb-base.midi", "../MAPS/MAPS_MUS-chpn_op35_3_AkPnStgb-played.ogg",
    "../MAPS/MAPS_MUS-chpn-p16_AkPnStgb-base.midi", "../MAPS/MAPS_MUS-chpn-p16_AkPnStgb-played.ogg",
    "../midpagcap1.mid", "../midpagcap1.ogg",
    "../midpagcap1.mid", "../pagcap1a.ogg",
    "../pambiche.midi", "../pambiche2.ogg",
    "../pambiche.midi", "../pambiche-.ogg"
};

int main(int argc, char* argv[])
{
    int testI = 5;
    //midiDTW(midiTestSet[2*testI], midiTestSet[2*testI+1], USEONLINEALGORITHM|USEFEATURES);
    dtwFiles(testSet[2*testI], testSet[2*testI+1], USEONLINEALGORITHM|USEFEATURES);
    //micODTW(testSet[2*testI]);
    //AccuracyTest::odtwTest();
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
            //odtw.showROI(50, 65, 49, 62);
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
        inputFeatures.showChromagram();
        trackFeatures.showChromagram();

        cout << "Track length: " <<trackFeatures.getLength()<<"(" <<trackDec.getSampleRate()<<") Execution length: "<< inputFeatures.getLength() << endl;

        FeatureODTW odtw(trackFeatures, ODTW_WINSIZE, ODTW_MAXRUN);
        int inputLength = inputFeatures.getLength();
        for(int i=0; i<inputLength; ++i){
            PrintUtils::printPercentage(i, inputLength);
            odtw.appendSingleFeature(inputFeatures.at(i));
        }
        odtw.printCheckSamples();
        //odtw.showROI(30, 49, 37, 62);
        odtw.showMatrix();
    }
}

void midiDTW(string midiPath, string inputPath, int flags){
    bool useOnlineAlgorithm = flags & USEONLINEALGORITHM;
    bool useFeatures = flags & USEFEATURES;

    Midi_Stream midiPitch(midiPath);

    OggDecoder inputDec(inputPath);

    if(useOnlineAlgorithm){
        if(useFeatures){
            MIDIFeatureStream midiFt(midiPitch);
            midiFt.showChromagram();
            PCMFeatureStream inputFeatures(inputDec);
            inputFeatures.showChromagram();
            FeatureODTW odtw(midiFt, ODTW_WINSIZE, ODTW_MAXRUN);
            int inputLength = inputFeatures.getLength();
            for(int i=0; i<inputLength; ++i){
                PrintUtils::printPercentage(i, inputLength);
                odtw.appendSingleFeature(inputFeatures.at(i));
            }
            odtw.printCheckSamples();
            odtw.showMatrix();
        }else{
            PitchDetect inputPitch(inputDec);
            PitchODTW odtw(midiPitch, ODTW_WINSIZE, ODTW_MAXRUN);
            int inputLength = inputPitch.getLength();
            for(int i=0; i<inputLength; ++i){
                PrintUtils::printPercentage(i, inputLength);
                vector<int> toPass;
                toPass.push_back(inputPitch.getPitch(i));
                odtw.appendPitch(toPass);
            }
            odtw.showMatrix();
        }
    }else{
        PitchDetect inputPitch(inputDec);
        DTW dTimeWarping(midiPitch, inputPitch);
    }
}
