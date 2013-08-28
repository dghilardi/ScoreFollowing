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
