#ifndef M2RCONVERTER_H
#define M2RCONVERTER_H

#include <stdio.h>
#include <stdlib.h>
#include <TTree.h>
#include <TFile.h>
#include <Bytes.h>
#include <Byteswap.h>

#include <M2RReader.h>
#include <M2RMidasRootEvent.h>

class M2RConverter
{
public :
  
  M2RConverter();
  ~M2RConverter();
  
  int InitTree(const char *);
  void RegisterReader(M2RReader *);
  
  void Process();
  void EndProcess();
  
private :
  TFile * fTheFile;
  TTree * fTheTree;
  M2RMidasRootEvent * fevt;
  M2RReader * fTheReader;
  
  void FillEvent();
  
};

#endif
