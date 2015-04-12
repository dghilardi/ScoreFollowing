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

#ifndef PITCHODTW_H
#define PITCHODTW_H

#include "odtw.h"

class PitchODTW : public ODTW
{
    PitchStream &track;
    vector<int> input;

    virtual int getInputSize();
    virtual int getTrackSize();
    virtual int getDistance(int x, int y);
public:
    PitchODTW(PitchStream &_track, int _c, int _maxRunCount);

    void appendPitch(vector<int> newFrames);
};

#endif // PITCHODTW_H
