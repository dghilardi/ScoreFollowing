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

#ifndef MIDIFEATURESTREAM_H
#define MIDIFEATURESTREAM_H

#include "featurestream.h"
#include "../pitchStream/midi_stream.h"
#include "midifeatures.h"

class MIDIFeatureStream : public FeatureStream
{
public:
    MIDIFeatureStream(Midi_Stream &stream);
};

#endif // MIDIFEATURESTREAM_H
