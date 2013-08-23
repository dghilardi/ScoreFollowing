#include "audioStream/mysndfileio.h"

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
