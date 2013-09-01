#ifndef PITCHODTW_H
#define PITCHODTW_H

#include "odtw.h"

class PitchODTW : public ODTW
{
    PitchStream &track;
    vector<int> input;

    virtual int getInputSize();
    virtual int getTrackSize();
    virtual int getDistance(int x, int y);
public:
    PitchODTW(PitchStream &_track, int _c, int _maxRunCount);

    void appendPitch(vector<int> newFrames);
};

#endif // PITCHODTW_H
