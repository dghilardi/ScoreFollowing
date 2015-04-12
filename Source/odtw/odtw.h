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
    void printCheckSamples();
    void showROI(int trackbegin, int trackend, int inputbegin, int inputend);
    float getTrackTime(float executionTime);
};

#endif // ODTW_H
