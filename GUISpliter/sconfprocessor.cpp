#include "sconfprocessor.h"

void SconfProcessor::SortBuffer() //Insertion Sort
{
  int i,j;
  TickList_Type temp;
  for(i=1;i<=this->Buffer->TickList_Index;++i)
    {
      temp = this->Buffer->TickList[i];
      for(j=i-1;j>=0 && temp.Time<this->Buffer->TickList[j].Time;--j)
      {
              this->Buffer->TickList[j+1]=this->Buffer->TickList[j];
      }
      this->Buffer->TickList[j+1]=temp;
    }
}

SconfProcessor::SconfProcessor(const char *filename)
{
  this->path=(String *)malloc(sizeof(String));
  this->Buffer=(SCONF *)malloc(sizeof(SCONF));
  String_Ctor(this->path);
  SCONF_Ctor(this->Buffer);
  String_SetChars(this->path,filename);
}

SconfProcessor::~SconfProcessor()
{
  SCONF_Dtor(this->Buffer);
  String_Dtor(this->path);
  free(this->Buffer);
  free(this->path);
}

void SconfProcessor::ReadSconfToBuffer()
{
  SCONFReader_Open(this->path);
  SCONF_Read(this->Buffer);
  SCONFReader_Close();
}

void SconfProcessor::WriteBufferToSconf()
{
  SCONFWriter_Save();
  SCONF_Write(this->Buffer);
  SCONFWriter_Write(this->path);
}

TickList_Type* SconfProcessor::ReadByCount(const int count)
{
  if(count<=this->Buffer->TickList_Index)
    {
      return &(this->Buffer->TickList[count]);
    }
  else{
      std::cout<<"Error on [SconfProcessor]:"<<std::endl;
      std::cout<<"  "<<"When [ReadByCount],sconf is "<<String_GetChars(this->path)<<":"<<std::endl;
      std::cout<<"  "<<"Request out of the range:TickList_Index="<<this->Buffer->TickList_Index<<",Request="<<count<<"."<<std::endl;
      std::cout<<"Do:[return NULL;]."<<std::endl;
      return NULL;
    }
}

float SconfProcessor::ReadTimeByCount(const int count)
{
  return this->ReadByCount(count)->Time;
}

char* SconfProcessor::ReadCByCount(const int count)
{
  return (char *)String_GetChars(&(this->ReadByCount(count)->Consonant));
}

char* SconfProcessor::ReadVByCount(const int count)
{
  return (char *) String_GetChars(&(this->ReadByCount(count)->Vowel));
}

int SconfProcessor::GetIndex()
{
  return Buffer->TickList_Index;
}

int SconfProcessor::ReadCountByTime(float Time)
{
  int i;
  for(i=0;i<=Buffer->TickList_Index;++i)
    {
      if(Buffer->TickList[i].Time==Time)
        {
          return i;
        }
    }
  return -1;
}

int SconfProcessor::InsertByTime(float Time, const char *C, const char *V) {
  ArrayType_PushNull(TickList_Type,Buffer->TickList);
  int count=Buffer->TickList_Index;
  EditByCount(count,Time,C,V);
  return count;
}

int SconfProcessor::BranchInsertByTime(float Time, const char *C, const char *V) {
  if(ReadCountByTime(Time)==-1){
    ArrayType_PushNull(TickList_Type,Buffer->TickList);
    int count=Buffer->TickList_Index;
    EditByCount(count,Time,C,V);
    SortBuffer();
    return count;
   }
  return -1;
}

int SconfProcessor::EditByCount(const int count,float Time,const char* C,const char* V)
{
  if(count<=Buffer->TickList_Index)
    {
      String c;
      String v;
      String_Ctor(&c);
      String_Ctor(&v);
      String_SetChars(&c,C);
      String_SetChars(&v,V);
      Buffer->TickList[count].Time=Time;
      Buffer->TickList[count].Consonant=c;
      Buffer->TickList[count].Vowel=v;
      return 0;
    }
  return 1;
}

int SconfProcessor::EditByTime(float Time,const char* C,const char* V)
{
  int count=ReadCountByTime(Time);
  if(count!=-1)
    {
      EditByCount(count,Time,C,V);
      return count;
    }
  return -1;
}

void SconfProcessor::SetPath(const char* topath)
{
  String_SetChars(path,topath);
}

void SconfProcessor::DeleteByCount(const int count)
{
  TickList_Type_Dtor(&(Buffer->TickList[count]));
  ArrayType_Remove(TickList_Type, Buffer->TickList, count);
}

void SconfProcessor::DeleteByTime(float Time)
{
  int co=ReadCountByTime(Time);
  if(co!=-1)
  {
    DeleteByCount(co);
  }
  else
  {
      std::cout<<"Error on [SconfProcessor]:"<<std::endl;
      std::cout<<"  "<<"When [DeleteByTime],sconf is "<<String_GetChars(path)<<":"<<std::endl;
      std::cout<<"  "<<"No such time:Time="<<Time<<"."<<std::endl;
      std::cout<<"Do:[nothing]."<<std::endl;
  }
}

void SconfProcessor::EditTimeByCount(int count,float Time)
{
    if((count>=0) & (count<=this->GetIndex()))
    {
        this->Buffer->TickList[count].Time=Time;
    }
}

void SconfProcessor::EditCByCount(int count,const char *C)
{
    if((count>=0) & (count<=this->GetIndex()))
    {
        String_SetChars(&this->Buffer->TickList[count].Consonant,C);
    }
}

void SconfProcessor::EditVByCount(int count,const char *V)
{
    if((count>=0) & (count<=this->GetIndex()))
    {
        String_SetChars(&this->Buffer->TickList[count].Vowel,V);
    }
}

int SconfProcessor::PushNew()
{
    ArrayType_PushNull(TickList_Type,this->Buffer->TickList);
    TickList_Type_Ctor(&(this->Buffer->TickList[this->Buffer->TickList_Index]));
    return this->Buffer->TickList_Index;
}
