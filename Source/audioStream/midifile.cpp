#include "midifile.h"
/*
MidiFile::MidiFile(string filename){
    ifstream fileStream(filename.c_str(), ios::in | ios::binary);
    char val[100];

    if(!(fileStream.read(val,4) && val[0]=='M' && val[1] == 'T' && val[2] == 'h' && val[3]=='d')) throw string(BAD_MIDI);

    long int chunkSize=0;
    readNumber(fileStream, &chunkSize, 4);
    long int tmp;
    readNumber(fileStream, &tmp, 2);
    type = tmp;
    readNumber(fileStream, &tmp, 2);
    tracksNumber = tmp;
    readNumber(fileStream, &tmp, 2);
    timeDivision = tmp;

    assert(type==1 || type==2 || tracksNumber==1);

    int fps;
    if(timeDivision&0x8000){
        timeDivType = FRAME_PER_SECONDS;
        fps = (timeDivision&0x7F00)>>8;
        cout << "Frames per second: "<< fps <<" ticks per frame: "<<(timeDivision&0x00FF)<< endl;
    }else{
        timeDivType = TICK_PER_BEATS;
        cout << "Ticks per beat: "<<(timeDivision&0x7FFF) << endl;
    }

    cout << "cunk size: " << tmp << endl;
    throw;

    while(fileStream.read(val, 1)){
        cout << val[0] << endl;
    }
}

void MidiFile::readNumber(ifstream &stream, long int *val, int length){
    char tmpval[length];
    *val=0;
    if(!stream.read(tmpval, length)) throw string(BAD_MIDI);
    for(int i=0; i<length; ++i) *val = tmpval[i]<<8*(length-i-1);
}*/

MidiFile::MidiFile(char *filename){
    aubio_midi_file_t *file = new_aubio_midi_file(filename);
    player = new_aubio_midi_player();
    aubio_midi_file_load_tracks(file, player);
    del_aubio_midi_file(file);
    long int length = getSongLength();
    songMap = new bool*[128];
    for(int i=0; i<128; ++i) songMap[i] = new bool[length];
    computeTable();

}

MidiFile::~MidiFile(){
    del_aubio_midi_player(player);
    for(int i=0; i<128; ++i) delete [] songMap[i];
    delete [] songMap;
}

void MidiFile::computeTable(){
    int tracknumber = aubio_midi_player_count_tracks(player);
    list<aubio_midi_event_t *> activeEvents;
    //for(int i=0; i<tracknumber; ++i){
        int i=2;
        aubio_track_t *track = aubio_midi_player_get_track(player, i);
        aubio_midi_event_t * evt = aubio_track_first_event(track);
        unsigned long int currentTime = 0;
        int currentPitch[16] = {-1};
        do{
            if(evt==NULL) break;
            int eventType = evt->type;
            currentTime += evt->dtime;
            cout << ((eventType&0xF0) >> 4) << endl;
            if((eventType&0xF0)==144){
                activeEvents.push_back(evt);
                if(currentPitch[evt->channel]!=-1){
                    for(int i=currentTime-evt->dtime; i<currentTime; ++i){
                        songMap[currentPitch[evt->channel]][i] = true;
                    }
                }
                currentPitch[evt->channel] = evt->param1;
            }else if(eventType==176){
                //cout << "MSG: "<<((eventType&0xF0)>>4)<<" - "<<(eventType&0x0F) << endl;
                /*
                //cout << "mode change " << evt->param1 <<" - "<< evt->param2 << endl;
                if(evt->param1==64){
                    for(int i=currentTime-evt->dtime; i<currentTime; ++i){
                        songMap[currentPitch[evt->channel]][i] = true;
                    }
                    currentPitch[evt->channel] = evt->param2;
                }//else cout << "type: " << evt->param1 << endl;
                /*
                list<aubio_midi_event_t *>::iterator pos;
                for(pos=activeEvents.begin(); pos!=activeEvents.end(); ++pos){
                    if(aubio_midi_event_get_pitch(evt) == aubio_midi_event_get_pitch(*pos) && evt->channel==(*pos)->channel) break;
                }
                if(pos!=activeEvents.end()) activeEvents.erase(pos);
                else cerr << "ERROR!" << endl;
                activeEvents.push_back(evt);
                */
            } else if((eventType >> 4)==8){
                cout << "STOP" << endl;
            }
        }while(evt=aubio_track_next_event(track));
    //}

    cv::Mat img(128*5,1366,CV_8UC1, cv::Scalar(10));
    for(int i=0; i<128; ++i){
        cv::line(img, cv::Point(1,i*5), cv::Point(1365,i*5), cv::Scalar(30));
        for(int j=0; j<1366; ++j){
            //img.at<uchar>(i,j) = (songMap[127-i][j*64]?255:0);
            if(songMap[127-i][j*8]) cv::line(img, cv::Point(j,i*5), cv::Point(j,i*5+5), cv::Scalar(230));
    }}
    cvNamedWindow("img", CV_WINDOW_AUTOSIZE);
    cv::imshow("img", img);
    cv::waitKey();
}

int MidiFile::getDistance(int pitch, int time){
    int distance = 0;
    if(songMap[pitch][time]) return 0;
    for(int i=pitch; i<128; ++i){
        if(songMap[i][time]) distance = i-pitch;
    }
    for(int i=pitch; i>=0; --i){
        if(songMap[i][time] && (time-i<distance || distance==0)) distance = time-i;
    }
    if(distance==0) return SILENCE_DIST;
}

long int MidiFile::getSongLength(){
    int tracknumber =aubio_midi_player_count_tracks(player);
    long int maxLength = 0;
    for(int i=0; i<tracknumber; ++i){
        int currentLength = aubio_track_get_duration(aubio_midi_player_get_track(player, i));
        if(currentLength>maxLength) maxLength = currentLength;
    }
    return maxLength;
}
