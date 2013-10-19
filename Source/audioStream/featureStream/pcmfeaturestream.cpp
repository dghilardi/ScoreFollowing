#include "pcmfeaturestream.h"

PCMFeatureStream::PCMFeatureStream()
{
}

PCMFeatureStream::PCMFeatureStream(PCMStream &stream){
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

    //Features::showChromagram(featuresList);
}
