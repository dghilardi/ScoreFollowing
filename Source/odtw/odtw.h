#ifndef ODTW_H
#define ODTW_H

#include <map>
#include <vector>

#include <iostream>

#include "../audioStream/pitchStream/pitchstream.h"

#include "../debugHeader.h"
#include "../settings.h"
#include "../Utils/printutils.h"

#ifdef USE_OPENCV
#include <sstream>
#include <iomanip>
#endif

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
protected:
    int c;
    int t, j;
    int runCount, maxRunCount;
    ToCompute previous;
    map<int, map<int, long int> > costMatrix;
    vector<matPoint> path;

    ToCompute getInc(int mx, int my);
    void evaluatePathCost(int x, int y);
    bool myIsMin(long a, long b);
    long int myAdd(long a, long b);

    virtual int getInputSize()=0;
    virtual int getTrackSize()=0;
    virtual int getDistance(int x, int y)=0;
public:
    ODTW(int _c, int _maxRunCount);
    void onlineTimeWarping();
    void showMatrix();
    // void appendPitch(vector<int> newFrames);
};

#endif // ODTW_H
