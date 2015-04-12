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

#ifndef MIDI_STREAM_H
#define MIDI_STREAM_H

#include "../../settings.h"

#include <string>
#include "../../../JuceLibraryCode/JuceHeader.h"

#include "../../dtw.h"

#include "../../Utils/printutils.h"
#include "../../settings.h"
#include "../../debugHeader.h"
#include "pitchstream.h"

#define ERROR_NO_FILE "Error! File not found"

const double SCALE_FACTOR = 44100.0/FRAME_SIZE;

using namespace std;
class Midi_Stream : public PitchStream
{
    bool **songMap;
    int length;
public:
    Midi_Stream(string filename);
    ~Midi_Stream();

    int getDistance(int pitch, int time);
    int getLength();

    void showNotes();
    void adaptMidi(string outputName, const DTW timeWarping);
    const bool *getPitchFrame(int time);
};

#endif // MIDI_STREAM_H
