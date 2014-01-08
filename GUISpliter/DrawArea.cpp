#include "MainWindow.h"

using namespace std;

DrawArea::DrawArea(Spliter *uplevel,QWidget *parent):QWidget(parent)
{
    this->MinmiumTime=0.05f;
    this->WaveCacheCreated=false;
    this->WaveReDrawLock=false;
    this->SPReDrawLock=false;
    this->DisplayEndSample=0;
    this->DisplayBeginSample=0;
    this->CanDrawSplitPoint=false;
    this->CanDrawWave=false;
    this->Pen_MidLine=QPen(QColor(160,160,0));
    this->Pen_Wave=QPen(QColor(0,192,128));
    this->Pen_SPShow=QPen(QColor(32,64,192));
    this->Pen_SplitLine_inactive=QPen(QBrush(QColor(128,128,255)),1,Qt::DotLine);
    this->Pen_SplitLine_active=QPen(QBrush(QColor(255,128,255)),1,Qt::DotLine);
    this->Pen_Pointer=QPen(QBrush(QColor(200,200,128)),1,Qt::DashLine);
    this->Brush_SplitPoint_inactive=QBrush(QColor(0,255,0));
    this->Brush_SplitPoint_active=QBrush(QColor(255,0,0));
    this->Brush_Background=QBrush(QColor(20,20,20));
    this->Brush_Panel=QBrush(QColor(162,162,162));
    this->MousePPressing=false;
    this->ActivedSP=-1;
    this->MouseSPressing=false;
    this->MousePosR=QPoint(0,0);
    this->SelectPointer=-1;
    this->UpLevel=uplevel;
    this->setMouseTracking(true);
}
DrawArea::~DrawArea()
{
    this->CanDrawSplitPoint=false;
    this->CanDrawWave=false;
    this->CleanWaveCache();
}
void DrawArea::drawText(QPainter *p,int x,int y,char *text) {
    p->save();
    p->setFont(QFont("Sans",10));
    p->drawText(x, y, text);
    p->restore();
}

void DrawArea::drawText(QPainter *p,int x,int y,int number) {
    p->save();
    p->setFont(QFont("Sans",10));
    p->drawText(x,y,QString::number(number));
    p->restore();
}

void DrawArea::drawText(QPainter *p,int x,int y,float number) {
    p->save();
    p->setFont(QFont("Sans",10));
    p->drawText(x,y,QString::number(number));
    p->restore();
}

void DrawArea::paintEvent(QPaintEvent *){
    QPainter D;
    int w=this->width(),h=this->height();
    int MidH=this->height()/2;
    D.begin(this);
        D.setRenderHint(QPainter::Antialiasing, false);
        D.setPen(Qt::NoPen);
        D.setBrush(this->Brush_Background);
        D.drawRect(0,0,this->width(),this->height());
        D.setPen(this->Pen_MidLine);
        D.drawLine(0,MidH,this->width(),MidH);
        D.setPen(Qt::NoPen);
        D.setBrush(this->Brush_Panel);
        D.drawRect(0,h-16,w,h);
        if(this->CanDrawWave)
        {
            this->WaveReDraw(&D);
            if(this->CanDrawSplitPoint)
            {
                this->SplitPointReDraw(&D);
                int cSP=CheckOnSP(this->MousePosR.x());
                if(cSP!=-1)
                {
                    D.save();
                    D.setPen(this->Pen_SPShow);
                    D.setFont(QFont("Sans",10));
                    QString t=QString("C:")+QString(this->SCONFClass->ReadCByCount(cSP))+QString(" V:")+QString(this->SCONFClass->ReadVByCount(cSP))+QString(" T:")+QString::number(this->SCONFClass->ReadTimeByCount(cSP));
                    D.drawText(this->MousePosR,t);
                    D.restore();
                }
            }
        }
        this->DrawPointer(&D);
    D.end();
}

