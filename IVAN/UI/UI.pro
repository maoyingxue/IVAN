#-------------------------------------------------
#
# Project created by QtCreator 2017-12-28T15:21:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UI
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /opt/anaconda3/include/python3.6m

LIBS += /usr/local/lib/libopencv_highgui.so
LIBS += /usr/local/lib/libopencv_core.so
LIBS += /usr/local/lib/libopencv_imgproc.so
LIBS += /usr/local/lib/libopencv_videoio.so
LIBS += /usr/local/lib/libopencv_imgcodecs.so
LIBS += /usr/local/lib/libopencv_tracking.so
LIBS += -L/opt/anaconda3/lib -lpython3.6m
LIBS += /opt/Qt5.10.0/5.10.0/gcc_64/lib/libQt5*.so

SOURCES += \
        main.cpp \
        mainwindow.cpp

HEADERS += \
        mainwindow.h

FORMS += \
        mainwindow.ui


win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Core/release/ -lCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Core/debug/ -lCore
else:unix: LIBS += -L$$OUT_PWD/../Core/ -lCore

INCLUDEPATH += $$PWD/../Core
DEPENDPATH += $$PWD/../Core

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Common/release/ -lCommon
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Common/debug/ -lCommon
else:unix: LIBS += -L$$OUT_PWD/../Common/ -lCommon

INCLUDEPATH += $$PWD/../Common
DEPENDPATH += $$PWD/../Common

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/release/ -lBasic_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Basic_algorithm/debug/ -lBasic_algorithm
else:unix: LIBS += -L$$OUT_PWD/../Basic_algorithm/ -lBasic_algorithm

INCLUDEPATH += $$PWD/../Basic_algorithm
DEPENDPATH += $$PWD/../Basic_algorithm

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../Advanced_algorithm/release/ -lAdvanced_algorithm
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../Advanced_algorithm/debug/ -lAdvanced_algorithm
else:unix: LIBS += -L$$OUT_PWD/../Advanced_algorithm/ -lAdvanced_algorithm

INCLUDEPATH += $$PWD/../Advanced_algorithm
DEPENDPATH += $$PWD/../Advanced_algorithm
