#include <QObject>
#include <QString>
#include <string.h>

#ifndef QSTRINGPCHARARRAY_H
#define QSTRINGPCHARARRAY_H

namespace QSPCA
{
    typedef struct Struct
    {
        Struct *next;
        char *Characters;
    }Struct;

    void Init();
    char *toChar(const QString &s);
    void Destroy();
}

#endif // QSTRINGPCHARARRAY_H
