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


#include "AudioBuffer.h"

// Constructor, caller can give us a hint about the number of samples we may need to hold.
AudioBuffer::AudioBuffer(FeatureODTW &_odtw, int iSizeHint): detector(NULL), odtw(_odtw)
{
	if (iSizeHint > 0)
		m_vectorSamples.reserve(iSizeHint);

	m_iPlaybackIter = m_vectorSamples.begin();
}

AudioBuffer::~AudioBuffer()
{
}

int AudioBuffer::RecordCallback(const void* pInputBuffer, 
								void* pOutputBuffer, 
								unsigned long iFramesPerBuffer, 
								const PaStreamCallbackTimeInfo* timeInfo, 
								PaStreamCallbackFlags statusFlags)
{
    float** pData = (float**) pInputBuffer;
    fvec_t loaded;

	if (pInputBuffer == NULL)
	{
		cout << "AudioBuffer::RecordCallback, input buffer was NULL!" << endl;
		return paContinue;
	}

    //loaded.data = pData;
    loaded.length = iFramesPerBuffer;
    loaded.channels = 1;
    loaded.data = new smpl_t*[loaded.channels];
    loaded.data[0] = new smpl_t[loaded.length];
    // Copy all the frames over to our internal vector of samples
    for (unsigned long i = 0; i < iFramesPerBuffer; i++){
        //m_vectorSamples.push_back(pData[0][i]);
        //cout << pData[0][i] << endl;
        loaded.data[0][i] = pData[0][i];
    }

    if(detector==NULL) detector = new FeatureDetectors(iFramesPerBuffer, 1);
    Features *frame = new Features(&loaded, *detector);
    odtw.appendSingleFeature(frame);
    delete [] loaded.data[0];
    delete [] loaded.data;

	return paContinue;
}

int AudioBuffer::PlaybackCallback(const void* pInputBuffer, 
									void* pOutputBuffer, 
									unsigned long iFramesPerBuffer, 
									const PaStreamCallbackTimeInfo* timeInfo, 
									PaStreamCallbackFlags statusFlags)
{
    float**			pData	= (float**) pOutputBuffer;
	unsigned long	iOutput = 0;

	if (pOutputBuffer == NULL)
	{
		cout << "AudioBuffer::PlaybackCallback was NULL!" << endl;
		return paComplete;
	}

	// Output samples until we either have satified the caller, or we run out
	while (iOutput < iFramesPerBuffer)
	{
		if (m_iPlaybackIter == m_vectorSamples.end())
		{
			// Fill out buffer with zeros
			while (iOutput < iFramesPerBuffer)
			{
				pData[0][iOutput] = (short) 0;
				iOutput++;
			}
			return paComplete;
		}

		pData[0][iOutput] = (short) *m_iPlaybackIter;

		m_iPlaybackIter++;
		iOutput++;
	}

	return paContinue;
}

// Clear out any data in the buffer and prepare for a new recording.
void AudioBuffer::Clear()
{
	m_vectorSamples.clear();
}

// Dumpt the samples to a raw file 
void AudioBuffer::WriteToFile(const string& strFilename)
{
	fstream fout(strFilename.c_str(), ios::out|ios::binary);
	short iSample;
	for (VECTOR_SHORT_ITER iter = m_vectorSamples.begin(); iter != m_vectorSamples.end(); iter++)
	{
		iSample = (short) *iter;
		fout.write((char *) &iSample, sizeof(short));
	}
	fout.close();
}

// Reset the playback index to the start of the samples
void AudioBuffer::ResetPlayback()
{
	m_iPlaybackIter = m_vectorSamples.begin();
}
