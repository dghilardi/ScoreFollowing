#ifndef FEATURESTREAM_H
#define FEATURESTREAM_H

#include <vector>

#include "../pcmStream/pcmstream.h"
#include "features.h"

using namespace std;
class FeatureStream
{
    vector<Features> featuresList;
public:
    FeatureStream();
    FeatureStream(PCMStream &stream);

    int getLength();

    static int getFeatureDistance(FeatureStream listA, int indexA, FeatureStream listB, int indexB);
};

#endif // FEATURESTREAM_H
