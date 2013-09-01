#ifndef FEATURESTREAM_H
#define FEATURESTREAM_H

#include <vector>

#include "../pcmStream/pcmstream.h"
#include "features.h"
#include "featuredetectors.h"

using namespace std;
class FeatureStream
{
    vector<Features *> featuresList;
public:
    FeatureStream();
    FeatureStream(PCMStream &stream);

    void append(const FeatureStream &toAppend);
    int getLength();

    static int getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB);
};

#endif // FEATURESTREAM_H
