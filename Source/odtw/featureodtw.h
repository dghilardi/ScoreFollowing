#ifndef FEATUREODTW_H
#define FEATUREODTW_H

#include "odtw.h"
#include "../audioStream/featureStream/featurestream.h"

class FeatureODTW : public ODTW
{
    FeatureStream &track;
    FeatureStream input;

    virtual int getInputSize();
    virtual int getTrackSize();
    virtual int getDistance(int x, int y);
public:
    FeatureODTW(FeatureStream &_track, int _c, int _maxRunCount);
    void appendFeatures(FeatureStream &toAppend);
};

#endif // FEATUREODTW_H
