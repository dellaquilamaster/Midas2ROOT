#include <M2RRunInfo.h>

//________________________________________________
M2RRunInfo::M2RRunInfo(const char * run_number) :
fRunName(run_number),
fRunNumber(std::stoi(fRunName.substr(0,fRunName.find_first_of("_")))),
fEvtFileNumber(std::stoi(fRunName.substr(fRunName.find_first_of("_")+1)))
{}

//________________________________________________
M2RRunInfo::~M2RRunInfo()
{}

//________________________________________________
int M2RRunInfo::InitClass(const char *file_name)
{
  int NLinesRead=0;

  //Set general setup configuration
  NLinesRead += LoadSetupConfiguration(file_name);
  
  //Load run-by-run information
  NLinesRead += LoadRunConfiguration(file_name,fRunNumber);

  return NLinesRead;
}

//________________________________________________
int M2RRunInfo::LoadSetupConfiguration(const char *file_name)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    return -1;
  }

  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);
    std::string LineReadCommentLess(LineRead.substr(0,LineRead.find("*")));

    if(LineReadCommentLess.empty()) continue;

    if(LineReadCommentLess.find_first_not_of(' ') == std::string::npos) continue;

    if(LineReadCommentLess.find("set ")!=std::string::npos) {
      ParseSetConfigLine(LineReadCommentLess.c_str());
    }

    NRead++;
  }
  FileIn.close();

  return NRead;
}

//________________________________________________
int M2RRunInfo::LoadRunConfiguration(const char *file_name, int run_num)
{
  std::ifstream FileIn(file_name);
  if(!FileIn.is_open()) {
    return -1;
  }

  int NRead=0;

  while (!FileIn.eof())
  {
    std::string LineRead;
    std::getline(FileIn, LineRead);
    std::string LineReadCommentLess(LineRead.substr(0,LineRead.find("*")));

    if(LineReadCommentLess.empty()) continue;

    if(LineReadCommentLess.find_first_not_of(' ') == std::string::npos) continue;

    if(LineReadCommentLess.find("set ")!=std::string::npos) {
      ParseSetConfigLineRunInfo(LineReadCommentLess.c_str(), run_num);
    }

    NRead++;
  }
  FileIn.close();

  return NRead;
}

const char * M2RRunInfo::GetName() const
{
  return fExperimentName.c_str();
}

const char * M2RRunInfo::GetTitle() const
{
   return fExperimentTitle.c_str(); 
}

int M2RRunInfo::GetBlockSize() const
{
  return fBlockSize;  
} 

int M2RRunInfo::GetRunNumber() const
{
  return fRunNumber;  
} 

int M2RRunInfo::GetEvtFileNumber() const
{
  return fEvtFileNumber;  
} 

const char * M2RRunInfo::GetRunFileName() const
{
  return fRunFileName.c_str();  
} 

const char * M2RRunInfo::GetRunFilePathName() const
{
  return Form("%s%s",fBinaryFilePath.c_str(), fRunFileName.c_str());  
} 

const char * M2RRunInfo::GetRunFileNamePrefix() const
{
  return fRunFileNamePrefix.c_str();  
} 

const char * M2RRunInfo::GetBinaryFilePath() const
{
  return fBinaryFilePath.c_str();  
} 

const char * M2RRunInfo::GetM2RROOTFilePath() const
{
  return fM2RRootFilePath.c_str();  
}          

