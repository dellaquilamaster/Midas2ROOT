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
  //TEMPORARY: Folder Definitions
  std::string fOutputFolder("/run/media/daniele/WD_IRB/analysis/2016_INFN_LNL_Ne_He/unpacked/");
  //
  
  //
  //Reading Input
  std::string Source;
  if(ReadInput(argc,argv,Source)<=0) {
    printf("Error: missing valid source file!\nPlease use --run=xxx command to specify an input file.\nAborting!\n"); 
    exit(1);
  }
  //
  
  //
  //Creating Midas Reader
  M2RReader * TheReader = new M2RReader();
  //
  
  //
  //Opening Input File
  if(TheReader->Open(Source.c_str())==0) {
    printf("Error: failed to open source file %s\nAborting!\n", Source.c_str()); 
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
  std::string fFileOutName(Form("%s%s.root",fOutputFolder.c_str(),Source.substr(Source.find_last_of('/')+1).c_str()));  
  if (TheRootConverter->InitTree(fFileOutName.c_str())!=0) {
    printf("Error: failed to create output file in folder %s.\nAborting!", fOutputFolder.c_str());
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
 
  printf("\nSuccessfully created file: %s\n\n", fFileOutName.c_str());
  return 0;
}
