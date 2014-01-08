#-------------------------------------------------
#
# Project created by QtCreator 2013-12-08T13:26:50
#
#-------------------------------------------------

QT       += core gui
QMAKE_CXXFLAGS_RELEASE+=-std=gnu++11 -march=native -pipe -Ofast -ftree-vectorize
QMAKE_CFLAGS_RELEASE+=-std=gnu99 -march=native -pipe -Ofast -ftree-vectorize
QMAKE_LFLAGS_RELEASE+=-Wl,-O1,--as-needed,--sort-common
QMAKE_CXXFLAGS_DEBUG+=-std=gnu++11
QMAKE_CFLAGS_DEBUG+=-std=gnu99 -O1
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GUISpliter
TEMPLATE = app


SOURCES += main.cpp\
        spliter.cpp \
    DrawArea.cpp \
    sconfprocessor.cpp \
    waveinput.cpp \
    speditdialog.cpp \
    qstringpchararray.cpp \
    $$PWD/../Rocaloid/RocaloidDevTools/CVDBToolChain/TCFILE/SCONF.c \
    $$PWD/../Rocaloid/RUtil/Structure/String.c \
    $$PWD/../Rocaloid/RUtil/IO/Wave.c \
    $$PWD/../Rocaloid/RUtil/IO/FileStream.c \
    $$PWD/../Rocaloid/RUtil/IO/FileUtil.c \
    $$PWD/../Rocaloid/RUtil/IO/StringStream.c \
    $$PWD/../Rocaloid/RUtil/Misc/Converter.c \
    $$PWD/../Rocaloid/RocaloidEngine/RFILE3/RDL.c \

HEADERS  += \
    sconfprocessor.h \
    waveinput.h \
    speditdialog.h \
    qstringpchararray.h \
    MainWindow.h \
    $$PWD/../Rocaloid/RUtil/RUtil.h \
    $$PWD/../Rocaloid/RocaloidDevTools/CVDBToolChain/TCFILE/SCONF.h \
    $$PWD/../Rocaloid/RUtil/Misc/Converter.h \


FORMS    += spliter.ui \
    speditdialog.ui


INCLUDEPATH += $$PWD/../Rocaloid
DEPENDPATH += $$PWD/../Rocaloid

OTHER_FILES +=

RESOURCES += \
    uirc.qrc
