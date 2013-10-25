#ifndef PRINTUTILS_H
#define PRINTUTILS_H

#include <sstream>
#include <iostream>

#define ERR_NO_OPENCV "Warning! compiled without opencv support"

using namespace std;
class PrintUtils
{
public:
    PrintUtils();
    static void printPercentage(int status, int total);
    static void printError(string errormsg);
    static void errNoOpenCV();
};

#endif // PRINTUTILS_H
