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

#include "mysndfileio.h"

SndFileIO::SndFileIO(string filename){
    file = new_aubio_sndfile_ro(filename.c_str());
}

SndFileIO::~SndFileIO(){
    int correctlydeleted = del_aubio_sndfile(file);
    if(correctlydeleted!=0) throw string("Deletion Error");
}

int SndFileIO::ReadFrames(int nframes, fvec_t *read){
        return aubio_sndfile_read(file, nframes, read);
}
