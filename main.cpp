#include <AL/alut.h>
#include <iostream>
#include <string>

#include "audioStream/ogg_stream.h"
#include "audioStream/mysndfileio.h"
#include "audioStream/oggdecoder.h"

#include <opencv2/highgui/highgui.hpp>

using namespace std;

void pitchDetect(string inputName);

int main(int argc, char* argv[])
{

    Ogg_Stream ogg;
    alutInit(&argc, argv);

    try
    {
        if(argc < 2)
            throw string("oggplayer *.ogg");
        pitchDetect(argv[1]);
        return 0;
        ogg.open(argv[1]);

        ogg.display();

        if(!ogg.playback())
            throw string("Ogg refused to play.");

        while(ogg.update())
        {
            if(!ogg.playing())
            {
                if(!ogg.playback())
                    throw string("Ogg abruptly stopped.");
                else
                    cout << "Ogg stream was interrupted.\n";
            }
        }

        cout << "Program normal termination.";
        cin.get();
    }
    catch(string error)
    {
        cout << error;
        cin.get();
    }

    ogg.release();

    alutExit();

    return 0;
}

void pitchDetect(string inputName){
    uint channelNumber = 0;
    OggDecoder decoder(inputName, &channelNumber);
    aubio_pitchdetection_t * pitchDetObj = new_aubio_pitchdetection(1024,1,channelNumber, decoder.getSampleRate(), aubio_pitch_yinfft, aubio_pitchm_midi);

    cv::Mat img(512, 1366, CV_8UC1, cv::Scalar::all(0));
    int pos=0;
    //decoder.play(file);
    fvec_t frame;
    //frame.length=decoder.getFrames(file, &(frame.data), &(frame.channels));
    bool cont=true;
    while(cont){
        cont=decoder.readSingleFrame(&(frame.data),&(frame.length));
        frame.channels = channelNumber;
        if(frame.length==1024){
            smpl_t detected = aubio_pitchdetection(pitchDetObj, &frame);
            smpl_t curlevel = aubio_level_detection(&frame, -90);
            detected = round(detected);
            cout << detected <<" level: "<<curlevel<< endl;
            if(pos<1366){
                if(curlevel!=1.) cv::line(img, cv::Point(pos, 512-4*detected), cv::Point(pos, 512-4*(detected+1)), cv::Scalar(255));
                pos++;
            }else{
                cvNamedWindow("Graph", CV_WINDOW_AUTOSIZE);
                cv::imshow("Graph", img);
                cv::waitKey();
                img.setTo(cv::Scalar(0));
                pos=0;
            }
        }
        //cout <<"length: "<< frame.length <<" channels: "<< frame.channels <<" data: "<< (frame.length>0?frame.data[0][0]:0.0) << endl;
    }
    del_aubio_pitchdetection(pitchDetObj);
    for(StreamMap::iterator it = decoder.mStreams.begin();
        it != decoder.mStreams.end();
        ++it) {
        OggStream* stream = (*it).second;
        delete stream;
    }
}
