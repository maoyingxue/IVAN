/******************************************
   file name: Blob.h
   create time: 2017年12月26日 星期二 20时42分26秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Base class of object
******************************************/
#ifndef BLOB_H
#define BLOB_H

#include<iostream>
#include<stdlib.h>
#include<math.h>
#include<vector>
#include<list>
using namespace std;

const double extendBBoxCoefficient = 5;
const double resizeHeightCoefficient = 1;
const double resizeWidthCoefficient = 1;
const int lineThickness = 3;
const int notMatchedFramCntLimit = 100;
const double IOUThreshold = 0.3;
const double newBlobDistanceRate = 0.4;


const pair<double,double> upStart(0.25,0.25);
const pair<double,double> upEnd(0.7,0.25);
const pair<double,double> leftStart(0.25,0.25);
const pair<double,double> leftEnd(0.25,0.9);
const pair<double,double> rightStart(0.7,0.25);
const pair<double,double> rightEnd(0.7,0.9);
const pair<double,double> downStart(0.25,0.9);
const pair<double,double> downEnd(0.7,0.9);
const vector<pair<double,double>> crossingLineStart = {upStart,leftStart,rightStart,downStart};
const vector<pair<double,double>> crossingLineEnd = {upEnd,leftEnd,rightEnd,downEnd};
const int countingLineNum = crossingLineStart.size();

class Blob
{
private:
    vector<Point> contour;
    Rect bBox;
    Rect extendedBBox;
    //Point center;
    vector<Point> center;
    int width;
    int height;
    int area;
    double diagonal;
    double ratio;
    double score;
    Scalar color;
    string cls;
    int notMatchedFrameCnt;            //how long it has't been matched
    bool isCurrentBlob;                          //is current blob?
    vector<bool> isCounted;
    vector<int> crossLines;

public:
    /*****************************************
     *  @brief construct Blob with contour c
     *  @param c: contour c
     * ***************************************/
    Blob(vector<Point> &c);
    /*****************************************
     *  @brief construct Blob with bBox box
     *  @param box: boundingbox box
     * ***************************************/
    Blob(Rect box);

    /*****************************************
     *  @brief construct Blob with bBox box and score
     *  @param box: boundingbox
     *  @param s: score
     * ***************************************/
    Blob(Rect box, double s);
    /*****************************************
     *  @brief construct Blob with bBox box and score
     *  @param box: boundingbox
     *  @param s: score
     *  @param c: class
     * ***************************************/
    Blob(Rect box, double s,string c);

    //set function
    void setBBox(Rect newBox);
    inline void setColor(Scalar c){color = c;}
    inline void setScore(double s){score = s;}

    //get function -----inline-----
    inline vector<Point> getContour() const {return contour;}
    inline Rect getBBox() const {return bBox;}
    //inline Point getCenter()const {return center;}
    inline vector<Point> getCenter() const {return center;}
    inline int getWidth() const {return width;}
    inline int getHeight() const {return height;}
    inline int getArea() const {return area;}
    inline double getDiagonal() const {return diagonal;}
    inline double getRatio() const {return ratio;}
    inline double getScore() const {return score;}
    inline Scalar getColor() const {return color;}

    vector<bool> getIsCounted();                //get is counted
    void changeIsCountedToTrue(int index);       //is counted to true

    Rect extendBBox();
    int getNotMatchedFrameCnt();                 //get not matched frame cnt
    bool getIsCurrentBlob();                     //get is current blob
    void changeIsCurrentBlobToFalse();           //is current blob to false
    void addNotMatchedFrameCnt();                //add not matched frame cnt
    void zeroNotMatchedFrameCnt();               //set not matched frame cnt to zero
    void updateBlob(Blob &currentBlob);          //update blob by current blob
    static void drawBlob(list<Blob> &blobs, Mat &frame2Copy);         //draw bounding box
    static void drawCnt(vector<int> &cnt, Mat &frame2Copy);           //draw counting result
    static void drawClass(list<Blob> &blobs, Mat &frame2Copy);

    bool hasCrossedFirstLine();
    bool hasCrossedSecondLine();
    void setCrossedLine(int index, int line);
    vector<int> getCrossResult();
    string getcls();

};


#endif // BLOB_H
