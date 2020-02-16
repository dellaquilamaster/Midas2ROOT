#ifndef M2RREADER_H
#define M2RREADER_H

#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <cstring>
#include <vector>
#include <Bytes.h>
#include <Byteswap.h>

#include <M2RRunInfo.h>
#include <shared.h>

/* Midas buffer reader
 * See more about the Midas DAQ here: http://npg.dl.ac.uk/MIDAS/
 * 
 * Data is organized in blocks, each of those has an 24-char (bytes) header containing:
 *   - 4 words = "EBYEDATA"
 *   - 2 words = header number within the file
 *   - 1 word  = --
 *   - 1 word  = --
 *   - 1 word  = --
 *   - 1 word  = --
 *   - 2 words = length of the block in char (bytes)
 *   - 1 word  = 0xffff
 * TOTAL: 13 words (26 chars)
 */

/*
 * A midas event is constituted by a succession of 2 word-groups:
 * 1 word = 1 char -> ch, 1 char -> gid
 * 1 word = amplitude (need byte swap)
 *  _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _      _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ 
 * |_ _ _ _ _ _ _ _|_ _ _ _ _ _ _ _|    |_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _|
 *       ch              gid                        amplitude
 * 
 * NOTE: The reader is now written for a 16-bit acquisition exclusively.
 * In a future improvement we will introduce the possibility to choose 
 * if to run on a 16, 24, or 32 bit system.
 */

//    24 byte data block header
typedef struct s_data_header {
    unsigned char   header_id[8];              //   contains the string  EBYEDATA
    unsigned long   header_sequence;           //   within the file
    unsigned short  header_stream;             //   data acquisition stream number (in the range 1=>4)
    unsigned short  header_tape;               //   =1
    unsigned short  header_MyEndian;           //   written as a native 1 by the tape server
    unsigned short  header_DataEndian;         //   written as a native 1 in the hardware structure of the data following
    unsigned long   header_dataLen;            //   total length of useful data following the header in bytes
} DATA_HEADER;
//   **************************

#define BLOCK_HEADER_SIZE_CHAR 18 //Size of the Header in char (9 words including end deliminter and excluding 4 words beginner delimiter)
#define BLOCK_BEGINNER_SIZE_CHAR 8 //Size of the block beginning identifier in char (4 words)

class M2RReader
{
public :
  M2RReader();
  
  int Open(const char *); //Open input file and read all the content in memory
  
  short * NextEvent(); //Returns the next event from the buffer
  
private :
  std::ifstream fInputFile;
  const int fBlockLength; //Length of a block. 16-bit words -> 16384; 24-bit words -> 24576; 32-bit words -> 32768; NOTE: For now we are working on a 16-bit acquisition.
  int fNumCharRead;
  int fNumCharUsefulRead;
  int fNumBlocksRead;
  
  std::vector<short *> fTheEvents;
  int fCurrEvent;
  
  void * ReadBlock();
  void RetrieveHeader(void *, DATA_HEADER &);
  
};

#endif
