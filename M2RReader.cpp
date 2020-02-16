#include <M2RReader.h>

//____________________________________________________
M2RReader::M2RReader() :
fBlockLength(gRun->GetBlockSize()*1024),
fNumCharRead(0),
fNumCharUsefulRead(0),
fNumBlocksRead(0),
fCurrEvent(0)
{}
  
//____________________________________________________
int M2RReader::Open(const char * file_name)
{
  //
  //Opening file
  fInputFile.open(file_name,std::ios::binary);
  //
  if(fInputFile.is_open()==0) {
    //
    //Opening failed
    return 0;
    //
  }
  //
  
  //
  //Buffer all the file content in a useful data structure
  while(!fInputFile.eof()) {
    //Reading a block
    unsigned char * ABlock = (unsigned char*) ReadBlock();
    //
        
    //Retrieving the block header
    DATA_HEADER TheEventHeader;
    //  
    RetrieveHeader((void *)ABlock, TheEventHeader);
    //
        
    //
    //Check if the block has valid data
    if(TheEventHeader.header_dataLen==0) continue;
    //
    
    //
    //Pass the header and move to the beginning of the useful data
    ABlock+=BLOCK_BEGINNER_SIZE_CHAR+BLOCK_HEADER_SIZE_CHAR;
    //
        
    //
    //Separating block into events
    for(unsigned long EventLength=0, DataRead=0; DataRead<TheEventHeader.header_dataLen; DataRead+=EventLength) {
      ABlock+=EventLength; //Moving to the next event
            
      EventLength = Rbswap_16(((short*)ABlock)[0]); //Reading the length of the event from the first word of the event. We need to swap the 2 char.
      if(EventLength==0) break; //If the event length is zero means that I reached the end of the block and the block is complemented by a series of termination words "5E 5E 5E 5E"
      
      fTheEvents.push_back((short*)ABlock); //Saving the current event on the buffer
    }
    //
  }
  //
    
  //
  //File successfully opened
  return 1;
  //
}

//____________________________________________________
short * M2RReader::NextEvent()
{  
  return fCurrEvent<fTheEvents.size() ? fTheEvents[fCurrEvent++] : 0;  
}

//____________________________________________________
void * M2RReader::ReadBlock()
{ 
  //
  unsigned char * TheBuffer = new unsigned char[fBlockLength];
  memset((char*)TheBuffer,0,fBlockLength); //Setting all buffer to zero
  //
  //Buffering the whole block in memory
  fInputFile.read((char *)TheBuffer,fBlockLength); //Read a quantity of chars equal to the length of a block
  fNumCharRead+=fInputFile.gcount(); //Counting number of char (bytes) read in the last reading instruction
    
  //
  // Check if block is corrupted (i.e. it doesn't start with the beginning string) NOTE: This is temporary!
  if(memcmp("EBYEDATA", (char *)TheBuffer, BLOCK_BEGINNER_SIZE_CHAR) != 0 && fInputFile.gcount()==fBlockLength) {
    printf("Hey, file is probably corrupted as I can't identify the beginning of the next block. Aborting!\n");
    exit (100); //I stop analyzing the file
  }
  //
  
  //I found the character indicating the beginning of the block. I return the pointer to the block.
  return (void *)TheBuffer;
}

//____________________________________________________
void M2RReader::RetrieveHeader(void * ABlock, DATA_HEADER & AnHeader)
{ 
  memcpy((char*)AnHeader.header_id,(char*)ABlock,BLOCK_BEGINNER_SIZE_CHAR);  
  AnHeader.header_sequence=((short*)ABlock)[4]+((short*)ABlock)[5];
  AnHeader.header_stream=((short*)ABlock)[6];
  AnHeader.header_tape=((short*)ABlock)[7];
  AnHeader.header_MyEndian=((short*)ABlock)[8];
  AnHeader.header_DataEndian=((short*)ABlock)[9];
  AnHeader.header_dataLen=((short*)ABlock)[10]+((short*)ABlock)[11];
    
  return;
}
