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

#ifndef MIDIFILE_H
#define MIDIFILE_H

#include <aubio/aubioext.h>
#include <iostream>
#include <list>

#include <opencv2/highgui/highgui.hpp>

#include "../debugHeader.h"

#define BAD_MIDI "Error! Midi is malformed"

#define TICK_PER_BEATS 1;
#define FRAME_PER_SECONDS 0;
#define SILENCE_DIST 5;
struct Note{
    int pitch;
    int duration;
    int start;
};

using namespace std;
class MidiFile
{
    /*
    bool timeDivType;
    unsigned int type, tracksNumber, timeDivision;
    void readNumber(ifstream &stream, long *val, int length);*/
    bool **songMap;
    aubio_midi_player_t *player;

    void computeTable();
public:
    MidiFile(char *filename);
    ~MidiFile();

    long getSongLength();
    int getDistance(int pitch, int time);
};

#endif // MIDIFILE_H
