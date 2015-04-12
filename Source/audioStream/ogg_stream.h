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

#ifndef OGGSTREAM_H
#define OGGSTREAM_H

#include <string>
#include <iostream>

#include <bitset>

#include <AL/al.h>
#include <AL/alut.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include <aubio/aubio.h>

#include "../debugHeader.h"

#define BUFFER_SIZE (4096*8)

using namespace std;
class Ogg_Stream
{
public:
    Ogg_Stream();
    void open(string path); // obtain a handle to the file
    void release(); // release the file handle
    void display(); // display some info on the Ogg
    bool playback(); // play the Ogg stream
    bool playing(); // check if the source is playing
    bool update(); // update the stream if necessary
    bool readSample(fvec_t *sample);
    bool readHeader(ogg_packet &packet, fvec_t *dest);
    bool getFrame(ogg_packet &packet, fvec_t *dest);
protected:
    bool stream(ALuint buffer); // reloads a buffer
    void empty(); // empties the queue
    void check(); // checks OpenAL error state
    string errorString(int code); // stringify an error code
private:
    FILE* oggFile; // file handle
    OggVorbis_File oggStream; // stream handle
    vorbis_info* vorbisInfo; // some formatting data
    vorbis_comment* vorbisComment; // user comments
    vorbis_dsp_state mDsp;
    vorbis_block mBlock;
    int prevRet;
    int samples;
    ALuint buffers[2]; // front and back buffers
    ALuint source; // audio source
    ALenum format; // internal format
};

#endif // OGGSTREAM_H
