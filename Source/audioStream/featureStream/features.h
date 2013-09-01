#ifndef FEATURES_H
#define FEATURES_H

#include <cmath>
#include <algorithm>
#include <iostream>

#include <aubio/aubio.h>

#include "featuredetectors.h"
#include "../../debugHeader.h"

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
public:
    Features(fvec_t *frame, FeatureDetectors &det);
    Features(const Features &other);
    ~Features();

    static int getDistance(Features *elem, Features *other);
};

#endif // FEATURES_H