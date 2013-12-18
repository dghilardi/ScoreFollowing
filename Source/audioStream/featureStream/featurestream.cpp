#include "featurestream.h"

FeatureStream::FeatureStream(){
}

FeatureStream::~FeatureStream(){
    //for(uint i=0; i<featuresList.size(); ++i) delete featuresList[i];
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

void FeatureStream::showChromagram(){
    Features::showChromagram(featuresList);
}
