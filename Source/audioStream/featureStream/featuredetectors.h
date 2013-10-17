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

    void computeFFT(fvec_t *sample, cvec_t *result);
    smpl_t detect(FeatureType type_index, cvec_t *fft);
    void computeFeature(FeatureType type_index, cvec_t *fft, smpl_t **result);
private:
    aubio_pickpeak_t * picker;
    aubio_pvoc_t *pvoc;
    fvec_t *onset;
    aubio_onsetdetection_t *(det_type[TYPE_COUNT]);
};

#endif // FEATUREDETECTORS_H
