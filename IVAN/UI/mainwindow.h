#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFileDialog>
#include <QFile>
#include <QStandardItemModel>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QMessageBox>
#include <QToolBar>
#include <QMenuBar>
#include <QLabel>
#include <QTableWidget>
#include <QDesktopServices>
#include <QApplication>
#include <fstream>

#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/tracking.hpp>

#include <Core.h>
#include <Common.h>
#include <Basic_algorithm.h>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_open_clicked();
    void on_play_clicked();
    void on_pause_clicked();
    void on_save_clicked();
    void on_setROI_clicked();
    void on_saveROI_clicked();
    void on_draw_clicked();
    void on_extract_clicked();
    void on_proc_clicked();
    void on_setRef_clicked();
    void nextFrame();
    void proc_nextFrame();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    QImage convertMat2QImage(Mat cvImg);
    void setROI();
    void init_ROI(int x,int y,int width,int height);
    void addStatistic(int type,int f, int s);

    Ui::MainWindow *ui;

    int crossingNumber;

    double rate;
    int videoHeight,videoWidth;
    bool isSettingROI;
    bool isDrawLine;
    bool perm;
    vector<pair<Point,Point>> lines;
    Point lineStart,lineEnd;
    vector<Scalar> lineColors;

    QTimer *pTimer;
    QImage image;

    Mat frame,frame2, frameShow;
    Rect mainROI;

    VideoCapture capture;

    Ptr<Tracker> tracker;
    Rect2d roi;                 //用于跟踪的标识物
    Rect2d roiOrigin;

    DetectionRCNN *R;
    list<Blob> existingBlobs;
};

#endif // MAINWINDOW_H