void DrawArea::DrawPointer(QPainter *p)
{
    if(this->CanDrawWave)
    {
        p->setPen(this->Pen_Pointer);
        int w=this->width(),h=this->height();
        int DisplaySample=this->DisplayEndSample-this->DisplayBeginSample;
        float fpp=((float)DisplaySample)/((float)w);
        int pointerx=(this->SelectPointer-this->DisplayBeginSample)/fpp;
        if(pointerx>=0)
        {
            p->drawLine(pointerx,h-16,pointerx,0);
        }
    }
}

float DrawArea::GetPointerTime()
{
    if(this->CanDrawWave)
    {
        return ((float)this->SelectPointer)/((float)this->WAVEClass->GetSamprate());
    }
    return -1.0f;
}

void DrawArea::SetPointerTime(float time)
{
    if(this->CanDrawWave)
    {
        this->SelectPointer=time*this->WAVEClass->GetSamprate();
        if(this->SelectPointer>this->WAVEClass->GetLength())
            this->SelectPointer=this->WAVEClass->GetLength();
        else if(this->SelectPointer<0)
            this->SelectPointer=0;
        this->UpLevel->SetPointerTime(((float)this->SelectPointer)/((float)this->WAVEClass->GetSamprate()));
        if(this->SelectPointer<this->DisplayBeginSample || this->SelectPointer>this->DisplayEndSample)
        {
            int dispd=(this->DisplayEndSample-this->DisplayBeginSample)/2;
            this->SetDisplayTime(this->SelectPointer-dispd,this->SelectPointer+dispd);
        }
        this->update();
    }
}

void DrawArea::SetDisplaySampleStart(int c)
{
    if(this->CanDrawWave && c>=0 && c<this->DisplayEndSample)
    {
        this->DisplayBeginSample=c;
        this->UpLevel->SetDisplayStartTime((((float)c)/((float)this->WAVEClass->GetSamprate())));
        this->CleanWaveCache();
        this->update();
    }
}

void DrawArea::SetDisplaySampleEnd(int c)
{
    if(this->CanDrawWave && c<=this->WAVEClass->GetLength() && c>this->DisplayBeginSample)
    {
        this->DisplayEndSample=c;
        this->UpLevel->SetDisplayEndTime((((float)c)/((float)this->WAVEClass->GetSamprate())));
        this->CleanWaveCache();
        this->update();
    }
}

int DrawArea::GetPointerSample()
{
    return this->SelectPointer;
}

void DrawArea::ResetActivedSP()
{
    this->ActivedSP=-1;
    this->UpLevel->SetSplitPointSelected(QString(" -- "));
    this->update();
}

void DrawArea::SetActivedSP(int SP)
{
    this->ActivedSP=SP;
    this->RefreshUplevelSPAS();
    this->update();
}

int DrawArea::GetActivedSP()
{
    return this->ActivedSP;
}

void DrawArea::RefreshUplevelSPAS()
{
    if(this->CanDrawSplitPoint)
    {
        if(this->ActivedSP!=-1)
            this->UpLevel->SetSplitPointSelected(QString::number(this->ActivedSP)+QString(" T:")+QString::number(this->SCONFClass->ReadTimeByCount(this->ActivedSP))+QString(",C:")+QString(this->SCONFClass->ReadCByCount(this->ActivedSP))+QString(",V:")+QString(this->SCONFClass->ReadVByCount(this->ActivedSP)));
        else
            this->UpLevel->SetSplitPointSelected(QString(" -- "));
        this->UpLevel->SetSplitPointCount(this->SCONFClass->GetIndex());
    }
}

