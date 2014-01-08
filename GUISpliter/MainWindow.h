#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <QMainWindow>
#include <QObject>
#include <QPainter>
#include <QWheelEvent>
#include <QMessageBox>
#include <QFileDialog>
#include <QValidator>
#include "qstringpchararray.h"
#include "speditdialog.h"
#include "sconfprocessor.h"
#include "waveinput.h"

extern "C"
{
#include <string.h>
#include <unistd.h>
}
class DrawArea;

namespace Ui {
  class Spliter;
}

class Spliter : public QMainWindow
{
  Q_OBJECT

public:
  explicit Spliter(QWidget *parent = 0);
  ~Spliter();
  DrawArea *da;
  void LoadSCONF();
  void LoadWAVE();
  void SaveSCONF();
  void SetPointerTime(float time);
  void SetDisplayStartTime(float time);
  void SetDisplayEndTime(float time);

  void BuildTimeValidator(float MaxTime);

  void SetSplitPointCount(int count);
  void SetSplitPointSelected(const QString &qs);
protected:
  void resizeEvent(QResizeEvent *);

private slots:
  void on_action_SCONF_O_triggered();
  void on_action_WAVE_triggered();
  void on_action_SCONF_2_triggered();
  void on_action_SCONF_triggered();
  void on_action_5_triggered();
  void on_TimeEndDisplay_returnPressed();
  void on_action_triggered();

  void on_PointerDisplay_returnPressed();

  void on_TimeStartDisplay_returnPressed();

  void on_action_6_triggered();

  void on_action_X_triggered();

private:
    unsigned int Width;
    unsigned int Height;
    bool RefreshLock;
    bool SCONFLoaded;
    bool WAVELoaded;
    char* SCONFPath;
    char* WAVEPath;

    SconfProcessor* SCONFClass;
    WaveInput* WAVEClass;

    QRect FSRect;
    Ui::Spliter *ui;

    QDoubleValidator *TimeValidator;
    bool TVBD;
};

class DrawArea:public QWidget
{
    Q_OBJECT
public:
        explicit DrawArea(Spliter *uplevel,QWidget* parent=0);
        ~DrawArea();
        void drawText(QPainter *p,int x,int y,char *text);
        void drawText(QPainter *p,int x,int y,int number);
        void drawText(QPainter *p,int x,int y,float number);
        void SetWaveDrawOn(WaveInput *WaveClass);
        void SetWaveDrawOff(void);
        void SetSconfDrawOn(SconfProcessor* SconfClass);
        void SetSconfDrawOff(void);
        void WaveReDraw(QPainter *p);
        void SplitPointReDraw(QPainter* p);
        bool GetSconfDrawStatus(void);
        bool GetWaveDrawStatus(void);
        void CleanWaveCache();
        void SetDisplayTime(int begin, int end);
        int CheckOnSP(int x);
        void DrawPointer(QPainter *p);
        float GetPointerTime();
        int GetPointerSample();
        void ResetActivedSP();
        void SetActivedSP(int SP);
        int GetActivedSP();
        void RefreshUplevelSPAS();
        void SetPointerTime(float time);
        void SetDisplaySampleStart(int c);
        void SetDisplaySampleEnd(int c);
protected:
        void paintEvent(QPaintEvent *);
        void resizeEvent(QResizeEvent *);
        void wheelEvent(QWheelEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);

private:
        QPen Pen_MidLine;
        QPen Pen_Wave;
        QPen Pen_SplitLine_inactive;
        QPen Pen_SplitLine_active;
        QPen Pen_Pointer;
        QPen Pen_SPShow;
        QBrush Brush_SplitPoint_inactive;
        QBrush Brush_SplitPoint_active;
        QBrush Brush_Background;
        QBrush Brush_Panel;
        bool CanDrawWave;
        WaveInput* WAVEClass;
        bool CanDrawSplitPoint;
        SconfProcessor* SCONFClass;
        int DisplayBeginSample;
        int DisplayEndSample;
        bool WaveReDrawLock;
        bool SPReDrawLock;
        float MinmiumTime;
        int MinmiumSample;
        MaxmiumAndMinmiumTree* WaveCache;
        bool WaveCacheCreated;

        int SelectPointer;

        int MousePosB;
        bool MousePPressing;
        bool MouseSPressing;
        int ActivedSP;

        QPoint MousePosR;

        Spliter *UpLevel;
};

#endif // MAINWINDOW_H
