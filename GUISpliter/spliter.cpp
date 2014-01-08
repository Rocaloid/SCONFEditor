#include "MainWindow.h"
#include "ui_spliter.h"

Spliter::Spliter(QWidget *parent) :QMainWindow(parent),ui(new Ui::Spliter)
{
    this->TVBD=false;
    this->SCONFLoaded=false;
    this->WAVELoaded=false;
    this->RefreshLock=true;
    this->ui->setupUi(this);
    this->da=new DrawArea(this,this->centralWidget());
    this->show();
    this->da->show();
    this->setMouseTracking(true);
    this->da->setGeometry(0,0,this->width(),this->height()-79);
    this->da->update();
    this->RefreshLock=true;
    this->RefreshLock=false;
}

Spliter::~Spliter()
{
  delete this->da;
  delete this->ui;
  if(this->SCONFLoaded) delete this->SCONFClass;
  if(this->WAVELoaded) delete this->WAVEClass;
  if(this->TVBD) delete this->TimeValidator;
}

void Spliter::SetPointerTime(float time)
{
    this->ui->PointerDisplay->setText(QString::number(time));
}

void Spliter::SetDisplayStartTime(float time)
{
    this->ui->TimeStartDisplay->setText(QString::number(time));
}

void Spliter::SetDisplayEndTime(float time)
{
    this->ui->TimeEndDisplay->setText(QString::number(time));
}

void Spliter::SetSplitPointCount(int count)
{
    this->ui->SplitPointCount->setText(QString::number(count));
}

void Spliter::SetSplitPointSelected(const QString &qs)
{
    this->ui->SplitPointActived->setText(qs);
}

void Spliter::BuildTimeValidator(float MaxTime)
{
    if(this->TVBD)
    {
        delete this->TimeValidator;
        this->TVBD=false;
    }
    this->TimeValidator=new QDoubleValidator;
    this->TimeValidator->setParent(this);
    this->TimeValidator->setRange(0.0f,MaxTime,3);
    this->ui->PointerDisplay->setValidator(this->TimeValidator);
    this->ui->TimeStartDisplay->setValidator(this->TimeValidator);
    this->ui->TimeEndDisplay->setValidator(this->TimeValidator);
    this->TVBD=true;
}

void Spliter::resizeEvent(QResizeEvent *)
{
    if(!this->RefreshLock)
    {
        this->RefreshLock=true;
        this->Width=this->width();
        this->Height=this->height();
        this->da->setUpdatesEnabled(TRUE);
        this->ui->PlayBackWidget->setGeometry(0,this->Height-77,591,51);
        this->da->resize(this->Width,this->Height-79);
        this->da->repaint();
        this->RefreshLock=false;
    }
}

void Spliter::on_action_SCONF_O_triggered()
{
    QString QSCONFPatht = QFileDialog::getOpenFileName(this,tr("打开SCONF"), QDir::currentPath(), tr(QString::fromUtf8("分割配置文件(*.sconf)(*.sconf)")));
    if(QString::compare(QSCONFPatht,QString("")))
    {
        this->SCONFPath=QSPCA::toChar(QSCONFPatht);
        this->LoadSCONF();
    }
    else std::cout<<"Canceled"<<std::endl;
}

void Spliter::on_action_WAVE_triggered()
{
    QString QWavePatht = QFileDialog::getOpenFileName(this,tr("打开WAVE"), QDir::currentPath(), tr(QString::fromUtf8("波形文件(*.wav)(*.wav)")));
    if(QString::compare(QWavePatht,QString("")))
        {
                this->WAVEPath=QSPCA::toChar(QWavePatht);
                this->LoadWAVE();
        }
    else std::cout<<"Canceled"<<std::endl;
}

void Spliter::on_action_SCONF_2_triggered()
{
    if(this->SCONFLoaded)
    {
        QString qpath = QFileDialog::getSaveFileName(this, tr("保存SCONF"), QDir::currentPath(), tr(QString::fromUtf8("分割配置文件(*.sconf)(*.sconf)")));
        this->SCONFPath=QSPCA::toChar(qpath);
        this->SaveSCONF();
    }
}

void Spliter::on_action_SCONF_triggered()
{
    if(this->SCONFLoaded)
    {
        this->SaveSCONF();
    }
}

