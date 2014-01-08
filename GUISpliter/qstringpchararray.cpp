#include "qstringpchararray.h"
#include <iostream>
namespace QSPCA
{
    Struct *llh;

    char *toChar(const QString &s)
    {
        QByteArray a=s.toLatin1();
        Struct *tab=new Struct;
        tab->next=llh;
        tab->Characters=new char[a.size()+1];
        strcpy(tab->Characters,a.data());
        llh=tab;
        return llh->Characters;
    }

    void Init()
    {
        llh=NULL;
    }

    void Destroy()
    {
        for(Struct *curr=llh;curr;)
        {
            Struct *entry = curr;
            curr = entry->next;
            delete[] entry->Characters;
            delete entry;
        }
    }
}
