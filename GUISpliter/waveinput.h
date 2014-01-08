#include <stdio.h>
#include <string.h>
#include "../Rocaloid/RUtil/RUtil.h"
#include "../Rocaloid/RUtil/IO/Wave.h"
#include <iostream>
#include <malloc.h>

#ifndef WAVEINPUT_H
#define WAVEINPUT_H

/*
 *Support 1/2 channel 8/16 bit only.
 */

typedef struct
{
    int Max;
    int Min;
} MaxmiumAndMinmiumTree;

class WaveInput
{
public:
    WaveInput(const char *filename);
    ~WaveInput();
    inline const MaxmiumAndMinmiumTree TreelizeWave(int Start, int End, int Accuracy)
    {
    #define DATA8 ((char *)this->WAVE_DATA)
    #define DATA16 ((short *)this->WAVE_DATA)
        MaxmiumAndMinmiumTree Tree;
        int i;
        if(this->Data_Long==1)    //8 bit samplerate
        {
            Tree.Max=DATA8[Start];
            Tree.Min=DATA8[Start];
            for(i=Start;i<End;i+=Accuracy)
            {
                if(Tree.Max<DATA8[i])
                {
                    Tree.Max=DATA8[i];
                }
                else if(Tree.Min>DATA8[i])
                {
                    Tree.Min=DATA8[i];
                }
            }
        }
        if(this->Data_Long==2)    //8 bit samplerate
        {
            Tree.Max=DATA16[Start];
            Tree.Min=DATA16[Start];
            for(i=Start;i<End;i+=Accuracy)
            {
                if(Tree.Max<DATA16[i])
                {
                    Tree.Max=DATA16[i];
                }
                else if(Tree.Min>DATA16[i])
                {
                    Tree.Min=DATA16[i];
                }
            }
        }
        return (const MaxmiumAndMinmiumTree)Tree;
    #undef DATA8
    #undef DATA16
    }
    void ShowWaveInfo();
    int GetSamprate();
    void ReleaseTreeBuffer();
    const void* GetSampleData();
    int GetBitPerSample();
    int GetLength();
private:
    int Data_Long;
    bool WAVE_LOADED;
    PCMWaveFile WaveStream;
    void *WAVE_DATA;
    int WAVE_DATA_LENGTH;
    int WAVE_SAMPLE_LENGTH;
};

#endif // WAVEINPUT_H
