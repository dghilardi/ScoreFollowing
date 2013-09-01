#include "featuredetectors.h"

FeatureDetectors::FeatureDetectors(uint_t win_s, uint_t num_channels){
    det_type[MODIFIED_KL] = new_aubio_onsetdetection(aubio_onset_mkl, win_s, num_channels);
    det_type[FRAME_ENERGY] = new_aubio_onsetdetection(aubio_onset_energy, win_s, num_channels);
    det_type[SPECTRAL_FLUX] = new_aubio_onsetdetection(aubio_onset_specdiff, win_s, num_channels);
    det_type[KL_DIVERGENCE] = new_aubio_onsetdetection(aubio_onset_kl, win_s, num_channels);
    det_type[COMPLEX_DOMAIN] = new_aubio_onsetdetection(aubio_onset_complex, win_s, num_channels);
    det_type[PHASE_DEVIATION] = new_aubio_onsetdetection(aubio_onset_phase, win_s, num_channels);
    det_type[HIGHFREQ_CONTENT] = new_aubio_onsetdetection(aubio_onset_hfc, win_s, num_channels);

    pvoc = new_aubio_pvoc(win_s, 1, num_channels);
    picker = new_aubio_peakpicker(0.1);
    onset = new_fvec(1, num_channels);
}

FeatureDetectors::~FeatureDetectors(){
    del_aubio_onsetdetection(det_type[MODIFIED_KL]);
    del_aubio_onsetdetection(det_type[FRAME_ENERGY]);
    del_aubio_onsetdetection(det_type[SPECTRAL_FLUX]);
    del_aubio_onsetdetection(det_type[KL_DIVERGENCE]);
    del_aubio_onsetdetection(det_type[COMPLEX_DOMAIN]);
    del_aubio_onsetdetection(det_type[PHASE_DEVIATION]);
    del_aubio_onsetdetection(det_type[HIGHFREQ_CONTENT]);

    del_aubio_peakpicker(picker);
    del_aubio_pvoc(pvoc);
    del_fvec(onset);
}

void FeatureDetectors::computeFFT(fvec_t *sample, cvec_t *result){
    aubio_pvoc_do(pvoc, sample, result);
}

smpl_t FeatureDetectors::detect(FeatureType type_index, cvec_t *fft){
    aubio_onsetdetection(det_type[type_index], fft, onset);
    smpl_t isonset = aubio_peakpick_pimrt(onset, picker);
    return isonset;
}

void FeatureDetectors::computeFeature(FeatureType type_index, cvec_t *fft, smpl_t **result){
    aubio_onsetdetection(det_type[type_index], fft, onset);
    (*result) = new smpl_t[onset->channels];
    for(int i=0; i<onset->channels; ++i) (*result)[i] = onset->data[i][0];
}
