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
