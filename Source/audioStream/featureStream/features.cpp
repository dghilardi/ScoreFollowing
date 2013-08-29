#include "features.h"

/**
 * @brief Features::Features Compute the features for the frame
 * @param frame frame of PCM audio
 */
Features::Features(fvec_t *frame){
    uint_t win_s      = frame->length;                       /* window size */
    numChannels   = frame->channels;                          /* number of channel */
    cvec_t * fft       = new_cvec (win_s, numChannels); /* input buffer */
    fvec_t * out      = new_fvec (1, numChannels);     /* input buffer */

    assert(win_s==1024);

    smpl_t curlevel = aubio_level_detection(frame, -90);
    isSilence = (curlevel==1.);

    aubio_pvoc_t *pvoc = new_aubio_pvoc(win_s, win_s/2, numChannels);
    aubio_pvoc_do(pvoc, frame, fft);
    del_aubio_pvoc(pvoc);

    /* allocate fft and other memory space */
    aubio_onsetdetection_t * o =
      new_aubio_onsetdetection(aubio_onset_energy, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_energy(o,fft,out);
    fillFeature(&frameEnergy, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_specdiff, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_specdiff(o,fft,out);
    fillFeature(&spectralFlux, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_hfc, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_hfc(o,fft,out);
    fillFeature(&highFreqContent, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_complex, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_complex(o,fft,out);
    fillFeature(&complexDomain, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_phase, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_phase(o,fft,out);
    fillFeature(&phaseDeviation, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_kl, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_kl(o,fft,out);
    fillFeature(&klDivergence, out);
    del_aubio_onsetdetection(o);

    o = new_aubio_onsetdetection(aubio_onset_mkl, win_s, numChannels);
    aubio_onsetdetection(o,fft,out);
    //aubio_onsetdetection_mkl(o,fft,out);
    fillFeature(&modifiedKL, out);
    del_aubio_onsetdetection(o);
}

Features::~Features(){
    delete [] frameEnergy;
    delete [] highFreqContent;
    delete [] spectralFlux;
    delete [] phaseDeviation;
    delete [] klDivergence;
    delete [] modifiedKL;
    delete [] complexDomain;
}

void Features::fillFeature(smpl_t **feature, fvec_t *computed){
    assert(computed->channels==numChannels && computed->length==1);
    *feature = new smpl_t[numChannels];
    for(int i=0; i<numChannels; ++i) (*feature)[i] = computed->data[i][0];
}

double Features::getSingleDistance(smpl_t *featureA, smpl_t *featureB, int otherChanNumb){
    int distance = 0;
    if(otherChanNumb==numChannels){
        for(int i=0; i<numChannels; ++i){
            int term = featureA[i]-featureB[i];
            term *= term;
            distance += distance;
        }
    }else{
        double meanA=0, meanB=0;
        for(int i=0; i<numChannels; ++i) meanA += featureA[i];
        meanA /= numChannels;
        for(int i=0; i<otherChanNumb; ++i) meanB += featureB[i];
        meanB /= otherChanNumb;
        distance = meanA-meanB;
        distance *= distance;
    }
    return distance;
}

/**
 * @brief Features::getDistance Compute euclidean distance between two features vector
 * @param other other vector of features
 * @return euclidean distance between vectors
 */
int Features::getDistance(Features &other){
    double euclideanDist = 0;
    int otherChannelNumber = other.numChannels;

    euclideanDist += getSingleDistance(modifiedKL, other.modifiedKL, otherChannelNumber);
    euclideanDist += getSingleDistance(frameEnergy, other.frameEnergy, otherChannelNumber);
    euclideanDist += getSingleDistance(spectralFlux, other.spectralFlux, otherChannelNumber);
    euclideanDist += getSingleDistance(klDivergence, other.klDivergence, otherChannelNumber);
    euclideanDist += getSingleDistance(complexDomain, other.complexDomain, otherChannelNumber);
    euclideanDist += getSingleDistance(phaseDeviation, other.phaseDeviation, otherChannelNumber);
    euclideanDist += getSingleDistance(highFreqContent, other.highFreqContent, otherChannelNumber);

    euclideanDist = sqrt(euclideanDist);
    assert((euclideanDist-(int)euclideanDist)/euclideanDist<0.1);

    return euclideanDist;
}
