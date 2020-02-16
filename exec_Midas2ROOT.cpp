/*
 * Midas2ROOT program
 * created by Daniele Dell'Aquila (daniele.dellaquila@irb.hr)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <M2RConverter.h>
#include <M2RReader.h>
#include <M2RRunInfo.h>
#include <shared.h>

int ReadInput(int argc, char ** argv, std::string & Source)
{
  int Nread=0;
  
  for(int i=1; i<argc; i++) {
    std::string AnImput(argv[i]);
    if(AnImput.find("--run=")!=std::string::npos) {
      std::string TheValue(AnImput.substr(AnImput.find("=")+1));
      Source.assign(TheValue);
      Nread++;
    }
  }
  
  return Nread;
}

int main (int argc, char ** argv)
{
  //
  //Reading Input
  std::string Source;
  if(ReadInput(argc,argv,Source)<=0) {
    printf("Error: missing valid source file!\nPlease use --run=xxx command to specify a run number.\nAborting!\n"); 
    exit(1);
  }
  //
    
  //
  //Initializing run info class
  gRun=new M2RRunInfo(Source.c_str());
  if(gRun->InitClass("config/Midas2ROOT.conf")<=0)  {
    printf("Error while intializing RunInfo class for run %s.\nAborting!",Source.c_str());
    exit(2);
  }
  //
  
  //
  //Creating Midas Reader
  M2RReader * TheReader = new M2RReader();
  //
  
  //
  //Opening Input File
  if(TheReader->Open(gRun->GetRunFilePathName())==0) {
    printf("Error: failed to open source file %s\nAborting!\n", gRun->GetRunFilePathName()); 
    exit(2);
  }
  //
  
  //
  //Creating Root Converter
  M2RConverter * TheRootConverter = new M2RConverter();
  //
  
  //
  //Registering Reader
  TheRootConverter->RegisterReader(TheReader);
  //
  
  //
  //Initializing Output Tree
  if (TheRootConverter->InitializeConverter()!=0) {
    printf("Error: failed to create output file in folder %s.\nAborting!", gRun->GetM2RROOTFilePath());
    exit(3);
  }
  //
  
  //
  //Midas to ROOT conversion
  TheRootConverter->Process();
  //
  
  //
  //Closing output file
  TheRootConverter->EndProcess();
  //
 
  printf("\nSuccessfully created file: %s\n\n", TheRootConverter->GetOutputFileName());
  return 0;
}
