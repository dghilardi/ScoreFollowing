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

#include "odtw.h"

ODTW::ODTW(int _c, int _maxRunCount) : t(0), j(0), previous(NONE), runCount(0), c(_c), maxRunCount(_maxRunCount){
    costMatrix[t][j]=0;
}

/**
 * @brief ODTW::getInc Compute if the next move should compute a row, a column or both
 * @param mx Represents the position of the input
 * @param my Represents the position of the track (score)
 * @return Enum that indicate if the next movement should be a row, a column or both
 */
ToCompute ODTW::getInc(int mx, int my){
    ToCompute toReturn;

    if(mx<c) toReturn = BOTH;
    else{
        if(runCount>maxRunCount){
            if(previous == ROW) toReturn = COLUMN;
            else toReturn = ROW;
        }else{
            assert(costMatrix.count(mx) && costMatrix[mx].count(my));
            long int min = costMatrix[mx][my];
            int bestT=mx, bestJ=my;
            for(int x=(mx-c+1>0?mx-c+1:0); x<mx; ++x){
                assert(costMatrix.count(x) && costMatrix[x].count(my));
                if(!myIsMin(min,costMatrix[x][my])){
                    bestT=x;
                    min=costMatrix[x][my];
                }
            }
            for(int y=(my-c+1>0?my-c+1:0); y<my; ++y){
                assert(costMatrix.count(mx) && costMatrix[mx].count(y));
                if(!myIsMin(min, costMatrix[mx][y])){
                    bestJ=y;
                    bestT=mx;
                    min=costMatrix[mx][y];
                }
            }
            //cout << "row: " << costMatrix[mx-c+1][my] << " col: " << costMatrix[mx][my-c+1] << " -> " << (bestT<mx?"ROW":"") << (bestJ<my?"COL":"") << endl;
            assert(bestT==mx || bestJ==my);
            //if(bestT==mx && bestJ ==my) cout << "DIAG" << endl;

            if(bestT<mx) toReturn = ROW;
            else if(bestJ<my) toReturn = COLUMN;
            else toReturn = BOTH;
        }
    }
    return toReturn;
}

/**
 * @brief ODTW::onlineTimeWarping compute the time warping for the new elements in the series
 */
void ODTW::onlineTimeWarping(){
    ToCompute toDo = getInc(t, j);
    while(t<getInputSize()-1 || toDo==ROW){
        //cout << t << ": " << getInputSize() << endl;
        if(toDo!=ROW){
            ++t;
            for(int k=(j-c+1>0?j-c+1:0); k<=j; ++k){
                evaluatePathCost(t,k);
            }
        }

        //do{
            if(toDo!=COLUMN && j<getTrackSize()-1){
                ++j;
                for(int k=(t-c+1>0?t-c+1:0); k<=t; ++k){
                    evaluatePathCost(k,j);
                }
            }
            if(toDo == previous){
                runCount++;
            }else{
                runCount=1;
            }
            if(toDo != BOTH) previous = toDo;
        //}while(getInc(t, j)==COLUMN);
            matPoint bestPoint;
            bestPoint.x=t;
            bestPoint.y=j;
            path.push_back(bestPoint);

            toDo = getInc(t, j);
    }
}

/**
 * @brief ODTW::evaluatePathCost Evaluate the cost-matrix in the position x (ref. to the input) and y (ref. to the track)
 * @param x index of the matrix (input time)
 * @param y index of the matrix (track time)
 */
void ODTW::evaluatePathCost(int x, int y){
    int cost = getDistance(x, y);
    //cout <<"cost: "<< cost << endl;
    //if(x==y) assert(cost==0);
    long int min=INFTY;
    const int K=2;
    if(costMatrix.count(x-1) && costMatrix[x-1].count(y-1)) min = myIsMin(min, myAdd(K*costMatrix[x-1][y-1],cost)) ? min : myAdd(K*costMatrix[x-1][y-1],cost);
    if(costMatrix.count(x-1) && costMatrix[x-1].count(y)) min = myIsMin(min, myAdd(costMatrix[x-1][y],cost)) ? min : myAdd(costMatrix[x-1][y],cost);
    if(costMatrix.count(x) && costMatrix[x].count(y-1)) min = myIsMin(min, myAdd(costMatrix[x][y-1],cost)) ? min : myAdd(costMatrix[x][y-1],cost);
    assert(min>=0);
    costMatrix[x][y] = min;
}

/**
 * @brief ODTW::myIsMin
 * @param a
 * @param b
 * @return
 */
bool ODTW::myIsMin(long a, long b){
    if(b==INFTY) return true;
    if(a==INFTY) return false;
    return (a<=b);
}

long int ODTW::myAdd(long a, long b){
    assert(a==INFTY || a>=0);
    assert(b==INFTY || b>=0);
    if(a==INFTY || b==INFTY) return INFTY;
    else return a+b;
}

/**
 * @brief ODTW::showMatrix Print the obtained matrix and path (using OpenCV)
 */
