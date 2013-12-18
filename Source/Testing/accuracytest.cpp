#include "accuracytest.h"

#define ODTW_WINSIZE 100
#define ODTW_MAXRUN 3

AccuracyTest::AccuracyTest()
{
}

void AccuracyTest::odtwTest(){

    //ifstream ifs("../mapsTests.json");
    ifstream ifs("../mapsTests-mid.json");
    //ifstream ifs("../accuracyTest.json");
    string content((istreambuf_iterator<char>(ifs)), istreambuf_iterator<char>());
    Json::Value root;
    Json::Reader reader;
    bool parsedSuccess = reader.parse(content, root, false);

    if(!parsedSuccess){
        PrintUtils::printError("ERROR! can't parse json");
        throw ERROR_PARSE_JSON;
    }

    Json::Value testInstances = root["AccuracyTest"];

    for(int i=0; i<testInstances.size(); ++i){
        cout << testInstances[i]["Name"].asString() << endl;
        string trackPath = testInstances[i]["Track"].asString();
        string inputPath = testInstances[i]["Execution"].asString();

        string ext = trackPath.substr(trackPath.size()-4);
        FeatureStream trackFeatures;
        if(ext==string(".mid") || ext == string("midi")){
            Midi_Stream midi(trackPath);
            trackFeatures = MIDIFeatureStream(midi);
        }else{
            OggDecoder trackDec(trackPath);
            trackFeatures = PCMFeatureStream(trackDec);
        }

        OggDecoder inputDec(inputPath);


        //PCMFeatureStream trackFeatures(trackDec);
        PCMFeatureStream inputFeatures(inputDec);

        FeatureODTW odtw(trackFeatures, ODTW_WINSIZE, ODTW_MAXRUN);
        int inputLength = inputFeatures.getLength();
        for(int i=0; i<inputLength; ++i){
            PrintUtils::printPercentage(i+1, inputLength);
            odtw.appendSingleFeature(inputFeatures.at(i));
        }

        float totErr=0;
        if(testInstances[i]["CompletelyAligned"].asBool()){
            const float samplesToSec = FRAME_SIZE/44100.0;
            float maxErr=0;
            int endTime = inputFeatures.getLength()*samplesToSec;
            cout << "Track time: " << trackFeatures.getLength()*samplesToSec << "\tExecution time: " << endTime << endl;
            for(int i=1; i<=endTime; ++i){
                float trackTime = odtw.getTrackTime(i);
                float err=abs(i-trackTime);
                totErr += err;
                maxErr = max(err, maxErr);
                //cout << "Error: " << err << endl;
            }

            cout << "Mean error: " << totErr/endTime <<"\tMax error: "<<maxErr<< endl;
        }else{
            Json::Value trackTimes = testInstances[i]["TrackSamples"];
            Json::Value executionTimes = testInstances[i]["ExecutionSamples"];
            for(int j=0; j<trackTimes.size(); ++j){
                totErr+=abs(trackTimes[j].asFloat()-odtw.getTrackTime(executionTimes[j].asFloat()));
                cout << executionTimes[j].asFloat() << "\tTrack real: " << trackTimes[j].asFloat() <<"\tFound: "<<odtw.getTrackTime(executionTimes[j].asFloat())<< "Err: " << abs(trackTimes[j].asFloat()-odtw.getTrackTime(executionTimes[j].asFloat())) << endl;
            }
            cout << "Mean error: " << totErr/trackTimes.size() << endl;
        }
    }
}
