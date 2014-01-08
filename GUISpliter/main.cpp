#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[])
{
/*
    using namespace std;
    QSPCA::Init();
    QString a=QString("Hello World!你好世界!");
    cout<<"01:"<<QSPCA::toChar(QString("1Hello World!你好世界!"))<<endl;
    cout<<"02:"<<QSPCA::toChar(QString("2Hello World!你好世界!"))<<endl;
    QSPCA::Destroy();
*/

/*
    using namespace std;
    WaveInput* x=new WaveInput("/home/tuxzz/RoDev/RocaloidUI/raw.wav");
    x->ShowWaveInfo();
    int i;
    int w=20;
    int StartFrame=0;
    int EndFrame=x->GetLength();
    int DisplayFrame=EndFrame-StartFrame;
    int FramesPerPixel=DisplayFrame/w;
    MaxmiumAndMinmiumTree t;
    for(i=0;i<w;i++)
    {
        t=x->TreelizeWave(i*FramesPerPixel,(i+1)*FramesPerPixel,8);
        cout<<"Pixel "<<i<<":Max="<<t.Max<<",Min="<<t.Min<<endl;
    }
    delete x;
*/

/*
    int i;
    SconfProcessor* x=new SconfProcessor("/home/tuxzz/RoDev/RocaloidUI/test.sconf");
    //x->SetPath("/home/tuxzz/RoDev/RocaloidUI/test.sconf");
    TickList_Type* t;
    x->ReadSconfToBuffer();
    x->SortBuffer();
    int co=x->GetIndex();

    for(i=0;i<=co;i++)
        {
            t=x->ReadByCount(i);
            std::cout<<"T:"<<t->Time<<",C:"<<String_GetChars(&(t->Consonant))<<",V:"<<String_GetChars(&(t->Vowel))<<std::endl;
        }
    x->WriteBufferToSconf();

    delete x;
    return 0;
*/


    QApplication a(argc, argv);
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForTr(codec);
    QSPCA::Init();
    Spliter w;
    int r=a.exec();
    QSPCA::Destroy();
    return r;

}
