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

#ifndef FEATURESTREAM_H
#define FEATURESTREAM_H

#include <vector>

#include "../pcmStream/pcmstream.h"
#include "features.h"
#include "featuredetectors.h"

using namespace std;
class FeatureStream
{
protected:
    vector<Features *> featuresList;
public:
    FeatureStream();
    ~FeatureStream();

    void append(const FeatureStream &toAppend);
    void appendSingle(Features *toAppend);
    int getLength();
    Features *at(int i);

    void showChromagram();
    static int getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB);
};

#endif // FEATURESTREAM_H
