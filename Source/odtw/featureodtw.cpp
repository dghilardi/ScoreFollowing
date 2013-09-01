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

void FeatureODTW::appendFeatures(FeatureStream &toAppend){
    input.append(toAppend);
    onlineTimeWarping();
}