int DrawArea::CheckOnSP(int x)
{
    if(this->CanDrawSplitPoint & this->CanDrawWave)
    {
        int index=this->SCONFClass->GetIndex();
        int w=this->width();
        float DisplayBeginTime=((float)this->DisplayBeginSample)/((float)this->WAVEClass->GetSamprate());
        float DisplayEndTime=((float)this->DisplayEndSample)/((float)this->WAVEClass->GetSamprate());
        float DisplayTime=((float)(this->DisplayEndSample-this->DisplayBeginSample))/((float)this->WAVEClass->GetSamprate());
        float TimePerPixel=DisplayTime/((float)w);
        int SPX;
        int i;
        TickList_Type* temp;
        while(this->SPReDrawLock)
        {
            //Do nothing;
        }
        this->SPReDrawLock=true;
        for(i=0;i<=index;++i)
        {
            temp=this->SCONFClass->ReadByCount(i);
            if((temp->Time>=DisplayBeginTime-4) & (temp->Time<=DisplayEndTime+4))
            {
                SPX=(temp->Time-DisplayBeginTime)/TimePerPixel;
                if(SPX<x+4 and SPX>x-4)
                {
                    this->SPReDrawLock=false;
                    return i;
                }
            }
        }
        this->SPReDrawLock=false;
    }
    return -1;
}

void DrawArea::SplitPointReDraw(QPainter* p)
{
    if(!this->SPReDrawLock)
    {
        this->SPReDrawLock=true;
        int w=this->width(),h=this->height();
        float DisplayBeginTime=((float)this->DisplayBeginSample)/((float)this->WAVEClass->GetSamprate());
        float DisplayEndTime=((float)this->DisplayEndSample)/((float)this->WAVEClass->GetSamprate());
        float DisplayTime=((float)(this->DisplayEndSample-this->DisplayBeginSample))/((float)this->WAVEClass->GetSamprate());
        float TimePerPixel=DisplayTime/((float)w);
        int SPX;
        int index=this->SCONFClass->GetIndex();
        int i;
        TickList_Type* temp;
        this->UpLevel->SetSplitPointCount(index);
        for(i=0;i<=index;++i)
        {
            temp=this->SCONFClass->ReadByCount(i);
            if((temp->Time>=DisplayBeginTime-4) & (temp->Time<=DisplayEndTime+4))
            {
                SPX=(temp->Time-DisplayBeginTime)/TimePerPixel;
                if(i==this->ActivedSP)
                {
                    p->setBrush(this->Brush_SplitPoint_active);
                    p->setPen(this->Pen_SplitLine_active);
                }
                else
                {
                    p->setBrush(this->Brush_SplitPoint_inactive);
                    p->setPen(this->Pen_SplitLine_inactive);
                }
                p->drawRect(SPX-4,h-10,9,8);
                p->drawLine(SPX,h-11,SPX,0);
            }
        }
        this->SPReDrawLock=false;
    }
}

void DrawArea::WaveReDraw(QPainter* p)
{
    if(!this->WaveReDrawLock)
    {
        this->WaveReDrawLock=true;
        p->setPen(this->Pen_Wave);
        int w=this->width(),h=this->height();
        int i;
        if(this->WaveCacheCreated)
        {
            for(i=0;i<w;++i)
            {
                p->drawLine(i,this->WaveCache[i].Max,i,this->WaveCache[i].Min);
            }
        }
        else
        {
            int bitdepth=this->WAVEClass->GetBitPerSample();
            this->WaveCache=new MaxmiumAndMinmiumTree[w];
            int MidH=h/2;
            int DisplaySample=this->DisplayEndSample-this->DisplayBeginSample;
            int FramesPerPixel=DisplaySample/w;
            MaxmiumAndMinmiumTree tb;
            int Acc=FramesPerPixel/768+1;
            if(bitdepth==16)
            {
                for(i=0;i<w;++i)
                {
                    tb=this->WAVEClass->TreelizeWave(this->DisplayBeginSample+i*FramesPerPixel,this->DisplayBeginSample+(i+1)*FramesPerPixel,Acc);
                    this->WaveCache[i].Max=MidH+(tb.Max/32767.0f*MidH);
                    this->WaveCache[i].Min=MidH+(tb.Min/32767.0f*MidH);
                    p->drawLine(i,this->WaveCache[i].Max,i,this->WaveCache[i].Min);
                }
            }
            else
            {
                for(i=0;i<w;++i)
                {
                    tb=this->WAVEClass->TreelizeWave(this->DisplayBeginSample+i*FramesPerPixel,this->DisplayBeginSample+(i+1)*FramesPerPixel,Acc);
                    this->WaveCache[i].Max=MidH+(tb.Max/127.0f*MidH);
                    this->WaveCache[i].Min=MidH+(tb.Min/127.0f*MidH);
                    p->drawLine(i,this->WaveCache[i].Max,i,this->WaveCache[i].Min);
                }
            }
            this->WaveCacheCreated=true;
        }
        this->WaveReDrawLock=false;
    }
}