void Spliter::LoadSCONF()
{
    if(!access(this->SCONFPath,R_OK))
    {
        if(this->SCONFLoaded)
        {
            this->SCONFLoaded=false;
            delete this->SCONFClass;
        }
        this->SCONFClass=new SconfProcessor(this->SCONFPath);
        this->SCONFClass->ReadSconfToBuffer();
        this->da->SetSconfDrawOn(this->SCONFClass);
        this->SCONFClass->SortBuffer();
        this->setWindowTitle(QString("Spliter - ")+QString(this->SCONFPath));
        this->SCONFLoaded=true;
    }
    else
    {
        std::cout<<"Error on [Spliter]:"<<std::endl;
        std::cout<<"  "<<"When [LoadSCONF],sconf is "<<this->SCONFPath<<":"<<std::endl;
        std::cout<<"  "<<"Cannot access."<<std::endl;
        std::cout<<"Do:[Nothing]."<<std::endl;
    }
}

void Spliter::LoadWAVE()
{
    if(!access(this->WAVEPath,R_OK))
    {
        if(this->WAVELoaded)
        {
            this->WAVELoaded=false;
            delete this->WAVEClass;
        }
        this->WAVEClass=new WaveInput(this->WAVEPath);
        this->BuildTimeValidator(((float)this->WAVEClass->GetLength())/((float)this->WAVEClass->GetSamprate()));
        this->da->SetWaveDrawOn(this->WAVEClass);
        this->WAVELoaded=true;
    }
    else
    {
        std::cout<<"Error on [Spliter]:"<<std::endl;
        std::cout<<"  "<<"When [LoadWAVE],Wave is "<<this->WAVEPath<<":"<<std::endl;
        std::cout<<"  "<<"Cannot access."<<std::endl;
        std::cout<<"Do:[Nothing]."<<std::endl;
    }
}

void Spliter::SaveSCONF()
{
    if(SCONFLoaded)
    {
        this->SCONFClass->SetPath(this->SCONFPath);
        this->SCONFClass->SortBuffer();
        this->SCONFClass->WriteBufferToSconf();
        this->LoadSCONF();
        this->da->update();
        this->update();
    }
    else std::cout<<"SCONF have been not opened yet."<<std::endl;
}

void Spliter::on_action_5_triggered()
{
    if(this->WAVELoaded & this->SCONFLoaded)
    {
        int SP=this->SCONFClass->PushNew();
        this->SCONFClass->EditTimeByCount(SP,this->da->GetPointerTime());
        SPEditDialog *SPED=new SPEditDialog(this->SCONFClass,SP,0,true);
        SPED->exec();
        this->SetSplitPointCount(this->SCONFClass->GetIndex());
        delete SPED;
        this->da->update();
    }
}

void Spliter::on_action_triggered()
{
    if(this->SCONFLoaded)
    {
        this->SCONFClass->SortBuffer();
        this->da->ResetActivedSP();
        this->update();
    }
}

void Spliter::on_PointerDisplay_returnPressed()
{
    this->da->SetPointerTime(this->ui->PointerDisplay->text().toFloat());
}

void Spliter::on_TimeStartDisplay_returnPressed()
{
    if(this->WAVELoaded)
    {
        this->da->SetDisplaySampleStart(this->ui->TimeStartDisplay->text().toFloat()*this->WAVEClass->GetSamprate());
    }
}

void Spliter::on_action_6_triggered()
{
    if(this->SCONFLoaded)
    {
        int Index0=this->SCONFClass->GetIndex();
        int ASP=this->da->GetActivedSP();
        if(ASP!=-1)
        {
            this->SCONFClass->DeleteByCount(ASP);
            if(ASP>this->SCONFClass->GetIndex())
                this->da->ResetActivedSP();
            else if(this->SCONFClass->GetIndex()!=Index0)
            {
                if(this->da->GetActivedSP()>ASP)
                {
                    this->da->SetActivedSP(this->da->GetActivedSP()+this->SCONFClass->GetIndex()-Index0);
                }
            }
            this->da->RefreshUplevelSPAS();
            this->da->update();
        }
    }
}

void Spliter::on_action_X_triggered()
{
    this->close();
}

void Spliter::on_TimeEndDisplay_returnPressed()
{
    if(this->WAVELoaded)
    {
        this->da->SetDisplaySampleEnd(this->ui->TimeEndDisplay->text().toFloat()*this->WAVEClass->GetSamprate());
    }
}
