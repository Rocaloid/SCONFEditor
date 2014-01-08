#include "speditdialog.h"
#include "ui_speditdialog.h"

SPEditDialog::SPEditDialog(SconfProcessor *SClass, int SNum, QWidget *parent,bool newsp):QDialog(parent),ui(new Ui::SPEditDialog)
{
    ui->setupUi(this);
    this->SCONFClass=SClass;
    this->SPNumber=SNum;
    this->NewSP=newsp;
    this->ui->SplitPointNumber->setText(QString::number(this->SPNumber));
    this->ui->SplitPointTime->setText(QString::number(this->SCONFClass->ReadTimeByCount(this->SPNumber)));
    if(!newsp)
    {
        this->ui->SplitPointC->setText(QString(this->SCONFClass->ReadCByCount(this->SPNumber)));
        this->ui->SplitPointV->setText(QString(this->SCONFClass->ReadVByCount(this->SPNumber)));
        this->setWindowTitle(QString("分割点 ")+QString::number(this->SPNumber)+QString(" @ ")+this->ui->SplitPointTime->text());
    }
    else
        this->setWindowTitle(QString("新建分割点 ")+QString::number(this->SPNumber)+QString(" @ ")+this->ui->SplitPointTime->text());
}

SPEditDialog::~SPEditDialog()
{
    delete ui;
}

void SPEditDialog::on_DeleteSplitPoint_clicked()
{
    this->SCONFClass->DeleteByCount(this->SPNumber);
    this->NewSP=false;
    this->close();
}

void SPEditDialog::on_buttonBox_accepted()
{
    this->SCONFClass->EditCByCount(this->SPNumber,QSPCA::toChar(this->ui->SplitPointC->text()));
    this->SCONFClass->EditVByCount(this->SPNumber,QSPCA::toChar(this->ui->SplitPointV->text()));
    this->SCONFClass->EditTimeByCount(this->SPNumber,this->ui->SplitPointTime->text().toFloat());
    this->NewSP=false;
    this->close();
}

void SPEditDialog::on_buttonBox_rejected()
{
    this->close();
}

void SPEditDialog::closeEvent(QCloseEvent *)
{
    if(this->NewSP)
        this->SCONFClass->DeleteByCount(this->SPNumber);
}
