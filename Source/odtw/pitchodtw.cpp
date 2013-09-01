#include "pitchodtw.h"

PitchODTW::PitchODTW(PitchStream &_track, int _c, int _maxRunCount) : ODTW(_c, _maxRunCount), track(_track)
{
}

void PitchODTW::appendPitch(vector<int> newFrames){
    input.insert(input.end(), newFrames.begin(), newFrames.end());
    onlineTimeWarping();
}

int PitchODTW::getInputSize(){
    return input.size();
}

int PitchODTW::getTrackSize(){
    return track.getLength();
}

int PitchODTW::getDistance(int x, int y){

    assert(x<input.size());
    int cost = track.getDistance(input[x], y);

    return cost;
}
