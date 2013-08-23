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
