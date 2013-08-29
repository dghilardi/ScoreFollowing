#include "featurestream.h"

FeatureStream::FeatureStream(){
}

FeatureStream::FeatureStream(PCMStream &stream){
    const unsigned int channelNumber = stream.getChannelNumber();
    fvec_t frame;

    bool cont=true;
    while(cont){
        cont = stream.readSingleFrame(&(frame.data),&(frame.length));
        frame.channels = channelNumber;
        if(frame.length==1024){
            Features currentFeatures(&frame);
            featuresList.push_back(currentFeatures);
        }
    }
}

int FeatureStream::getLength(){
    return featuresList.size();
}

int FeatureStream::getFeatureDistance(FeatureStream listA, int indexA, FeatureStream listB, int indexB){
    return listA.featuresList[indexA].getDistance(listB.featuresList[indexB]);
}
