#ifndef PCMSTREAM_H
#define PCMSTREAM_H

class PCMStream
{
public:
    virtual bool readSingleFrame(float ***data, unsigned int *samples) = 0;
    virtual int  getChannelNumber() = 0;
    virtual int  getSampleRate() = 0;
};

#endif // PCMSTREAM_H
