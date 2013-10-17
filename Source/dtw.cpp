#include "dtw.h"

DTW::DTW(PitchStream &track, SinglePitchStream &input)
{
    computeDinamicTimeWarping(track, input);
}

DTW::~DTW(){
    for(int i=0; i<inputSize; ++i) delete [] dtw[i];
    delete [] dtw;
}

/**
 * @brief DTW::computeDinamicTimeWarping compute the Dynamic Time Warping between two series
 * @param track
 * @param input
 */
void DTW::computeDinamicTimeWarping(PitchStream &track, SinglePitchStream &input){
    inputSize = input.getLength();
    trackSize = track.getLength();

    dtw = new long long int*[inputSize];
    for(int i=0; i<inputSize; ++i) dtw[i] = new long long int[trackSize];

    for(int i=1; i<inputSize; ++i) dtw[i][0] = INFTY;
    for(int i=1; i<trackSize; ++i) dtw[0][i] = INFTY;
    dtw[0][0] = 0;

    long long int max=0;

    const float K = 2.0;
    for(int i=1; i<inputSize; ++i){ PrintUtils::printPercentage(i,inputSize); for(int j=1; j<trackSize; ++j){
            int cost = track.getDistance(input.getPitch(i), j);
            long long int min = myAdd(dtw[i-1][j], cost);
            min = myIsMin(min, myAdd(dtw[i][j-1], cost)) ? min : myAdd(dtw[i][j-1], cost);
            min = myIsMin(min, myAdd(dtw[i-1][j-1], K*cost)) ? min : myAdd(dtw[i-1][j-1], K*cost);
            assert(cost>=0);
            assert(min>=0);
            dtw[i][j] = min;
            max = dtw[i][j]>max?dtw[i][j]:max;
    }}

    cout << "Distance: " << dtw[inputSize-1][trackSize-1] << endl;

    const int WIDTH = 640, HEIGHT = 640;
    cv::Mat img(HEIGHT, WIDTH, CV_8UC1, cv::Scalar(0));
    for(int x=0; x<WIDTH; ++x){ for(int y=0; y<HEIGHT; ++y){
            img.at<uchar>(y,x) = 255*pow(dtw[x*inputSize/WIDTH][y*trackSize/HEIGHT]/(double)max, 0.15);
    }}

    computePath();
    for(int i=0; i<minPath.size(); ++i){
        img.at<uchar>(minPath[i].y*HEIGHT/trackSize,minPath[i].x*WIDTH/inputSize) = 255;
    }

    cvNamedWindow("DTW", CV_WINDOW_AUTOSIZE);
    cv::imshow("DTW", img);
    cv::waitKey();
}

/**
 * @brief DTW::computePath Compute the minimal path that rappresent the best-matching path for the time series
 */
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

/**
 * @brief DTW::prevStep compute the previous position in the minimal path with a position given
 * @param actual actual position
 * @return previous position
 */
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

/**
 * @brief DTW::myIsMin Check if the first parameter is less than the second (the parameters must be positive integers, INFTY (-1) is considered the greatest possible)
 * @param a
 * @param b
 * @return true if a is less or equal than b
 */
bool DTW::myIsMin(long a, long b){
    if(b==INFTY) return true;
    if(a==INFTY) return false;
    return (a<=b);
}

/**
 * @brief DTW::myAdd Compute the sum taking count of the infinity values
 * @param a
 * @param b
 * @return
 */
long int DTW::myAdd(long a, long b){
    assert(a==INFTY || a>=0);
    assert(b==INFTY || b>=0);
    if(a==INFTY || b==INFTY) return INFTY;
    else return a+b;
}

int DTW::getTrackAdaptedTime(int currentTime){

}
