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

#ifndef PITCHDETECT_H
#define PITCHDETECT_H

#include <vector>
#include <aubio/aubio.h>
#include <cmath> //for round
#include <iostream>

#include "pitchstream.h"
#include "../pcmStream/pcmstream.h"

#include "../../Utils/printutils.h"

#include "../../debugHeader.h"
#include "../../settings.h"
using namespace std;
class PitchDetect : public SinglePitchStream
{
    vector<int> pitches;
public:
    PitchDetect(PCMStream &stream);
    int getDistance(int pitch, int time);
    int getLength();
    int getPitch(int time);

    void showNotes();
};

#endif // PITCHDETECT_H
