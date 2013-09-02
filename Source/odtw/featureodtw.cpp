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
