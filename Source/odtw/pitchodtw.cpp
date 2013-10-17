#include "pitchodtw.h"

PitchODTW::PitchODTW(PitchStream &_track, int _c, int _maxRunCount) : ODTW(_c, _maxRunCount), track(_track)
{
}

/**
 * @brief PitchODTW::appendPitch Append a vector of pitch to the non-costant series and compute the time warping
 * @param newFrames vector of int representing the new pitches
 */
void PitchODTW::appendPitch(vector<int> newFrames){
    input.insert(input.end(), newFrames.begin(), newFrames.end());
    onlineTimeWarping();
}

/**
 * @brief PitchODTW::getInputSize Return the number of elements contained in input
 * @return
 */
int PitchODTW::getInputSize(){
    return input.size();
}

/**
 * @brief PitchODTW::getTrackSize Return the number of elements contained in the ref track
 * @return
 */
int PitchODTW::getTrackSize(){
    return track.getLength();
}

/**
 * @brief PitchODTW::getDistance Return the distance between input and track at given times
 * @param x input time
 * @param y track time
 * @return distance between input[x] and track[y]
 */
int PitchODTW::getDistance(int x, int y){
    assert(x<input.size());
    int cost = track.getDistance(input[x], y);

    return cost;
}
