/*
    Copyright (C) 2013  Davide Ghilardi

    This file is part of reScoFo

    reScoFo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    reScoFo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with reScoFo.  If not, see <http://www.gnu.org/licenses/>.
*/

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
