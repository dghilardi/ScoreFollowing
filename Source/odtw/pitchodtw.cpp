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
