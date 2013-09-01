#include "featurestream.h"

FeatureStream::FeatureStream(){
}

FeatureStream::FeatureStream(PCMStream &stream){
    const unsigned int channelNumber = stream.getChannelNumber();
    fvec_t frame;

    bool cont=true;
    FeatureDetectors det(1024, 2);
    while(cont){
        cont = stream.readSingleFrame(&(frame.data),&(frame.length));
        frame.channels = channelNumber;
        if(frame.length==1024){
            //Features currentFeatures(&frame);
            Features *ft = new Features(&frame, det);
            featuresList.push_back(ft);
        }
    }
}

int FeatureStream::getLength(){
    return featuresList.size();
}

int FeatureStream::getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB){
    return Features::getDistance(listA.featuresList[indexA], listB.featuresList[indexB]);
    //return listA.featuresList[indexA]->getDistance(listB.featuresList[indexB]);
}

void FeatureStream::append(const FeatureStream &toAppend){
    featuresList.insert(featuresList.end(), toAppend.featuresList.begin(), toAppend.featuresList.end());
}
