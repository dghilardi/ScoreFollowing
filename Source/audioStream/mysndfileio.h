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

#ifndef MY_SNDFILEIO_H
#define MY_SNDFILEIO_H

#include <string>

#include <aubio/aubio.h>
#include <aubio/sndfileio.h>

using namespace std;
class SndFileIO
{
    aubio_sndfile_t *file;
public:
    SndFileIO(string filename);
    ~SndFileIO();
    int ReadFrames(int nframes, fvec_t *read);
};

#endif // MY_SNDFILEIO_H
