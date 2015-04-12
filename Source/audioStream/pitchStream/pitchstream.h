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

#ifndef PITCHSTREAM_H
#define PITCHSTREAM_H

#define NUM_MID_NOTES 127
#define SILENCE_DIST 127
#define SILENCE -1

class PitchStream
{
public:
    virtual int getDistance(int pitch, int time) = 0;
    virtual int getLength() = 0;
};

class SinglePitchStream : public PitchStream {
public:
    virtual int getPitch(int time) = 0;
};

#endif // PITCHSTREAM_H
