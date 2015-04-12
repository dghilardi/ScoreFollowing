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

#include "midifeatures.h"

MIDIFeatures::MIDIFeatures(const bool *tones)
{
    //initialize vectors
    int nmatches[NBINS];
    for(uint i=0; i<NBINS; ++i){
        nmatches[i]=0;
        chromaVector[i]=0.0;
    }
    isSilence = true;
    //for each note-class in the frame increment the chroma vector
    for(int i=0; i<NUM_MID_NOTES; ++i){
        if(tones[i]){
            chromaVector[i%12] = 1.0;
            nmatches[i%12]++;
            isSilence = false;
        }
    }

    if(!isSilence){
        //compute mean for each chroma-class
        for(int i=0; i<NBINS; ++i) if(nmatches[i]>0) chromaVector[i]/=nmatches[i];

        //Trasform the vector in zero mean and unit variance:

        //compute mean
        double tot=0;
        for(int i=0; i<NBINS; ++i) tot+= chromaVector[i];
        double mean = tot/(double)NBINS;

        //compute std deviation
        tot=0;
        for(int i=0; i<NBINS; ++i) tot+=pow(chromaVector[i]-mean, 2.0);
        double stddev = sqrt(tot/NBINS);

        //standardize vector
        for(int i=0; i<NBINS; ++i) chromaVector[i] = (chromaVector[i]-mean)/stddev;
    }
}
