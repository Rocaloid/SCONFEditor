#ifndef SPEDITDIALOG_H
#define SPEDITDIALOG_H

#include <QDialog>
#include "sconfprocessor.h"
#include "qstringpchararray.h"

namespace Ui {
class SPEditDialog;
}

class SPEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SPEditDialog(SconfProcessor* SClass,int SNum,QWidget *parent = 0,bool newsp = false);
    ~SPEditDialog();

protected:
    void closeEvent(QCloseEvent *);

private slots:
    void on_DeleteSplitPoint_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::SPEditDialog *ui;
    SconfProcessor* SCONFClass;
    int SPNumber;
    bool NewSP;
};

#endif // SPEDITDIALOG_H
