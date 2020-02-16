#ifndef M2RCONVERTER_H
#define M2RCONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <TTree.h>
#include <TFile.h>
#include <Bytes.h>
#include <Byteswap.h>

#include <M2RReader.h>
#include <M2RMidasRootEvent.h>
#include <M2RRunInfo.h>
#include <shared.h>

class M2RConverter
{
public :
  
  M2RConverter();
  ~M2RConverter();
  
  void RegisterReader(M2RReader *);
  int InitializeConverter();
  
  const char * GetOutputFileName() const;
  
  void Process();
  void EndProcess();
  
private :
  TFile * fTheFile;
  TTree * fTheTree;
  M2RMidasRootEvent * fevt;
  M2RReader * fTheReader;
  std::string fOutputFileName;
  
  void FillEvent();
  
};

#endif
