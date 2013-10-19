#include "midi_stream.h"

Midi_Stream::Midi_Stream(string filename){
    MidiFile fMidi;
    File midiFile(filename);
    FileInputStream midiStream(midiFile);

    if(!fMidi.readFrom(midiStream)) throw string(ERROR_NO_FILE);
    length = fMidi.getLastTimestamp()/REDUCTION_FACTOR+1;
    //create map structure
    songMap = new bool*[length];
    for(int i=0; i<length; ++i) songMap[i] = new bool[NUM_MID_NOTES]();
    int tracksNumber = fMidi.getNumTracks();
    for(int i=0; i<tracksNumber; ++i){
        const MidiMessageSequence *track = fMidi.getTrack(i);
        int evnum = track->getNumEvents();
        for(int j=0; j<evnum; ++j){
            const MidiMessageSequence::MidiEventHolder *evt = track->getEventPointer(j);
            if(evt->message.isNoteOn()){
                const MidiMessageSequence::MidiEventHolder *evtoff = evt->noteOffObject;
                int begin = evt->message.getTimeStamp();
                int end = evtoff->message.getTimeStamp();
                for(int k=begin; k<end; ++k){
                    int fstidx = k/REDUCTION_FACTOR;
                    int sndidx = evt->message.getNoteNumber();
                    songMap[k/REDUCTION_FACTOR][evt->message.getNoteNumber()] = true;
                }
            }
        }
    }
}

Midi_Stream::~Midi_Stream(){
    for(int i=0; i<length; ++i) delete [] songMap[i];
    delete [] songMap;
}

int Midi_Stream::getDistance(int pitch, int time){
    assert(time>=0 && time<length);
    assert(pitch == SILENCE || (pitch>=0 && pitch<NUM_MID_NOTES));
    int distance = 0;
    if(pitch==SILENCE){
        bool isTrackSilence = true;
        for(int i=0; i<NUM_MID_NOTES; ++i) if(songMap[time][i]) isTrackSilence = false;
        return (isTrackSilence ? 0 : SILENCE_DIST);
    }
    if(songMap[time][pitch]) return 0;
    for(int i=pitch; i<NUM_MID_NOTES; ++i){
        if(songMap[time][i]){
            distance = i-pitch;
            break;
        }
    }
    for(int i=pitch; i>=0; --i){
        if(songMap[time][i] && (time-i<distance || distance==0)){
            distance = pitch-i;
            break;
        }
    }
    assert(distance>=0);
    if(pitch!=SILENCE && distance==0) return SILENCE_DIST;
    return distance;
}

int Midi_Stream::getLength(){
    return length;
}

void Midi_Stream::adaptMidi(string outputName, const DTW timeWarping){

}

void Midi_Stream::showNotes(){
#ifdef USE_OPENCV
    const int hconst = 4, width = 1366;
    cv::Mat img(hconst*NUM_MID_NOTES, width, CV_8UC1, cv::Scalar(0));
    for(int x=0; x<width; ++x){
        int vecpos = x*length/width;
        for(int y=0; y<NUM_MID_NOTES; ++y){
            if(songMap[vecpos][y]) cv::line(img, cv::Point(x,y*hconst), cv::Point(x,(y+1)*hconst-1), cv::Scalar(255));
        }
    }
    cvNamedWindow("MIDI RAPP", CV_WINDOW_AUTOSIZE);
    cv::imshow("MIDI RAPP", img);
    cv::waitKey();
#else
    PrintUtils::errNoOpenCV();
#endif
}
