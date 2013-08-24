#include "midi_stream.h"

Midi_Stream::Midi_Stream(string filename){
    MidiFile fMidi;
    File midiFile(filename);
    FileInputStream midiStream(midiFile);

    if(!fMidi.readFrom(midiStream)) throw string(ERROR_NO_FILE);
    int length = fMidi.getLastTimestamp();
    //create map structure
    songMap = new bool*[NUM_MID_NOTES];
    for(int i=0; i<NUM_MID_NOTES; ++i) songMap[i] = new bool[length]();
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
                for(int k=begin; k<end; ++k) songMap[evt->message.getNoteNumber()][k] = true;
            }
        }
    }
}

Midi_Stream::~Midi_Stream(){
    for(int i=0; i<NUM_MID_NOTES; ++i) delete [] songMap[i];
    delete [] songMap;
}

int Midi_Stream::getDistance(int pitch, int time){
    int distance = 0;
    if(songMap[pitch][time]) return 0;
    for(int i=pitch; i<NUM_MID_NOTES; ++i){
        if(songMap[i][time]) distance = i-pitch;
    }
    for(int i=pitch; i>=0; --i){
        if(songMap[i][time] && (time-i<distance || distance==0)) distance = time-i;
    }
    if(distance==0) return SILENCE_DIST;
}
