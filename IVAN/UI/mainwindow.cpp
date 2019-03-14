#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace cv;

#define SHOW_FLAG false
#define DEBUG_BY_STEP false
#define SAVE_VIDEO false


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setStyleSheet("border: 1px solid black");
    ui->textView->setText("WelCome!");
    //====some initialization
    pTimer = NULL;
    init_ROI(0,0,0,0);
    isSettingROI = false;
    isDrawLine = false;
    for (int i=0;i<4;i++)
    lineColors.push_back(Scalar(255-60*i,255,60*i));
    /*******BUTTONS*******/
    connect(ui->button_open, SIGNAL(clicked()), this, SLOT(on_open_clicked()));
    connect(ui->button_play, SIGNAL(clicked()), this, SLOT(on_play_clicked()));
    connect(ui->button_pause, SIGNAL(clicked()), this, SLOT(on_pause_clicked()));
    connect(ui->button_save, SIGNAL(clicked()), this, SLOT(on_save_clicked()));
    connect(ui->button_ROI, SIGNAL(clicked()), this, SLOT(on_setROI_clicked()));
    connect(ui->button_saveROI, SIGNAL(clicked()), this, SLOT(on_saveROI_clicked()));
    connect(ui->button_draw, SIGNAL(clicked()), this, SLOT(on_draw_clicked()));
    connect(ui->button_extract, SIGNAL(clicked()), this, SLOT(on_extract_clicked()));
    connect(ui->button_ref, SIGNAL(clicked()), this, SLOT(on_setRef_clicked()));
    connect(ui->button_proc, SIGNAL(clicked()), this, SLOT(on_proc_clicked()));

    /*******TABLEVIEW*******/
    QStringList header;
    header<<"minibus"<<"bus"<<"smalltruck"<<"mediumtruck"<<"largetruck"<<"trailer";

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(6);
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    for (int i=0 ;i<ui->tableWidget->columnCount();i++)
    {
        ui->tableWidget->horizontalHeader()->resizeSection(i,(ui->tableWidget->width()-ui->tableWidget->verticalHeader()->width())/(ui->tableWidget->columnCount()));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_ROI(int x,int y,int width,int height)
{
    mainROI.x = x;
    mainROI.y = y;
    mainROI.width = width;
    mainROI.height = height;
}

QImage MainWindow::convertMat2QImage(Mat cvImg)
{
    QImage qImg;
    if(cvImg.channels() == 3)                         //3 channels color image
    {
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_RGB888);
    }
    else if(cvImg.channels() == 1)                    //grayscale image
    {
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_Indexed8);
    }
    else
    {
        qImg = QImage((const unsigned char*)(cvImg.data),
                    cvImg.cols, cvImg.rows,
                    cvImg.cols * cvImg.channels(),
                    QImage::Format_RGB888);
    }
    return qImg;
}

void MainWindow::on_open_clicked()
{
    QString s = QString::number(ui->label->width());
    QString t = QString::number(ui->label->height());

    ui->textView->append("Open Video...");

    //open
    if(pTimer!=NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }
    if(capture.isOpened())
        capture.release();

    QString sFileName =QFileDialog::getOpenFileName(this,tr("Open Video File"),".",tr("Video Files(*.avi *.mp4 *.flv *.mkv *.mov)"));
    capture.open(sFileName.toLocal8Bit().data());
    if(capture.isOpened())
    {
        capture.read(frame);
        frameShow = frame.clone();
        rate = capture.get(CV_CAP_PROP_FPS);
        videoHeight = capture.get(CV_CAP_PROP_FRAME_HEIGHT);
        videoWidth = capture.get(CV_CAP_PROP_FRAME_WIDTH);
        init_ROI(0,0,videoWidth, videoHeight);
        cout<<ui->label->width()<<"\t"<<ui->label->height()<<endl;

        ui->label->resize(ui->label->width(),ui->label->width()*videoHeight/videoWidth);

        cout<<ui->label->width()<<"\t"<<ui->label->height()<<endl;

        cout<<"FPS:"<<rate<<endl<<"Height:"<<videoHeight<<endl<<"Width:"<<videoWidth<<endl;
        cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));
        if(!frame.empty())
        {
            image = convertMat2QImage(frame);
            ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height()));//,Qt::KeepAspectRatio));
        }
    }

    ui->textView->append("Open Video Done!");
}

