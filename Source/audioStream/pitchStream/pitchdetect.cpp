#include "pitchdetect.h"

PitchDetect::PitchDetect(PCMStream &stream){
    const unsigned int channelNumber = stream.getChannelNumber();
    aubio_pitchdetection_t * pitchDetObj = new_aubio_pitchdetection(1024,1,channelNumber, stream.getSampleRate(), aubio_pitch_yinfft, aubio_pitchm_midi);
    fvec_t frame;

    bool cont=true;
    while(cont){
        cont=stream.readSingleFrame(&(frame.data),&(frame.length));
        frame.channels = channelNumber;
        if(frame.length==1024){
            smpl_t detected = aubio_pitchdetection(pitchDetObj, &frame);
            smpl_t curlevel = aubio_level_detection(&frame, -90);
            detected = round(detected);
            //cout << detected <<" level: "<<curlevel<< endl;
            //assert(curlevel==1. || (detected>=0 && detected<NUM_MID_NOTES));
            if(curlevel!=1. && (detected>=0 && detected<NUM_MID_NOTES)) pitches.push_back(detected);
            else pitches.push_back(SILENCE);
        }
    }
    del_aubio_pitchdetection(pitchDetObj);
}

int PitchDetect::getDistance(int pitch, int time){
    assert((pitch>=0 && pitch<NUM_MID_NOTES) || pitch==SILENCE);
    assert(time>=0 && time<pitches.size());

    if(pitch!=pitches[time] && (pitch==SILENCE || pitches[time]==SILENCE)) return SILENCE_DIST;
    else return (pitch>pitches[time] ? pitch-pitches[time] : pitches[time]-pitch); //ensure the distance is positive
}

int PitchDetect::getLength(){
    return pitches.size();
}

int PitchDetect::getPitch(int time){
    assert(time>=0 && time<pitches.size());

    return pitches[time];
}

void PitchDetect::showNotes(){
#ifdef USE_OPENCV
    const int hconst = 4, width = 1366;
    cv::Mat img(hconst*NUM_MID_NOTES, width, CV_8UC1, cv::Scalar(0));
    for(int i=0; i<width; ++i){
        int vecpos = i*pitches.size()/width;
        //if(pitches[vecpos]!=SILENCE) img.at<uchar>(pitches[vecpos]*hconst, i) = 255;
        if(pitches[vecpos]!=SILENCE) cv::line(img,cv::Point(i,pitches[vecpos]*hconst), cv::Point(i,(1+pitches[vecpos])*hconst-1),cv::Scalar(255));
    }
    cvNamedWindow("DETECTION RAPP", CV_WINDOW_AUTOSIZE);
    cv::imshow("DETECTION RAPP", img);
    cv::waitKey();
#else
    PrintUtils::errNoOpenCV();
#endif
}
