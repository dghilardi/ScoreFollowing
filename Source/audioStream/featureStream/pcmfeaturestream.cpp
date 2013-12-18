#include "pcmfeaturestream.h"

PCMFeatureStream::PCMFeatureStream()
{
}

PCMFeatureStream::PCMFeatureStream(PCMStream &stream){
    const unsigned int channelNumber = stream.getChannelNumber();
    fvec_t frame;

    bool cont=true;
    FeatureDetectors det(FRAME_SIZE, 2);
    int skipped=0;
    while(cont){
        frame.data = new float*[2];
        const int frameSize = FRAME_SIZE;
        frame.data[0] = new float[frameSize];
        frame.data[1] = new float[frameSize];
        cont = stream.readSingleFrame(&(frame.data),&(frame.length));
        frame.channels = channelNumber;
        if(frame.length==FRAME_SIZE){
            //Features currentFeatures(&frame);
            Features *ft = new Features(&frame, det);
            featuresList.push_back(ft);
        }
        delete [] frame.data[0];
        delete [] frame.data[1];
        delete [] frame.data;
    }

    //Features::showChromagram(featuresList);
}
