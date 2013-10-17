#ifndef FEATURES_H
#define FEATURES_H

#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>
#include <vector>
#include <aubio/aubio.h>

#include "featuredetectors.h"
#include "../../debugHeader.h"
#include "../../settings.h"
#include "../../Utils/printutils.h"

#ifdef USE_OPENCV
#include <opencv2/highgui/highgui.hpp>
#endif
#define FS 44100
#define NBINS 12

using namespace std;
class Features
{
private:
    static double max;
    uint_t numChannels;
    smpl_t *frameEnergy, *highFreqContent, *spectralFlux, *phaseDeviation, *klDivergence, *modifiedKL, *complexDomain;
    bool isSilence;

    void fillFeature(smpl_t **feature, fvec_t *computed);
    static double getSingleDistance(smpl_t *featureA, int featureAChanNum, smpl_t *featureB, int otherChanNumb);
    float chromaVector[NBINS];
public:
    Features(fvec_t *frame, FeatureDetectors &det);
    Features(const Features &other);
    ~Features();

    static int getDistance(Features *elem, Features *other);
    static void showChromagram(vector<Features*> &data);

    int getBin(int frequency);
    void fillChromaVector(cvec_t *fft);
};

#endif // FEATURES_H
