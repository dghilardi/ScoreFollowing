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

#include "printutils.h"

PrintUtils::PrintUtils()
{
}

void PrintUtils::printPercentage(int status, int total){
    cout.flush();
    if(status<total){
        stringstream str;
        int perc = status*100/total;
        str << perc << "%\t[";
        for(int i=0; i<50; ++i) str << (i*2<perc?"#":i*2==perc?"+":"-");
        str << "]\r";
        string percentage = str.str();
        cerr << percentage;
    }else{
        cout <<"[OK]"<<endl;
    }
}

void PrintUtils::errNoOpenCV(){
    printError(ERR_NO_OPENCV);
}

void PrintUtils::printError(string errormsg){
    cerr << errormsg << endl;
}
