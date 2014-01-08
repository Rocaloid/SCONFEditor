#include "waveinput.h"
using namespace std;

WaveInput::WaveInput(const char *filename)
{
    this->WAVE_LOADED=true;
    String Rfilename;
    String_Ctor(&Rfilename);
    String_SetChars(&Rfilename,filename);
    PCMWaveFile_Ctor(&(this->WaveStream));
    if(!PCMWaveFile_Open(&(this->WaveStream),&Rfilename))
    {
        cout<<"Error on [WaveStream]:"<<endl;
        cout<<"  "<<"When [Ctor],wave is "<<String_GetChars(&Rfilename)<<":"<<endl;
        cout<<"  "<<"Cannot open wave file."<<endl;
        cout<<"Do:[nothing]."<<endl;
        this->WAVE_LOADED=false;
    }
    this->Data_Long=this->WaveStream.Header.bitPerSample/8;
    if(WAVE_LOADED)
    {
        this->WAVE_DATA=(void *)malloc(WaveStream.Header.dataSize);
        if(this->WAVE_DATA==NULL)
        {
            cout<<"Error on [WaveInput]:"<<endl;
            cout<<"  "<<"When [Ctor],wave is "<<String_GetChars(&Rfilename)<<":"<<endl;
            cout<<"  "<<"Cannot alloc memory:"<<(WaveStream.Header.dataSize)/1024/1024<<"MiB"<<endl;
            cout<<"Do:[nothing]."<<endl;
            free(this->WAVE_DATA);
            WAVE_LOADED=false;
        }
        if(WAVE_LOADED)
        {
            this->WAVE_DATA_LENGTH=PCMWaveFile_FetchAll((char *)this->WAVE_DATA,&(this->WaveStream));
            if(this->WAVE_DATA_LENGTH>0)
            {
                this->WAVE_LOADED=true;
                this->WAVE_SAMPLE_LENGTH=this->WaveStream.Header.dataSize/this->Data_Long;
            }
        }
    }
    else
    {
        cout<<"Error on [WaveInput]:"<<endl;
        cout<<"  "<<"When [Ctor],wave is "<<String_GetChars(&Rfilename)<<":"<<endl;
        cout<<"  "<<"Cannot fetch wave data."<<endl;
        cout<<"Do:[nothing]."<<endl;
        this->WAVE_LOADED=false;
    }
    PCMWaveFile_Close(&(this->WaveStream));
    String_Dtor(&Rfilename);
}

WaveInput::~WaveInput()
{
    PCMWaveFile_Dtor(&(this->WaveStream));
    if(this->WAVE_LOADED)
    {
        free(this->WAVE_DATA);
    }
}

void WaveInput::ShowWaveInfo()
{
    if(this->WAVE_LOADED)
    {
        cout<<"RIFF WAVE:SampleCount="<<this->WAVE_DATA_LENGTH<<",SampleRate="<<this->GetSamprate()<<endl;
    }
    else
    {
        cout<<"Error on [WaveStream]:"<<endl;
        cout<<"  "<<"When [GetSamprate]:"<<endl;
        cout<<"  "<<"Wave is not loaded."<<endl;
        cout<<"Do:[nothing]."<<endl;
    }
}

int WaveInput::GetSamprate()
{
    if(this->WAVE_LOADED)
    {
        return this->WaveStream.Header.samplePerSecond;
    }
    else
    {
        cout<<"Error on [WaveStream]:"<<endl;
        cout<<"  "<<"When [GetSamprate]:"<<endl;
        cout<<"  "<<"Wave is not loaded."<<endl;
        cout<<"Do:[return -1;]."<<endl;
        return -1;
    }
}

const void* WaveInput::GetSampleData()
{
    return (const void*)this->WAVE_DATA;
}

int WaveInput::GetBitPerSample()
{
    return (int)this->WaveStream.Header.bitPerSample;
}

int WaveInput::GetLength()
{
    return this->WaveStream.Header.dataNum;
}