void DrawArea::CleanWaveCache()
{
    if(this->WaveCacheCreated)
    {
        delete[] this->WaveCache;
        this->WaveCacheCreated=false;
    }
}

void DrawArea::resizeEvent(QResizeEvent *)
{
    this->WaveReDrawLock=true;
    this->CleanWaveCache();
    this->WaveReDrawLock=false;
}

bool DrawArea::GetWaveDrawStatus(void)
{
    return this->CanDrawWave;
}

bool DrawArea::GetSconfDrawStatus(void)
{
    return this->CanDrawSplitPoint;
}

void DrawArea::SetWaveDrawOn(WaveInput *WaveClass)
{
    if(this->CanDrawWave)
    {
        this->CanDrawWave=false;
    }
    this->WAVEClass=WaveClass;
    this->SetDisplayTime(0,this->WAVEClass->GetLength());
    this->MinmiumSample=this->MinmiumTime*this->WAVEClass->GetSamprate();
    this->SelectPointer=0;
    this->UpLevel->SetPointerTime(0.0f);
    this->CanDrawWave=true;
}

void DrawArea::SetWaveDrawOff(void)
{
    CanDrawWave=false;
    this->CleanWaveCache();
    this->WAVEClass=NULL;
}

void DrawArea::SetSconfDrawOn(SconfProcessor *SconfClass)
{
    if(this->CanDrawSplitPoint)
    {
        this->CanDrawSplitPoint=false;
    }
    this->SCONFClass=SconfClass;
    this->CanDrawSplitPoint=true;
}

void DrawArea::SetSconfDrawOff(void)
{
    this->CanDrawSplitPoint=false;
    this->SCONFClass=NULL;
}

void DrawArea::SetDisplayTime(int begin,int end)
{
    if((begin<0) || (end>this->WAVEClass->GetLength()) || (end<begin))
    {
        cout<<"OOPS! SetDisplayTime("<<begin<<","<<end<<")"<<endl;
    }
    else
    {
        this->DisplayBeginSample=begin;
        this->DisplayEndSample=end;
        this->UpLevel->SetDisplayStartTime(((float)begin)/((float)this->WAVEClass->GetSamprate()));
        this->UpLevel->SetDisplayEndTime(((float)end)/((float)this->WAVEClass->GetSamprate()));
        this->CleanWaveCache();
        this->update();
    }
}

