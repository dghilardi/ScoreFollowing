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

#ifndef FEATURES_H
#define FEATURES_H

#include <cmath>
#include <climits>
#include <algorithm>
#include <iostream>
#include <iomanip>
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

    void fillFeature(smpl_t **feature, fvec_t *computed);
    static double getSingleDistance(smpl_t *featureA, int featureAChanNum, smpl_t *featureB, int otherChanNumb);
protected:
    Features(){};
    float chromaVector[NBINS];
    bool isSilence;
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