void ODTW::showMatrix(){
#ifdef USE_OPENCV
    int time = costMatrix.size();
    int trackTime = getTrackSize();
    const int height = 640, width = 640;
    //const int height = trackTime, width = time;
    cv::Mat img(height, width, CV_8UC1, cv::Scalar(128));

    long int max=0;
    for(map<int, map<int, long int> >::iterator it=costMatrix.begin(); it!=costMatrix.end(); ++it){
        for(map<int, long int>::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt){
            max = max>jt->second?max:jt->second;
    }}
    max+=1;
    for(map<int, map<int, long int> >::iterator it=costMatrix.begin(); it!=costMatrix.end(); ++it){
        for(map<int, long int>::iterator jt=it->second.begin(); jt!=it->second.end(); ++jt){
            int x = width*it->first/time;
            int y = height*jt->first/trackTime;
            if(x<width && y<height)img.at<uchar>(y,x)=((255*jt->second)/max);
        }
    }

    const int numhlines = 10, numvlines = 10;
    int trackSec = getTrackSize()*FRAME_SIZE/44100;
    int inputTime = getInputSize()*FRAME_SIZE/44100;
    for(int i=0; i<numhlines; ++i){
        cv::line(img, cv::Point(0, i*height/numhlines), cv::Point(width, i*height/numhlines), cv::Scalar(200));
        stringstream sstream;
        sstream << (trackSec*i/numhlines)/60 << ":" <<setfill('0')<<setw(2)<< (trackSec*i/numhlines)%60;
        putText(img, sstream.str(), cvPoint(3,i*height/numhlines-3), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.7, cv::Scalar(200), 1, CV_AA);
    }
    for(int i=0; i<numvlines; ++i){
        cv::line(img, cv::Point(i*width/numvlines, 0), cv::Point(i*width/numvlines, height), cv::Scalar(200));
        stringstream sstream;
        sstream << (inputTime*i/numvlines)/60 << ":" <<setfill('0')<<setw(2)<< (inputTime*i/numvlines)%60;
        putText(img, sstream.str(), cvPoint(i*width/numhlines+3, height-3), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200), 1, CV_AA);
    }

    for(int i=0; i<path.size(); ++i){
        int x=width*path[i].x/time;
        int y=height*path[i].y/trackTime;
        assert(x<time);
        assert(y<trackTime);
        if(x<width && y<height)img.at<uchar>(y,x)=255;

        //if((path[i].x%215)==0){
        //    cout << "sec: " << (path[i].x/43) << " -> " << (100*(path[i-10].x-path[i+10].x)/(float)(path[i-10].y-path[i+10].y)) << endl;
        //}
    }
    cv::namedWindow("ODTW");
    cv::imshow("ODTW", img);
    cv::waitKey();
#else
    PrintUtils::errNoOpenCV();
#endif
}

void ODTW::showROI(int trackbegin, int trackend, int inputbegin, int inputend){
#ifdef USE_OPENCV
    int beginTrackFrame = (trackbegin*44100)/FRAME_SIZE;
    int beginInputFrame = (inputbegin*44100)/FRAME_SIZE;
    int endTrackFrame = (trackend*44100)/FRAME_SIZE;
    int endInputFrame = (inputend*44100)/FRAME_SIZE;
    cout << endTrackFrame-beginTrackFrame << "x" << endInputFrame-beginInputFrame << endl;
    cv::Mat img(endTrackFrame-beginTrackFrame, endInputFrame-beginInputFrame, CV_8UC1, cv::Scalar(128));

    //find max value
    long max = 0;
    long min = LONG_MAX;
    for(int x=beginInputFrame; x<endInputFrame; ++x){
        for(int y=beginTrackFrame; y<endTrackFrame; ++y){
            if(costMatrix[x].count(y)){
                if(max<costMatrix[x][y]) max=costMatrix[x][y];
                if(min>costMatrix[x][y]) min=costMatrix[x][y];
            }
        }
    }

    for(int x=beginInputFrame; x<endInputFrame; ++x){
        for(int y=beginTrackFrame; y<endTrackFrame; ++y){
            if(costMatrix[x].count(y))
                img.at<uchar>(y-beginTrackFrame, x-beginInputFrame) = (255*(costMatrix[x][y]-min))/(max-min);
        }
    }

    for(int i=0; i<path.size(); ++i){
        int x=path[i].x-beginInputFrame;
        int y=path[i].y-beginTrackFrame;
        if(x>=0 && y>=0 && x<img.cols && y<img.rows) img.at<uchar>(y,x)=255;
    }

    cv::namedWindow("ROI cost matrix", cv::WINDOW_NORMAL);
    cv::imshow("ROI cost matrix",img);
#else
    PrintUtils::errNoOpenCV();
#endif
}

void ODTW::printCheckSamples(){
    const float sampleToSecFactor = FRAME_SIZE/44100.0;

    const int numSamples = 40;
    for(int i=0; i<numSamples; ++i){
        matPoint pt = path.at(path.size()/numSamples*i);
        cout << "track: " << ((int)(pt.y*sampleToSecFactor))/60 <<":"<<setfill('0')<<setw(2)<< ((int)(pt.y*sampleToSecFactor))%60  << "\tinput: " << ((int)(pt.x*sampleToSecFactor))/60<<":"<<setfill('0')<<setw(2)<< ((int)(pt.x*sampleToSecFactor))%60 << endl;
    }
}

float ODTW::getTrackTime(float executionTime){
    const float sampleToSecFactor = FRAME_SIZE/44100.0;
    int sampleIndex = executionTime/sampleToSecFactor;

    for(int i=0; i<path.size(); ++i){
        if(path.at(i).x==sampleIndex) return path.at(i).y*sampleToSecFactor;
    }
    return -1;
}
