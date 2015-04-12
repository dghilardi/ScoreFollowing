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

#include "featureodtw.h"

FeatureODTW::FeatureODTW(FeatureStream &_track, int _c, int _maxRunCount) : ODTW(_c, _maxRunCount), track(_track)
{
}

int FeatureODTW::getInputSize(){
    return input.getLength();
}

int FeatureODTW::getTrackSize(){
    return track.getLength();
}

int FeatureODTW::getDistance(int x, int y){
    return FeatureStream::getFeatureDistance(input, x, track, y);
}

/**
 * @brief FeatureODTW::appendFeatures append to the input a series of frame and their features
 * @param toAppend series of frames and their features
 */
void FeatureODTW::appendFeatures(FeatureStream &toAppend){
    input.append(toAppend);
    onlineTimeWarping();
}

/**
 * @brief FeatureODTW::appendSingleFeature append to the input the features for a single frame
 * @param toAppend set of features for a single frame
 */
void FeatureODTW::appendSingleFeature(Features *toAppend){
    input.appendSingle(toAppend);
    onlineTimeWarping();
}
