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


// Class that can be used to record, playback and save audio data 
// to a file.  It is designed to be a producer/consumer with the
// portaudio library.
//
// This class expects mono audio in INT16 (short) format.
//
// Copyright 2007 by Keith Vertanen.

#ifndef _AUDIO_BUFFER_H_
#define _AUDIO_BUFFER_H_

#include <vector>
#include <iostream>
#include <fstream>

#include "portaudiocpp/PortAudioCpp.hxx"

#include <aubio/aubio.h>
#include "../featureStream/features.h"
#include "../featureStream/featurestream.h"
#include "../featureStream/featuredetectors.h"
#include "../../odtw/featureodtw.h"

using namespace std;

typedef vector<float> VECTOR_SHORT;
typedef vector<float>::iterator VECTOR_SHORT_ITER;

class AudioBuffer
{
	public:
        AudioBuffer(FeatureODTW &_odtw,int iSizeHint);
		~AudioBuffer();
		
		int RecordCallback(const void* pInputBuffer, 
							void* pOutputBuffer, 
							unsigned long iFramesPerBuffer, 
							const PaStreamCallbackTimeInfo* timeInfo, 
							PaStreamCallbackFlags statusFlags);
		int PlaybackCallback(const void* pInputBuffer, 
							void* pOutputBuffer, 
							unsigned long iFramesPerBuffer, 
							const PaStreamCallbackTimeInfo* timeInfo, 
							PaStreamCallbackFlags statusFlags);
		void Clear();
		void WriteToFile(const string& strFilename);
		void ResetPlayback();

	private:
		VECTOR_SHORT		m_vectorSamples;					// Holds the 16-bit mono samples
		VECTOR_SHORT_ITER	m_iPlaybackIter;					// Tracks where we are during playback

        FeatureDetectors *detector;
        FeatureODTW &odtw;

};

#endif

