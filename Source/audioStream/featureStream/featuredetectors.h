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

#ifndef FEATUREDETECTORS_H
#define FEATUREDETECTORS_H

#include <aubio/aubio.h>
#include <cmath>

#include "../../debugHeader.h"

class FeatureDetectors
{
public:
    FeatureDetectors(uint_t win_s, uint_t num_channels);
    ~FeatureDetectors();

    enum FeatureType {
        MODIFIED_KL,
        FRAME_ENERGY,
        SPECTRAL_FLUX,
        KL_DIVERGENCE,
        COMPLEX_DOMAIN,
        PHASE_DEVIATION,
        HIGHFREQ_CONTENT,
        TYPE_COUNT //number of types, must be the last
    };

    void computePVOC(fvec_t *sample, cvec_t *result);
    void computeFFT(fvec_t *sample, cvec_t *result);
    smpl_t detect(FeatureType type_index, cvec_t *fft);
    void computeFeature(FeatureType type_index, cvec_t *fft, smpl_t **result);
private:
    aubio_pickpeak_t * picker;
    aubio_pvoc_t *pvoc;
    aubio_mfft_t *mfft;
    fvec_t *onset;
    aubio_onsetdetection_t *(det_type[TYPE_COUNT]);
};

#endif // FEATUREDETECTORS_H
