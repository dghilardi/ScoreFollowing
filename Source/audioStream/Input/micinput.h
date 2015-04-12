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

#ifndef MICINPUT_H
#define MICINPUT_H

#include "portaudiocpp/PortAudioCpp.hxx"

#include <iostream>
#include "AudioBuffer.h"
#include "../../odtw/featureodtw.h"

class MicInput
{
    const double SAMPLE_RATE = 44100.0;
    const int FRAME_PER_BUFFER = 1024;

    AudioBuffer objAudioBuffer;

    portaudio::AutoSystem autoSys;

    portaudio::DirectionSpecificStreamParameters *inParamsRecord;
    portaudio::StreamParameters *paramsRecord;
    portaudio::MemFunCallbackStream<AudioBuffer> *streamRecord;
public:
    MicInput(FeatureODTW &odtw, int iDevice=-1);
    ~MicInput();
    void start();
    void stop();
};

#endif // MICINPUT_H
