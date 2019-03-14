/******************************************
   file name: Blob.cpp
   create time: 2017年12月26日 星期二 20时42分26秒
   author: Jipeng Huang
   e-mail: huangjipengnju@gmail.com
   github: https://github.com/hjptriplebee
   details: Base class of object
******************************************/
#include "Core.h"

Blob::Blob(Rect box)
{
    setBBox(box);
    color = Scalar(rand() % 255, rand() % 255, rand() % 255);
}

Blob::Blob(vector<Point> &c)
{
    setBBox(boundingRect(c));
    color = Scalar(rand() % 255, rand() % 255, rand() % 255);
}

Blob::Blob(Rect box, double s)
{
    score = s;
    setBBox(box);
    color = Scalar(rand() % 255, rand() % 255, rand() % 255);
}

Blob::Blob(Rect box, double s, string c)
{
    score = s;
    setBBox(box);
    color = Scalar(rand() % 255, rand() % 255, rand() % 255);
    cls = c;
}

void Blob::setBBox(Rect newBox)
{
    bBox = newBox;
    width = bBox.width;
    height = bBox.height;
    area = width * height;
    diagonal = sqrt(pow(width, 2) + pow(height, 2));
    ratio = 1.0 * width / height;
    Point curCenter;
    curCenter.x = (bBox.x * 2 + width) / 2;
    curCenter.y = (bBox.y * 2 + height) / 2;
    center.push_back(curCenter);
    vector<bool> temp(countingLineNum, false);
    isCounted = temp;
    vector<int> temp2(2, -1);
    crossLines = temp2;
}


Rect Blob::extendBBox()
{
    extendedBBox.x = bBox.x - extendBBoxCoefficient >= 0 ? bBox.x - extendBBoxCoefficient : 0;
    extendedBBox.y = bBox.y - extendBBoxCoefficient >= 0 ? bBox.y - extendBBoxCoefficient : 0;
    extendedBBox.height = bBox.height + 2 * extendBBoxCoefficient;
    extendedBBox.width = bBox.width + 2 * extendBBoxCoefficient;
    return extendedBBox;
}

int Blob::getNotMatchedFrameCnt()
{
    return notMatchedFrameCnt;
}

bool Blob::getIsCurrentBlob()
{
    return isCurrentBlob;
}

void Blob::changeIsCurrentBlobToFalse()
{
    isCurrentBlob = false;
}

void Blob::addNotMatchedFrameCnt()
{
    notMatchedFrameCnt++;
}

void Blob::zeroNotMatchedFrameCnt()
{
    notMatchedFrameCnt = 0;
}

void Blob::updateBlob(Blob &currentBlob)
{
    contour = currentBlob.contour;
    bBox = currentBlob.bBox;
    center.push_back(currentBlob.center.back());
    diagonal = currentBlob.diagonal;
    ratio = currentBlob.ratio;
    isCurrentBlob = true;
    return;
}

vector<bool> Blob::getIsCounted()
{
    return isCounted;
}

void Blob::changeIsCountedToTrue(int index)
{
    isCounted[index] = true;
}

void Blob::drawBlob(list<Blob> &blobs, Mat &frame2Copy)
{
    for (auto blob : blobs)
    {
        if (!blob.getIsCurrentBlob()) continue;
        Rect r = blob.getBBox();
        r.x = (int)(r.x * resizeWidthCoefficient);
        r.width = (int)(r.width * resizeWidthCoefficient);
        r.y = (int)(r.y * resizeHeightCoefficient);
        r.height = (int)(r.height * resizeHeightCoefficient);
        //cout << r.x << "r:" << endl;
        rectangle(frame2Copy, r, blob.getColor(), lineThickness);
    }
    return;
}

void Blob::drawCnt(vector<int> &cnt, Mat &frame2Copy)
{
    double fontScale = (frame2Copy.rows * frame2Copy.cols) / 800000.0;
    int fontThickness = (int)round(fontScale * 1.5);
    for (int i = 0; i < cnt.size(); i++)
    {
        string text("line " + to_string(i + 1) + ": " + to_string(cnt[i]));
        Size textSize = getTextSize(text, CV_FONT_HERSHEY_SIMPLEX, fontScale, fontThickness, 0);
        Point position;
        position.x = frame2Copy.cols - 1 - (int)((double)textSize.width * 1.1);
        position.y = (int)((double)textSize.height * 1.25 * (i + 1));
        putText(frame2Copy, text, position, CV_FONT_HERSHEY_SIMPLEX, fontScale, GREEN, fontThickness);
    }
    return;
}

void Blob::drawClass(list<Blob> &blobs, Mat &frame2Copy)
{
    for (auto blob : blobs)
    {
        Point classPosition = blob.getBBox().br();
        Scalar classColor = blob.color;
        if (blob.getIsCurrentBlob() == true)
            putText(frame2Copy, blob.cls, classPosition, CV_FONT_HERSHEY_SIMPLEX, 1, classColor, lineThickness, 8);
    }
    return;
}

bool Blob::hasCrossedFirstLine()
{
    if (crossLines[0] == -1)
        return false;
    return true;
}

bool Blob::hasCrossedSecondLine()
{
    if (crossLines[0] == -1 || crossLines[1] == -1)
        return false;
    return true;
}

void Blob::setCrossedLine(int index, int line)
{
    if (index>1) return;
    crossLines[index]=line;
}

vector<int> Blob::getCrossResult()
{
    return crossLines;
}

string Blob::getcls()
{
    return cls;
}













