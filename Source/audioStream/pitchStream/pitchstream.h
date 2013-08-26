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
