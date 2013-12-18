#ifndef FEATURESTREAM_H
#define FEATURESTREAM_H

#include <vector>

#include "../pcmStream/pcmstream.h"
#include "features.h"
#include "featuredetectors.h"

using namespace std;
class FeatureStream
{
protected:
    vector<Features *> featuresList;
public:
    FeatureStream();
    ~FeatureStream();

    void append(const FeatureStream &toAppend);
    void appendSingle(Features *toAppend);
    int getLength();
    Features *at(int i);

    void showChromagram();
    static int getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB);
};

#endif // FEATURESTREAM_H
