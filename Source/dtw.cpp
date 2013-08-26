#include "dtw.h"

DTW::DTW(PitchStream &track, SinglePitchStream &input)
{
    computeDinamicTimeWarping(track, input);
}

DTW::~DTW(){
    for(int i=0; i<inputSize; ++i) delete [] dtw[i];
    delete [] dtw;
}

void DTW::computeDinamicTimeWarping(PitchStream &track, SinglePitchStream &input){
    inputSize = input.getLength();
    trackSize = track.getLength();

    dtw = new long long int*[inputSize];
    for(int i=0; i<inputSize; ++i) dtw[i] = new long long int[trackSize];

    for(int i=1; i<inputSize; ++i) dtw[i][0] = INFTY;
    for(int i=1; i<trackSize; ++i) dtw[0][i] = INFTY;
    dtw[0][0] = 0;

    long long int max=0;

    for(int i=1; i<inputSize; ++i){ PrintUtils::printPercentage(i,inputSize); for(int j=1; j<trackSize; ++j){
            int cost = track.getDistance(input.getPitch(i), j);
            long long int min = dtw[i-1][j];
            min = myIsMin(min,dtw[i][j-1]) ? min : dtw[i][j-1];
            min = myIsMin(min, dtw[i-1][j-1]) ? min : dtw[i-1][j-1];
            assert(cost>=0);
            assert(min>=0);
            dtw[i][j] = cost+min;
            max = dtw[i][j]>max?dtw[i][j]:max;
    }}

    cout << "Distance: " << dtw[inputSize-1][trackSize-1] << endl;

    const int WIDTH = 1366, HEIGHT = 512;
    cv::Mat img(HEIGHT, WIDTH, CV_8UC1, cv::Scalar(0));
    for(int x=0; x<WIDTH; ++x){ for(int y=0; y<HEIGHT; ++y){
            img.at<uchar>(y,x) = 255*dtw[x*inputSize/WIDTH][y*trackSize/HEIGHT]/max;
    }}

    computePath();
    for(int i=0; i<minPath.size(); ++i){
        img.at<uchar>(minPath[i].y*HEIGHT/trackSize,minPath[i].x*WIDTH/inputSize) = 255;
    }

    cvNamedWindow("DTW", CV_WINDOW_AUTOSIZE);
    cv::imshow("DTW", img);
    cv::waitKey();
}

void DTW::computePath(){
    coord actual;
    actual.x=inputSize-1;
    actual.y=trackSize-1;
    minPath.push_back(actual);
    while(actual.x!=0 || actual.y!=0){
        actual = prevStep(actual);
        minPath.push_back(actual);
    }
}

coord DTW::prevStep(coord actual){
    coord toReturn;
    if(actual.x == 0){
        toReturn.x = 0;
        toReturn.y = actual.y-1;
    }else if(actual.y == 0){
        toReturn.y = 0;
        toReturn.x = actual.x -1;
    }else{
        if(myIsMin(dtw[actual.x-1][actual.y-1], dtw[actual.x-1][actual.y])){
            if(myIsMin(dtw[actual.x-1][actual.y-1], dtw[actual.x][actual.y-1])){
                toReturn.x = actual.x-1;
                toReturn.y = actual.y-1;
            }else{
                toReturn.x = actual.x;
                toReturn.y = actual.y-1;
            }
        }else{
            if(myIsMin(dtw[actual.x-1][actual.y], dtw[actual.x][actual.y-1])){
                toReturn.x = actual.x-1;
                toReturn.y = actual.y;
            }else{
                toReturn.x = actual.x;
                toReturn.y = actual.y-1;
            }
        }
    }

    return toReturn;
}

bool DTW::myIsMin(long long int a, long long int b){
    if(b==INFTY) return true;
    if(a==INFTY) return false;
    return (a<=b);
}
