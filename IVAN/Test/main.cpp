#define DEBUG_BY_STEP false
#include <iostream>
#include <vector>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

#include <Core.h>
#include <Basic_algorithm.h>
#include <Common.h>
#include <Match.h>

using namespace std;
using namespace cv;

//----------------------------HOGSVM test-------------------------------------------
int main()
{
    vector<Point> lineStart, lineEnd;
    vector<int> cnt(crossingLineStart.size(),0);
    int frameCnt = 2;
    char keyValue = 0;
    Mat img1, img2;

    DetectionRCNN R("/home/iris/IVAN/Basic_algorithm/Detection.config");
//    VideoCapture cap("/data/jipeng/9_1_1.mp4");
    VideoCapture cap("/home/iris/test.mp4");
    cap.read(img1);
    list<Blob> existingBlobs = R.detect(img1);
    cap.read(img2);

    for (int i = 0; i < crossingLineStart.size(); i++)
    {
        int x = (int)((img1.cols - 1) * crossingLineStart[i].first);
        int y = (int)((img1.rows - 1) * crossingLineStart[i].second);
        Point start(x,y);
        lineStart.push_back(start);

        x = (int)((img1.cols - 1) * crossingLineEnd[i].first);
        y = (int)((img1.rows - 1) * crossingLineEnd[i].second);
        Point end(x,y);
        lineEnd.push_back(end);
    }

    while(cap.isOpened() && keyValue != 27)
    {
        Mat img2Copy;            //copy completely

        list<Blob> currentBlobs = R.detect(img2);
        //for(auto blob: currentBlobs) rectangle(img2, blob.getBBox(), Scalar(0, 255, 0), 3);

        //resize(img1, img1Copy, Size(1080, 720));
        //resize(img2, img2Copy, Size(1080, 720));

        Match::matchBlobs(existingBlobs, currentBlobs, img2Copy);

        img2Copy = img2.clone();
        Blob::drawClass(existingBlobs, img2Copy);
        Blob::drawBlob(existingBlobs, img2Copy);

        for (int i = 0; i < lineStart.size(); i++)
        {
            if (Geometry::isCrossLine(existingBlobs, lineStart[i], lineEnd[i], cnt[i],i))
            {
                line(img2Copy, lineStart[i], lineEnd[i], GREEN, lineThickness);
            }
            else
            {
                line(img2Copy, lineStart[i], lineEnd[i], RED, lineThickness);
            }
        }

        Blob::drawCnt(cnt, img2Copy);
        imshow("demo", img2Copy);
        currentBlobs.clear();
        img1 = img2.clone();
        if ((cap.get(CV_CAP_PROP_POS_FRAMES) + 1) < cap.get(CV_CAP_PROP_FRAME_COUNT)) //has next frame
            cap.read(img2);
        else
            break;

        frameCnt++;
        keyValue = waitKey(1);
        if (DEBUG_BY_STEP) waitKey(0);
    }
    return 0;
}

//----------------------------DetectionFrameDiff test----------------------------------
//int main()
//{
//    DetectionFrameDiff D("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config", "/data/jipeng/IVAN/IVAN/Basic_algorithm/DetectionFrameDiff.config");
//    VideoCapture cap("/data/jipeng/9_1_1.mp4");
//    Mat frame1, frame2, frame3;
//    cap>>frame1;
//    while(true)
//    {
//        cap>>frame2;
//        list<Blob> blobs = D.detect(frame1, frame2);
//        for(auto blob: blobs) rectangle(frame1, blob.getBBox(), Scalar(0, 255, 0), 3);
//        resize(frame1, frame3, Size(1080, 720));
//        imshow("demo", frame3);
//        waitKey(1);
//        frame1 = frame2.clone();

//    }
//    return 0;
//}

////----------------------------DetectionRCNN test-------------------------------------------
//int main()
//{
//    DetectionRCNN R("/data/jipeng/IVAN/IVAN/Basic_algorithm/Detection.config");
//    VideoCapture cap("/data/jipeng/9_1_1.mp4");
//    Mat img, img2;
//    while(true)
//    {
//        cap >> img;
//        list<Blob> blobs = R.detect(img);
//        for(auto blob: blobs) rectangle(img, blob.getBBox(), Scalar(0, 255, 0), 3);
//        resize(img, img2, Size(1080, 720));
//        imshow("demo", img2);
//        waitKey(1);
//    }
//    return 0;
//}


