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

#ifndef DTW_H
#define DTW_H

#include <opencv2/highgui/highgui.hpp>

#include "audioStream/pitchStream/pitchstream.h"
#include "Utils/printutils.h"

#include <iostream>
#include <algorithm>

#define INFTY (int)-1

struct coord{
    int x;
    int y;
};


using namespace std;
class DTW
{
    long long int **dtw;
    int inputSize, trackSize;
    int *trackCorr, *inputCorr;
    vector<coord> minPath;

    void computeDinamicTimeWarping(PitchStream &track, SinglePitchStream &input);
    void computePath();
    coord prevStep(coord actual);
    bool myIsMin(long a, long b);
    long int myAdd(long int a, long int b);
public:
    DTW(PitchStream &track, SinglePitchStream &input);
    ~DTW();

    int getTrackAdaptedTime(int currentTime);
};

#endif // DTW_H
