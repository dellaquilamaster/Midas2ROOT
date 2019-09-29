#include <M2RConverter.h>

//____________________________________________________
M2RConverter::M2RConverter() :
fTheFile(0),
fTheTree(0),
fevt(new M2RMidasRootEvent()),
fTheReader(0)
{}

//____________________________________________________
M2RConverter::~M2RConverter()
{
  delete fevt;
  delete fTheTree;
  delete fTheFile;
}

//____________________________________________________
int M2RConverter::InitTree(const char * file_name)
{
  //
  //Opening output ROOT file
  fTheFile = new TFile(file_name, "RECREATE");
  //
  if(fTheFile->IsZombie()) {
    return -1; 
  }
  //
  
  //
  //Creating output tree
  fTheTree = new TTree("Midas","Midas2ROOT Output Tree");
  //Initializing Branch
  fTheTree->Branch("DAQ.", "M2RMidasRootEvent", fevt);
  //
  
  //
  fTheTree->SetAutoSave(500000);
  //
  
  return 0;
}

//____________________________________________________
void M2RConverter::RegisterReader(M2RReader * NewReader)
{
  fTheReader = NewReader;  
}

//____________________________________________________
void M2RConverter::Process()
{ 
  //
  //Reading event-by-event the Midas file
  short * AnEvent;
  while ((AnEvent=fTheReader->NextEvent())) {
    unsigned long evt_size = Rbswap_16(AnEvent[0])-2; //The last 2 char are FF FF and therefore I exclude them.
        
    //
    //Initializing multiplicity of the event
    fevt->fmulti=0;
    //
    
    //
    for(int i=2; i<evt_size; i+=4) {
      //I go ahead 2-words by 2-words. Each group of 2 words (4 chars) is an individual entry of the event
      /*
       * A midas event is constituted by a succession of 2 word-groups:
       * 1 word = 1 char -> ch, 1 char -> gid
       * 1 word = amplitude (need byte swap)
       *  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _      _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
       * |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
       *       ch              gid                        amplitude
       */
      unsigned char ch_num = ((char *)AnEvent)[i]; //Number of channel fired in the ADC
      unsigned char gid_num = ((char *)AnEvent)[i+1]; //Location of the ADC
      unsigned short amplitude; //Amplitude of the signal
      memcpy(&amplitude, ((char *)AnEvent)+i+2, 2);
      amplitude=Rbswap_16(amplitude); //We need to swap the bytes of the amplitude
      
      fevt->fgid[fevt->fmulti]=gid_num;
      fevt->fch[fevt->fmulti]=ch_num;
      fevt->fampl[fevt->fmulti]=amplitude;
      fevt->fmulti++;
    }
    //
    
    //
    //Filling the entry of the current event
    FillEvent();
    //
  }
  //
}

//____________________________________________________
void M2RConverter::FillEvent()
{
  fTheTree->Fill(); 
}

//____________________________________________________
void M2RConverter::EndProcess()
{
  //Writing TTree to TFile
  fTheTree->AutoSave();
  //
  
  //
  //Closing output file
  fTheFile->Close();
  //
}
