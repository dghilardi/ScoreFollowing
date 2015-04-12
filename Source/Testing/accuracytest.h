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
