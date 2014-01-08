extern "C"
{
#include "../Rocaloid/RocaloidDevTools/CVDBToolChain/TCFILE/SCONF.h"
#include <malloc.h>
#include <string.h>
}
#include <iostream>
#include <QMessageBox>
#include <string>
#include "qstringpchararray.h"

#ifndef SCONFPROCESSOR_H
#define SCONFPROCESSOR_H

class SconfProcessor
{

public:
    SconfProcessor(const char* filename);
    ~SconfProcessor();
    bool SortComparison(TickList_Type a, TickList_Type b);
    void SortBuffer();
    void ReadSconfToBuffer();
    void WriteBufferToSconf();
    TickList_Type* ReadByCount(const int count);
    float ReadTimeByCount(const int count);
    char* ReadCByCount(const int count);
    char* ReadVByCount(const int count);
    int GetIndex();
    int InsertByTime(float Time,const char* C,const char* V);
    int BranchInsertByTime(float Time,const char* C,const char* V);
    int EditByTime(float Time,const char* C,const char* V);
    int EditByCount(const int count,float Time,const char* C,const char* V);
    int ReadCountByTime(float Time);
    void SetPath(const char* topath);
    void DeleteByCount(const int count);
    void DeleteByTime(float Time);
    void EditTimeByCount(int count,float Time);
    void EditCByCount(int count,const char *C);
    void EditVByCount(int count,const char *V);
    int PushNew();

private:
    SCONF* Buffer;
    String* path;

};

#endif // SCONFPROCESSOR_H
