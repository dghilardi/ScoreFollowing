#ifndef ODTW_H
#define ODTW_H

#include <map>
#include <vector>

#include <iostream>

#include "audioStream/pitchStream/pitchstream.h"

#include "debugHeader.h"
#include "settings.h"
#include "Utils/printutils.h"

#define INFTY -1

enum ToCompute {
    COLUMN,
    ROW,
    BOTH,
    NONE
};

struct matPoint{
    int x;
    int y;
};

using namespace std;
class ODTW
{
    int c;
    int t, j;
    int runCount, maxRunCount;
    ToCompute previous;
    map<int, map<int, long int> > costMatrix;
    vector<matPoint> path;

    PitchStream &track;
    vector<int> input;

    ToCompute getInc(int mx, int my);
    void evaluatePathCost(int x, int y);
    bool myIsMin(long a, long b);
    long int myAdd(long a, long b);
public:
    ODTW(PitchStream &_track, int _c, int _maxRunCount);
    void onlineTimeWarping(vector<int> newFrames);
    void showMatrix();
};

#endif // ODTW_H
