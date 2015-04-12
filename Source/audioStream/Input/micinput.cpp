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

#include "micinput.h"

MicInput::MicInput(FeatureODTW &odtw, int iDevice) : objAudioBuffer(odtw, (int)(60*SAMPLE_RATE))
{
    portaudio::System &sys = portaudio::System::instance();

    if(iDevice==-1){
        iDevice = sys.defaultInputDevice().index();
    }

    inParamsRecord = new portaudio::DirectionSpecificStreamParameters(sys.deviceByIndex(iDevice), 1, portaudio::FLOAT32, false, sys.deviceByIndex(iDevice).defaultLowInputLatency(), NULL);
    paramsRecord = new portaudio::StreamParameters(*inParamsRecord, portaudio::DirectionSpecificStreamParameters::null(), SAMPLE_RATE, FRAME_PER_BUFFER, paClipOff);
    streamRecord = new portaudio::MemFunCallbackStream<AudioBuffer>(*paramsRecord, objAudioBuffer, &AudioBuffer::RecordCallback);
}

MicInput::~MicInput(){
    streamRecord->close();

    delete inParamsRecord;
    delete paramsRecord;
    delete streamRecord;
}

void MicInput::start(){
    streamRecord->start();
}

void MicInput::stop(){
    streamRecord->stop();
}
