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
