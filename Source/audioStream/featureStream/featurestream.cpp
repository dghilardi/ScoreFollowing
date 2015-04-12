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

#include "featurestream.h"

FeatureStream::FeatureStream(){
}

FeatureStream::~FeatureStream(){
    //for(uint i=0; i<featuresList.size(); ++i) delete featuresList[i];
}

int FeatureStream::getLength(){
    return featuresList.size();
}

Features *FeatureStream::at(int i){
    return featuresList.at(i);
}

int FeatureStream::getFeatureDistance(FeatureStream &listA, int indexA, FeatureStream &listB, int indexB){
    assert(indexA<listA.featuresList.size());
    assert(indexB<listB.featuresList.size());
    int distance = Features::getDistance(listA.featuresList[indexA], listB.featuresList[indexB]);
    assert(distance>=0);
    return distance;
    //return listA.featuresList[indexA]->getDistance(listB.featuresList[indexB]);
}

void FeatureStream::append(const FeatureStream &toAppend){
    featuresList.insert(featuresList.end(), toAppend.featuresList.begin(), toAppend.featuresList.end());
}

void FeatureStream::appendSingle(Features *toAppend){
    featuresList.push_back(toAppend);
}

void FeatureStream::showChromagram(){
    Features::showChromagram(featuresList);
}