void DrawArea::wheelEvent(QWheelEvent *event)
{
    int delta=event->delta();
    if(this->CanDrawWave)
    {
        int w=this->width();
        int DisplaySample=this->DisplayEndSample-this->DisplayBeginSample;
        float percentx=((float)event->pos().x())/((float)w);
        float mpercentx=1-percentx;
        float ScaleSample=DisplaySample*0.1f;
        int ScaleFrameTop,ScaledFrameBegin,ScaledFrameEnd,ScaledFrame;
        if(delta>0)
        {
            ScaledFrameBegin=this->DisplayBeginSample+ScaleSample*percentx;
            ScaledFrameEnd=this->DisplayEndSample-ScaleSample*mpercentx;
            ScaledFrame=ScaledFrameEnd-ScaledFrameBegin;
            if(DisplaySample<this->MinmiumSample)
            {
                ScaleFrameTop=(this->MinmiumSample-ScaledFrame)/2;
                ScaledFrameBegin-=ScaleFrameTop;
                ScaledFrameEnd+=ScaleFrameTop;
            }
            else if(ScaleSample<this->MinmiumSample)
            {
                ScaleFrameTop=(this->MinmiumSample-ScaledFrame)/2;
                ScaledFrameBegin-=ScaleFrameTop;
                ScaledFrameEnd+=ScaleFrameTop;
                this->SetDisplayTime(ScaledFrameBegin,ScaledFrameEnd);
            }
            else
            {
                this->SetDisplayTime(ScaledFrameBegin,ScaledFrameEnd);
            }
        }
        else
        {
            ScaledFrameBegin=this->DisplayBeginSample-ScaleSample*percentx;
            ScaledFrameEnd=this->DisplayEndSample+ScaleSample*mpercentx;
            if(DisplaySample==this->WAVEClass->GetLength())
            {
                //Do nothing.
            }
            else
            {
                if(ScaledFrameBegin<0)
                {
                    ScaledFrameBegin=0;
                }
                if(ScaledFrameEnd>this->WAVEClass->GetLength())
                {
                    ScaledFrameEnd=this->WAVEClass->GetLength();
                }
                this->SetDisplayTime(ScaledFrameBegin,ScaledFrameEnd);
            }
        }
    }
    event->accept();
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    int x=event->x(),y=event->y();
    this->MousePosR=QPoint(x,y);
    int w=this->width();
    int delta=x-this->MousePosB;
    this->MousePosB=event->x();
    if((event->buttons() & Qt::LeftButton))
    {
        if(this->MousePPressing && this->CanDrawWave)
        {
            int fpp=(this->DisplayEndSample-this->DisplayBeginSample)/w*(-delta);
            int B=this->DisplayBeginSample+fpp;
            int E=this->DisplayEndSample+fpp;
            if((B>=0) & (E<=this->WAVEClass->GetLength()))
            {
                this->SetDisplayTime(B,E);
            }
            this->CleanWaveCache();
        }
        else if((this->MouseSPressing) && (this->CanDrawSplitPoint) && (this->ActivedSP!=-1) && (this->CanDrawWave))
        {
            float DisplayTime=(this->DisplayEndSample-this->DisplayBeginSample)/this->WAVEClass->GetSamprate();
            float tpp=DisplayTime/((float)w);
            this->SCONFClass->EditTimeByCount(this->ActivedSP,this->SCONFClass->ReadTimeByCount(this->ActivedSP)+delta*tpp);
            this->RefreshUplevelSPAS();
        }
        event->accept();
    }
    this->update();
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
    int x=event->x(),y=event->y();
    int w=this->width(),h=this->height();
    int SP=CheckOnSP(x);
    if((event->button() == Qt::RightButton) && (y>this->height()-16) && (SP!=-1) && (this->CanDrawSplitPoint) && (this->CanDrawWave))
    {
        SPEditDialog *SPED=new SPEditDialog(this->SCONFClass,SP,0);
        int Index0=this->SCONFClass->GetIndex();
        SPED->exec();
        if(SP>this->SCONFClass->GetIndex())
            this->ResetActivedSP();
        else if(this->SCONFClass->GetIndex()!=Index0)
        {
            if(this->ActivedSP>SP)
            {
                this->ActivedSP+=this->SCONFClass->GetIndex()-Index0;
            }
        }
        this->RefreshUplevelSPAS();
        delete SPED;
        this->update();
    }
    else if((event->button() == Qt::LeftButton) && (y<h-2) && (y>h-10) && (this->CanDrawSplitPoint) && (this->CanDrawWave))
    {
        this->MousePosB=event->x();
        this->MouseSPressing=true;
        this->SetActivedSP(SP);
    }
    else if((event->button() == Qt::LeftButton) && (y<this->height()-16) && (this->CanDrawWave))
    {
        this->MousePosB=event->x();
        this->MousePPressing=true;
        int DisplaySample=this->DisplayEndSample-this->DisplayBeginSample;
        float fpp=((float)DisplaySample)/((float)w);
        this->SelectPointer=this->DisplayBeginSample+x*fpp;
        this->UpLevel->SetPointerTime(((float)this->SelectPointer)/((float)this->WAVEClass->GetSamprate()));
        this->update();
    }
    event->accept();
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        this->MousePPressing=false;
        this->MouseSPressing=false;
        event->accept();
    }
}
