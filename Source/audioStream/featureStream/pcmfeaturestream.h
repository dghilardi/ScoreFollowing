#ifndef PCMFEATURESTREAM_H
#define PCMFEATURESTREAM_H
#include "featurestream.h"
#include "../pcmStream/pcmstream.h"
#include "../pcmStream/oggdecoder.h"

class PCMFeatureStream : public FeatureStream
{
public:
    PCMFeatureStream();
    PCMFeatureStream(PCMStream &stream);
};

#endif // PCMFEATURESTREAM_H