//________________________________________________
void M2RRunInfo::ParseSetConfigLine(const char *line_to_parse)
{
  std::string LineToParse(line_to_parse);

  LineToParse.assign(LineToParse.substr(LineToParse.find("set ")+4)); //remove set command

  std::istringstream LineStream(LineToParse);

  std::string ValueToSet;
  LineStream>>ValueToSet;
  std::string NewValue(LineToParse.substr(LineToParse.find("\"")+1,LineToParse.find_last_of("\"")-(LineToParse.find("\"")+1)));

  if(ValueToSet.compare("EXPERIMENT_NAME")==0) {
    fExperimentName.assign(NewValue);
  } else if (ValueToSet.compare("EXPERIMENT_TITLE")==0) {
    fExperimentTitle.assign(NewValue);
  } else if (ValueToSet.compare("MIDAS_BLOCK_SIZE")==0) {
    fBlockSize=std::stoi(NewValue);
  } else if (ValueToSet.compare("RUN_FILENAME_PREFIX")==0) {
    fRunFileNamePrefix.assign(NewValue);
    fRunFileName.assign(Form("%s%s",GetRunFileNamePrefix(),fRunName.c_str()));
  } else if (ValueToSet.compare("BINARY_FILE_PATH")==0) {
    if(LineToParse.find("--run")==std::string::npos) { //EvtFileName might be different run-by-run, if --run is specified don't set it now but in ParseSetConfigLineRunInfo
      fBinaryFilePath.assign(NewValue);
    }
  } else if (ValueToSet.compare("M2R_ROOT_FILE_PATH")==0) {
    fM2RRootFilePath.assign(NewValue);
  }

  return;
}

//________________________________________________
void M2RRunInfo::ParseSetConfigLineRunInfo(const char *line_to_parse, int run_num)
{
  std::string LineToParse(line_to_parse);

  LineToParse.assign(LineToParse.substr(LineToParse.find("set ")+4)); //remove set command

  std::istringstream LineStream(LineToParse);

  std::string ValueToSet;
  LineStream>>ValueToSet;
  std::string NewValue;
  std::string Option;
  bool RunFound=false;

  //NOTE: for a future improvement take into account more possible options not only --run and --exclude
  //A --run or --exclude option can contain or "," or "-" as a separator, not combination of both
  // Loop on the option strings, every time one finds -- this is an option string, e.g. --run, --exclude
  // 2018/08/20 Included the possibility of specifying only 1 run with --run or --exclude options
  while(LineStream>>Option && Option.find("--")!=std::string::npos) {
    if(Option.find("--run=")!=std::string::npos) {
      Option.assign(Option.substr(Option.find("--run=")+6));
      std::istringstream LineRunStream(Option);
      if(Option.find(",")!=std::string::npos) {
        std::string RunToInclude;
        while(std::getline(LineRunStream, RunToInclude, ',')) {
          if(run_num==std::stoi(RunToInclude)) RunFound=true;
        }
      }
      if(Option.find("-")!=std::string::npos) {
        std::string StartRun;
        std::string StopRun;
        std::getline(LineRunStream, StartRun, '-');
        std::getline(LineRunStream, StopRun, '-');
        int StartRunNum=std::stoi(StartRun);
        int StopRunNum=std::stoi(StopRun);
        if(run_num>=StartRunNum && run_num<=StopRunNum) RunFound=true;
      }
      if(Option.find(",")==std::string::npos && Option.find("-")==std::string::npos && !Option.empty()) {
        if(run_num==std::stoi(Option)) RunFound=true;
      }
    } else if (Option.find("--exclude=")!=std::string::npos) {
        Option.assign(Option.substr(Option.find("--exclude=")+10));
        std::istringstream LineExcludeStream(Option);
        if(Option.find(",")!=std::string::npos) {
          std::string RunToExclude;
          while(std::getline(LineExcludeStream, RunToExclude, ',')) {
            if(run_num==std::stoi(RunToExclude)) return; //this run is excluded
          }
        }
        if(Option.find("-")!=std::string::npos) {
          std::string StartRun;
          std::string StopRun;
          std::getline(LineExcludeStream, StartRun, '-');
          std::getline(LineExcludeStream, StopRun, '-');
          int StartRunNum=std::stoi(StartRun);
          int StopRunNum=std::stoi(StopRun);
          if(run_num>=StartRunNum && run_num<=StopRunNum) return; //this run belong to a bunch of runs to be excluded
        }
        if(Option.find(",")==std::string::npos && Option.find("-")==std::string::npos && !Option.empty()) {
          if(run_num==std::stoi(Option)) return;
        }
      }
  }

  if(RunFound) {
    NewValue.assign(LineToParse.substr(LineToParse.find("\"")+1,LineToParse.find_last_of("\"")-(LineToParse.find("\"")+1)));
  } else return;

  // if I'm here so run_num has been found in the configuration line
  if(ValueToSet.compare("MBINARY_FILE_PATH")==0) {
    fBinaryFilePath.assign(NewValue);
  }

  return;
}
