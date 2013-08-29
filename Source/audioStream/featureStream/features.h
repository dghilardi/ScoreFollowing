#ifndef FEATURES_H
#define FEATURES_H

#include <cmath>

#include <aubio/aubio.h>

#include "../../debugHeader.h"

using namespace std;
class Features
{
private:
    uint_t numChannels;
    smpl_t *frameEnergy, *highFreqContent, *spectralFlux, *phaseDeviation, *klDivergence, *modifiedKL, *complexDomain;
    bool isSilence;

    void fillFeature(smpl_t **feature, fvec_t *computed);
    double getSingleDistance(smpl_t *featureA, smpl_t *featureB, int otherChanNumb);
public:
    Features(fvec_t *frame);
    ~Features();

    int getDistance(Features &other);
};

#endif // FEATURES_H
