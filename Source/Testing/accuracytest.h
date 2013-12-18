#ifndef ACCURACYTEST_H
#define ACCURACYTEST_H

#include "../odtw/featureodtw.h"
#include "../audioStream/featureStream/pcmfeaturestream.h"
#include "../audioStream/featureStream/midifeaturestream.h"
#include "../audioStream/pcmStream/oggdecoder.h"
#include "../audioStream/pitchStream/midi_stream.h"
#include "../settings.h"

#include <jsoncpp/json/json.h>

#define ERROR_PARSE_JSON 1

class AccuracyTest
{
public:
    AccuracyTest();
    static void odtwTest();
};

#endif // ACCURACYTEST_H
