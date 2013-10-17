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

    Features::showChromagram(featuresList);
}

int FeatureStream::getLength(){
    return featuresList.size();
}

Features *FeatureStream::at(int i){
    return featuresList.at(i);
}

int FeatureStream::getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB){
    assert(indexA<listA.featuresList.size());
    assert(indexB<listB.featuresList.size());
    int distance = Features::getDistance(listA.featuresList[indexA], listB.featuresList[indexB]);
    assert(distance>=0);
    return distance;
    //return listA.featuresList[indexA]->getDistance(listB.featuresList[indexB]);
}

void FeatureStream::append(const FeatureStream &toAppend){
    featuresList.insert(featuresList.end(), toAppend.featuresList.begin(), toAppend.featuresList.end());
}

void FeatureStream::appendSingle(Features *toAppend){
    featuresList.push_back(toAppend);
}
