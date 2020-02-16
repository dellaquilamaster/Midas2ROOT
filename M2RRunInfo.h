#ifndef M2RRUNINFO_H
#define M2RRUNINFO_H

#include <fstream>
#include <sstream>
#include <TROOT.h>

class M2RRunInfo
{
public:
  M2RRunInfo(const char * run_number);                   //! Constructor
  ~M2RRunInfo();                                         //! Destructor
  
  int InitClass(const char *);                           //! Class initializer called by the main program
  
  int LoadSetupConfiguration(const char *);              //! Read general setup lines from configuration file of the experiment
  int LoadRunConfiguration(const char *, int);           //! Read run configuration for a specific run

  const char * GetName() const;                          //! Get Experiment Name
  const char * GetTitle() const;                         //! Get Experiment Title
  int GetBlockSize() const;                              //! Size of a block
  const char * GetRunFileName() const;                   //! Get the file name of a certain run
  const char * GetRunFilePathName() const;               //! Get the file absolute path name of a certain run
  const char * GetRunFileNamePrefix() const;             //! Get the prefix of run file names
  int GetRunNumber() const;                              //! Returns run number
  int GetEvtFileNumber() const;                          //! Returns the number of the evt file
  const char * GetBinaryFilePath() const;                //! Returns path containing binary files for the run
  const char * GetM2RROOTFilePath() const;               //! Returns the path to the unpacked files of the run (output of the program)

  void SetBinaryFilePath(const char *);                  //! Set path for the binary files of the run

private:
  std::string fRunName;                                  //!
  int fRunNumber;                                        //!
  int fEvtFileNumber;                                    //!
  std::string fExperimentName;                           //!
  std::string fExperimentTitle;                          //!
  int fBlockSize;                                        //!
  std::string fM2RRootFilePath;                          //!
  std::string fBinaryFilePath;                           //!
  std::string fRunFileName;                              //!
  std::string fRunFileNamePrefix;                        //!
  
  void ParseSetConfigLine(const char *);                 //! Parse a "set" line in the configuration file
  void ParseSetConfigLineRunInfo(const char *, int );    //! Parse a "set" line of a run-by-run instruction
};

#endif
