#include "features.h"
double Features::max=0;
/**
 * @brief Features::Features Compute the features for the frame
 * @param frame frame of PCM audio
 */
Features::Features(fvec_t *frame, FeatureDetectors &det){
    uint_t win_s      = frame->length;                       /* window size */
    numChannels   = frame->channels;                          /* number of channel */
    cvec_t * fft       = new_cvec (win_s, numChannels); /* input buffer */
    fvec_t * out      = new_fvec (1, numChannels);     /* input buffer */

    assert(win_s==1024);

    smpl_t curlevel = aubio_level_detection(frame, -90);
    isSilence = (curlevel==1.);
/*
    aubio_pvoc_t *pvoc = new_aubio_pvoc(win_s, 1, numChannels);
    aubio_pvoc_do(pvoc, frame, fft);
    del_aubio_pvoc(pvoc);
*/
    det.computeFFT(frame, fft);
    fillChromaVector(fft);
    //if(det.detect(FeatureDetectors::SPECTRAL_FLUX, fft))cout << "det "<< endl;
    det.computeFeature(FeatureDetectors::MODIFIED_KL, fft, &modifiedKL);
    det.computeFeature(FeatureDetectors::FRAME_ENERGY, fft, &frameEnergy);
    det.computeFeature(FeatureDetectors::SPECTRAL_FLUX, fft, &spectralFlux);
    det.computeFeature(FeatureDetectors::KL_DIVERGENCE, fft, &klDivergence);
    det.computeFeature(FeatureDetectors::COMPLEX_DOMAIN, fft, &complexDomain);
    det.computeFeature(FeatureDetectors::PHASE_DEVIATION, fft, &phaseDeviation);
    det.computeFeature(FeatureDetectors::HIGHFREQ_CONTENT, fft, &highFreqContent);

    /* allocate fft and other memory space */
    /*
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
    // //aubio_onsetdetection_complex(o,fft,out);
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
    /*for(int i=0; i<numChannels; ++i){
        modifiedKL[i]/=1.2e4;
        klDivergence[i] /= 3.1e4;
        phaseDeviation[i] /= 4e2;
        spectralFlux[i] /= 3e2;
        highFreqContent[i] /= 3.2e5;
        frameEnergy[i] /= 4.5e3;
    }*./
    smpl_t **a = &frameEnergy;
    if((*a)[0]>max) max=(*a)[0];
    if((*a)[1]>max) max=(*a)[1];

    cout << "Frame energy: " << frameEnergy[0]
         << "High Freq: "    << highFreqContent[0]
         << "Spectral flux: "<< spectralFlux[0]
         << "Phase Deviat: " << phaseDeviation[0]
         << "KL diverg: "    << klDivergence[0]
         << "mod KL: "       << modifiedKL[0] << endl;
    //cout << max << endl;*/
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

/**
 * @brief Features::Features Copy constructor
 * @param other
 */
Features::Features(const Features &other):
    numChannels(other.numChannels),
    isSilence(other.isSilence),
    frameEnergy(new smpl_t[numChannels]),
    highFreqContent(new smpl_t[numChannels]),
    spectralFlux(new smpl_t[numChannels]),
    phaseDeviation(new smpl_t[numChannels]),
    klDivergence(new smpl_t[numChannels]),
    modifiedKL(new smpl_t[numChannels])
    //complexDomain(new smpl_t[numChannels])
{
    copy(other.frameEnergy, other.frameEnergy+other.numChannels, frameEnergy);
    copy(other.highFreqContent, other.highFreqContent+other.numChannels, highFreqContent);
    copy(other.spectralFlux, other.spectralFlux+other.numChannels, spectralFlux);
    copy(other.phaseDeviation, other.phaseDeviation+other.numChannels, phaseDeviation);
    copy(other.klDivergence, other.klDivergence+other.numChannels, klDivergence);
    copy(other.modifiedKL, other.modifiedKL+other.numChannels, modifiedKL);
    //copy(other.complexDomain, other.complexDomain+other.numChannels, complexDomain);
}

void Features::fillFeature(smpl_t **feature, fvec_t *computed){
    assert(computed->channels==numChannels && computed->length==1);
    *feature = new smpl_t[numChannels];
    for(int i=0; i<numChannels; ++i) (*feature)[i] = computed->data[i][0];
}

/**
 * @brief Features::getSingleDistance Compute the euclidean distance between two features of the same type but different frames
 * @param featureA contains the feature for each channel of a single frame
 * @param featureB contains the feature for each channel of a single frame
 * @param otherChanNumb Number of channels for the comparing frame
 * @return distance between frames
 */
double Features::getSingleDistance(smpl_t *featureA, int featureAChanNum, smpl_t *featureB, int otherChanNumb){
    double distance = 0;
    if(otherChanNumb==featureAChanNum){
        for(int i=0; i<featureAChanNum; ++i){
            double term = featureA[i]-featureB[i];
            term *= term;
            distance += term;
        }
    }else{
        double meanA=0, meanB=0;
        for(int i=0; i<featureAChanNum; ++i) meanA += featureA[i];
        meanA /= featureAChanNum;
        for(int i=0; i<otherChanNumb; ++i) meanB += featureB[i];
        meanB /= otherChanNumb;
        distance = meanA-meanB;
        distance *= distance;
    }
    //distance = log10(distance+1.0);
    //distance = atan(distance);
    //cout << "sdist: " << distance << endl;
    return distance;
}

/**
 * @brief Features::getDistance Compute euclidean distance between two features vector
 * @param other other vector of features
 * @return euclidean distance between vectors
 */
int Features::getDistance(Features *elem, Features *other){
    double euclideanDist = 0;
    int otherChannelNumber = other->numChannels;

    for(int i=0; i<NBINS; ++i) euclideanDist += pow(elem->chromaVector[i]-other->chromaVector[i], 2);

    /*
    //euclideanDist += getSingleDistance(elem->modifiedKL, elem->numChannels, other->modifiedKL, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->frameEnergy, elem->numChannels, other->frameEnergy, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->spectralFlux, elem->numChannels, other->spectralFlux, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->klDivergence, elem->numChannels, other->klDivergence, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->complexDomain, elem->numChannels, other->complexDomain, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->phaseDeviation, elem->numChannels, other->phaseDeviation, otherChannelNumber);
    euclideanDist += getSingleDistance(elem->highFreqContent, elem->numChannels, other->highFreqContent, otherChannelNumber);
    */
    assert(euclideanDist>=0);
    euclideanDist = 10*sqrt(euclideanDist);
    //if(!(euclideanDist==0 || (euclideanDist-(int)euclideanDist)/euclideanDist<0.1)) cerr << "Error too high"<<endl;
    //assert(euclideanDist==0 || (euclideanDist-(int)euclideanDist)/euclideanDist<0.1);
    assert(euclideanDist>=0);
    if(euclideanDist>INT_MAX) return INT_MAX;
    if(elem->isSilence && other->isSilence) return 0;
    //if(elem->isSilence != other->isSilence) return 50;
    return euclideanDist;
}

int Features::getBin(int frequency){
    return ((int)round(69+12*log2(frequency/440.0)))%12;
}

void Features::fillChromaVector(cvec_t *fft){
    int nmatches[NBINS];
    for(unsigned int i=0; i<NBINS; ++i){
        nmatches[i] = 0;
        chromaVector[i] = 0;
    }
    for(unsigned int i=0; i<fft->length/2; ++i){
        int frequency = i*FS/fft->length;
        int bin = getBin(frequency);
        chromaVector[bin] += fft->norm[0][i];
        nmatches[bin]++;
    }

    //mean
    for(int i=0; i<NBINS; ++i) chromaVector[i]/=nmatches[i];
    /*
    float tot = 0;
    for(int i=0; i<NBINS; ++i) tot+=chromaVector[i];
    if(tot>0 && !isSilence)
        for(int i=0; i<NBINS; ++i) chromaVector[i]/=(tot/10);
    else
        for(int i=0; i<NBINS; ++i) chromaVector[i] = 0;
    */

    //Zero mean, unit variance
    //compute mean
    double tot=0;
    for(int i=0; i<NBINS; ++i) tot += chromaVector[i];
    double mean = tot/(double)NBINS;

    //compute standard deviation
    tot=0;
    for(int i=0; i<NBINS; ++i) tot+=pow(chromaVector[i]-mean,2);
    float stddev = sqrt(tot/NBINS);

    //normalize
    double var = 0, m=0;
    for(int i=0; i<NBINS; ++i){
        chromaVector[i] = (chromaVector[i]-mean)/stddev;
        //cout << chromaVector[i]<<"\t";
        var += pow(chromaVector[i], 2);
        m+=chromaVector[i];
    }
    //cout << endl;
    var=var/NBINS;
    m=m/NBINS;
    //cout << "Varianza: " << var << ", Media: " << m << endl;
    assert(var-1.0 < 1e-5 && var-1.0 > -1e-5 || isSilence);
    assert(m<1e-5 && m>-1e-5 || isSilence);
    if(isSilence) for(int i=0; i<NBINS; ++i) chromaVector[i]=0;

}

void Features::showChromagram(vector<Features*> &data){
#ifdef USE_OPENCV
    const int vscale=50;
    cv::Mat img(vscale*NBINS, data.size(), CV_8UC1, cv::Scalar(0));
    float min=data[0]->chromaVector[0], max=min;
    for(uint i=0; i<data.size(); ++i){ for(int j=0; j<NBINS; ++j){
            if(data[i]->chromaVector[j]>max) max = data[i]->chromaVector[j];
            if(data[i]->chromaVector[j]<min) min = data[i]->chromaVector[j];
        }}
    for(uint i=0; i<data.size(); ++i){ for(int j=0; j<NBINS; ++j){
            cv::line(img, cv::Point(i,j*vscale), cv::Point(i,(j+1)*vscale-1), cv::Scalar(255*(data[i]->chromaVector[j]-min)/(max-min)));
    }}
    cv::namedWindow("chromagram", cv::WINDOW_NORMAL);
    cv::imshow("chromagram", img);
    cv::waitKey();
#else
    PrintUtils::errNoOpenCV();
#endif
}