void MainWindow::nextFrame()
{
    if (frame.empty()) return;

    capture >> frame;

    for (int i = 1; i < lines.size(); i++)
    {
        line(frame, lines[i].first, lines[i].second, Scalar(0,0,255), 10);
    }

    frameShow = frame;

    if (mainROI.width != 0)     frame = Mat(frame, mainROI);
    cv::resize(frame, frame, Size(ui->label->width(), ui->label->height()));
    if (!frame.empty())
    {
        image = convertMat2QImage(frame);
        ui->label->setPixmap(QPixmap::fromImage(image).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    }
}

void MainWindow::on_play_clicked()
{
    ui->textView->append("Playing...");
    if(pTimer!=NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }

    cout<<"lines:"<<lines.size()<<endl;
    for (int i=1;i<lines.size();i++)
    {
        cout<<lines[i].first.x<<"\t"<<lines[i].first.y<<"\t"<<lines[i].second.x<<"\t"<<lines[i].second.y<<endl;
    }

    if(capture.isOpened() && !frame.empty())
    {
        pTimer = new QTimer(this);
        pTimer->setInterval(1000 / rate);
        connect(pTimer, SIGNAL(timeout()), this, SLOT(nextFrame()));
        pTimer->start();
    }
}

void MainWindow::on_pause_clicked()
{
    ui->textView->append("Pause...");
    if(pTimer != NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if(isSettingROI)
    {
        Mat frame_ = frameShow.clone();
        rectangle(frame_, mainROI, cv::Scalar(255,0,0), 3);
        if (!frame_.empty())
        {
            QImage image_ = convertMat2QImage(frame_);
            ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
        }
    }
    else if(isDrawLine)
    {
        Mat frame_ = frameShow.clone();
        line(frame_, lineStart, lineEnd, cv::Scalar(255,0,0), 3);
        for (int i =0;i<lines.size();i++){
            line(frame_, lines[i].first, lines[i].second, lineColors[i], 3);
            putText(frame_,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
        }
        if (!frame_.empty())
        {
            QImage image_ = convertMat2QImage(frame_);
            ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    perm = false;
    if(isSettingROI)
    {
        mainROI.x = int(((event->pos().rx()-ui->label->x())*1.0)/ui->label->width()*videoWidth);
        mainROI.y = int(((event->pos().ry()-ui->label->y())*1.0)/ui->label->height()*videoHeight);
        mainROI.width = 1;
        mainROI.height = 1;
    }
    else if(isDrawLine)
    {
        lineStart = Point(int(((event->pos().rx()-ui->label->x())*1.0)/ui->label->width()*videoWidth),int(((event->pos().ry()-ui->label->y())*1.0)/ui->label->height()*videoHeight));
        lineEnd = Point(int(((event->pos().rx()-ui->label->x())*1.0)/ui->label->width()*videoWidth),int(((event->pos().ry()-ui->label->y())*1.0)/ui->label->height()*videoHeight));
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (!perm)
    {
        if (isSettingROI)
        {
            mainROI.width= abs(int(((event->pos().rx()-ui->label->x())*1.0)/ui->label->width()*videoWidth) - mainROI.x);
            mainROI.height= abs(int(((event->pos().ry()-ui->label->y())*1.0)/ui->label->height()*videoHeight) - mainROI.y);
            update();
        }
        else if(isDrawLine)
        {
            lineEnd = Point(int(((event->pos().rx()-ui->label->x())*1.0)/ui->label->width()*videoWidth),int(((event->pos().ry()-ui->label->y())*1.0)/ui->label->height()*videoHeight));
            update();
        }
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (isSettingROI)
    {
        isSettingROI=false;
    }
    if (isDrawLine)
    {
        isDrawLine = false;
        lines.push_back(pair<Point,Point>(lineStart,lineEnd));
        Mat frame_ = frameShow.clone();
        for (int i =0;i<lines.size();i++){
            line(frame_, lines[i].first, lines[i].second, lineColors[i], 5);
            putText(frame_,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
        }
        lineStart = Point(0,0);
        lineEnd = Point(0,0);
        QImage image_ = convertMat2QImage(frame_);
        ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
    }
    perm = true;
}

void MainWindow::on_setROI_clicked()
{
    if (!capture.isOpened()) return;
    init_ROI(0,0,videoWidth,videoHeight);
    if (lines.size()>=1)
    {
        for (int i =1;i<lines.size();i++){
            line(frameShow, lines[i].first, lines[i].second, lineColors[i], 3);
            putText(frameShow,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
        }
    }
    on_pause_clicked();
    isSettingROI = true;
    isDrawLine = false;
    ui->textView->append("Set 2 points to determine ROI.");
}

void MainWindow::on_draw_clicked()
{
    if (!capture.isOpened()) return;
    if (mainROI.x != 0)     rectangle(frameShow, mainROI, cv::Scalar(255,0,0), 3);
    on_pause_clicked();
    isDrawLine = true;
    isSettingROI = false;
    ui->textView->append("Draw lines between roads");
}

void MainWindow::on_extract_clicked()
{
    init_ROI(0,0,videoWidth,videoHeight);
    Mat frame_ = frameShow.clone();
    lines.pop_back();
    for (int i =0;i<lines.size();i++){
        line(frame_, lines[i].first, lines[i].second, lineColors[i], 3);
        putText(frame_,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
    }
    QImage image_ = convertMat2QImage(frame_);
    ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));
}

void MainWindow::on_saveROI_clicked()
{
    if (!capture.isOpened() || mainROI.width==0) return;
    if (lines.size()>=1)
    {
        for (int i =0;i<lines.size();i++){
            line(frameShow, lines[i].first, lines[i].second, lineColors[i], 3);
            putText(frameShow,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);

        }
    }
    cout<<"ROI SETTINGs:"<<endl<<mainROI.x<<"\t"<<mainROI.x<<"\t"<<mainROI.width<<"\t"<<mainROI.height<<endl;
    cout<<"Line SETTINGs:"<<endl<<lines.size()<<endl;
    for (int i =0;i<lines.size();i++){
        cout<<lines[i].first.x<<"\t"<<lines[i].first.y<<"\t"<<lines[i].second.x<<"\t"<<lines[i].second.y<<endl;
    }
    frameShow = Mat(frameShow, mainROI);
    cv::resize(frameShow, frameShow, Size(ui->label->width(), ui->label->height()));
    QImage image_ = convertMat2QImage(frameShow);
    ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height()));//,Qt::KeepAspectRatio));
}

void MainWindow::on_setRef_clicked()
{
    if (!capture.isOpened()) //cannot open the video
    {
        cout << "can't open the video!" << endl;
        system("pause");
        return;
    }

    //Modify "KCF"
    tracker = TrackerKCF::create("KCF");
    roi = selectROI("tracker", frame, true, false);
    roiOrigin = roi;

    if(roi.width ==0 || roi.height==0)
    {
        cout<<"select roi wrong."<<endl;
        system("pause");
    }

    tracker->init(frame, roi);
    cout<<roi.x<<"\t"<<roi.y<<endl;
}


void MainWindow::addStatistic(int type,int f, int s)
{
    int row,column=type;
    if(f>s){
        row = (crossingNumber-1)*f+s;
    }
    if(f<s){
        row = (crossingNumber-1)*f+s-1;
    }
    QString number=QString::number(ui->tableWidget->item(row,column)->text().toInt()+1);
    ui->tableWidget->setItem(row,column,new QTableWidgetItem(number));
}

void MainWindow::proc_nextFrame()
{
    if ((capture.get(CV_CAP_PROP_POS_FRAMES) + 1) < capture.get(CV_CAP_PROP_FRAME_COUNT)) //has next frame
    {
        capture >> frame2;
        list<Blob> currentBlobs = R->detect(frame2);

        Mat img2Copy = frame2.clone();
        Match::matchBlobs(existingBlobs, currentBlobs, img2Copy);
        vector<int> cnt(lines.size(),0);

//        Blob::drawBlob(existingBlobs, img2Copy);
//        Blob::drawClass(existingBlobs, img2Copy);

        for (int i = 0; i < lines.size(); i++)
        {
            if (Geometry::isCrossLine(existingBlobs, lines[i].first, lines[i].second, cnt[i],i))
            {
                line(img2Copy, lines[i].first, lines[i].second, cv::Scalar(255,0,0), 5);
                putText(img2Copy,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
            }
            else
            {
                line(img2Copy, lines[i].first, lines[i].second, lineColors[i], 5);
                putText(img2Copy,to_string(i+1),Point(lines[i].first.x-10,lines[i].first.y-10),CV_FONT_HERSHEY_SIMPLEX, 3, lineColors[i], lineThickness, 8);
            }
        }

        for (auto &blob : existingBlobs)
        {
            if (blob.hasCrossedSecondLine())
            {
                int f = blob.getCrossResult()[0];
                int s = blob.getCrossResult()[1];
                if (blob.getcls()=="minibus")
                    addStatistic(0,f,s);
                else if (blob.getcls()=="bus")
                    addStatistic(1,f,s);
                else if (blob.getcls()=="smalltruck")
                    addStatistic(2,f,s);
                else if (blob.getcls()=="mediumtruck")
                    addStatistic(3,f,s);
                else if (blob.getcls()=="largetruck")
                    addStatistic(4,f,s);
                else if (blob.getcls()=="trailer")
                    addStatistic(5,f,s);

                cout<<blob.getcls()<<" has cross two line from "<<to_string(f)<<" to "<<to_string(s)<<endl;

                blob.setCrossedLine(0,-1);
                blob.setCrossedLine(1,-1);
            }
        }
//        img2Copy = Mat(img2Copy, mainROI);
//        cv::resize(img2Copy, img2Copy, Size(ui->label->width(), ui->label->height()));
//        QImage image_ = convertMat2QImage(img2Copy);
//        ui->label->setPixmap(QPixmap::fromImage(image_).scaled(ui->label->width(), ui->label->height(), Qt::KeepAspectRatio));

        frame = frame2.clone();
        currentBlobs.clear();
    }
    else
        on_pause_clicked();
}

void MainWindow::on_proc_clicked()
{
    if (mainROI.width == 0){
        cout<<"ROI Area width = 0!"<<endl;
        return;
    }

    if (ui->crossNumber->text()==""){
        cout<<"Please check the lines number!"<<endl;
        return;
    }
    else{
        if (lines.size() != stoi(ui->crossNumber->text().toStdString()))
        {
            cout<<"Check the number of lines"<<endl;
            return;
        }
        crossingNumber = lines.size();
        QStringList verticalLabels;
        ui->tableWidget->setRowCount(crossingNumber*(crossingNumber-1));
        for (int i=0;i<crossingNumber;i++){
            for(int j=0;j<crossingNumber;j++){
                if (i!=j)
                    verticalLabels<<QString::fromStdString(to_string(i+1)+" to "+to_string(j+1));
            }
        }
        ui->tableWidget->setVerticalHeaderLabels(verticalLabels);
        ui->tableWidget->verticalHeader()->setStretchLastSection(true);
        for (int i=0 ;i<ui->tableWidget->columnCount();i++)
        {
            ui->tableWidget->horizontalHeader()->resizeSection(i,(ui->tableWidget->width()-ui->tableWidget->verticalHeader()->width())/(ui->tableWidget->columnCount()+1));
        }

        for (int i=0 ;i<ui->tableWidget->rowCount();i++)
        {
            ui->tableWidget->verticalHeader()->resizeSection(i,(ui->tableWidget->height()-ui->tableWidget->horizontalHeader()->height())/(ui->tableWidget->rowCount()));
        }

        for(int i=0;i<ui->tableWidget->rowCount();i++)
            for (int j=0;j<ui->tableWidget->columnCount();j++)
                ui->tableWidget->setItem(i,j,new QTableWidgetItem("0"));
    }
    cout<<"lines:"<<lines.size()<<endl;
    for (int i=0;i<lines.size();i++)
    {
        cout<<lines[i].first.x<<"\t"<<lines[i].first.y<<"\t"<<lines[i].second.x<<"\t"<<lines[i].second.y<<endl;
    }
    R = new DetectionRCNN("/home/iris/IVAN/Basic_algorithm/Detection.config");
    if (pTimer!=NULL)
    {
        delete pTimer;
        pTimer = NULL;
    }

    if (!capture.isOpened())
    {
        cout<<"Open the Video first!"<<endl;
        return;
    }
  //  cout<<3<<endl;
    existingBlobs = R->detect(frame);

    if (capture.isOpened())
    {
        pTimer = new QTimer(this);
        pTimer->setInterval(1024/rate);
        connect(pTimer, SIGNAL(timeout()), this, SLOT(proc_nextFrame()));
        pTimer->start();
    }
}

void MainWindow::on_save_clicked()
{
    string direction[12] = {"South-East","South-North","South-West","East-North","East-West","East-South","North-West","North-South","North-East","West-South","West-East","West-North"};
    QString fileName = QFileDialog::getSaveFileName(ui->tableWidget, "保存",QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),"Text 文件(*.txt)");
    if (fileName != "")
    {
        ofstream fp;
        fp.open(fileName.toStdString());
        if (!fp.is_open()) return;
        int colCount = ui->tableWidget->columnCount();
        int rowCount = ui->tableWidget->rowCount();
        fp<<"\t\t"<<"Car\t"<<"Bus\t"<<"Flow\n";


        for(int i=0;i<rowCount;i++)
        {
            fp<<direction[i]<<"\t";
            for (int j=0;j<colCount;j++){
                QModelIndex index = ui->tableWidget->model()->index(i,j);
                String strData = ui->tableWidget->model()->data(index).toString().toStdString();
                fp<<strData<<"\t";
            }
            fp<<"\n";
        }
    }
}
