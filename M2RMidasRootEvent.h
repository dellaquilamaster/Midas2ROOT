#ifndef M2RMIDASROOTEVENT_H
#define M2RMIDASROOTEVENT_H

#include <TROOT.h>

#include <shared.h>

class M2RMidasRootEvent
{
public :
  M2RMidasRootEvent();
  virtual ~M2RMidasRootEvent();
  
  Int_t fmulti;
  Short_t * fgid; //[fmulti]
  Short_t * fch; //[fmulti]
  Short_t * fampl; //[fmulti]
  
  ClassDef(M2RMidasRootEvent,1);
};

#endif
