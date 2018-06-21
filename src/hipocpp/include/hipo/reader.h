/*
 *This sowftware was developed at Jefferson National Laboratory.
 */
 /**
  * <pre>
  *
  * FILE HEADER STRUCTURE ( 56 bytes, 14 integers (32 bit) )
  *
  *    +----------------------------------+
  *  1 |              ID                  | // HIPO: 0x43455248, Evio: 0x4556494F
  *    +----------------------------------+
  *  2 +          File Number             | // split file #
  *    +----------------------------------+
  *  3 +         Header Length            | // 14 (words)
  *    +----------------------------------+
  *  4 +      Record (Index) Count        |
  *    +----------------------------------+
  *  5 +      Index Array Length          | // bytes
  *    +-----------------------+----------+
  *  6 +       Bit Info        | Version  | // version (8 bits)
  *    +-----------------------+----------+
  *  7 +      User Header Length          | // bytes
  *    +----------------------------------+
  *  8 +          Magic Number            | // 0xc0da0100
  *    +----------------------------------+
  *  9 +          User Register           |
  *    +--                              --+
  * 10 +                                  |
  *    +----------------------------------+
  * 11 +         Trailer Position         | // File offset to trailer head (64 bits).
  *    +--                              --+ // 0 = no offset available or no trailer exists.
  * 12 +                                  |
  *    +----------------------------------+
  * 13 +          User Integer 1          |
  *    +----------------------------------+
  * 14 +          User Integer 2          |
  *    +----------------------------------+
  *
  * -------------------
  *   Bit Info Word
  * -------------------
  *     0-7  = version
  *     8    = true if dictionary is included (relevant for first record only)
  *     9    = true if this file has "first" event (in every split file)
  *    10    = File trailer with index array exists
  *    11-19 = reserved
  *    20-21 = pad 1
  *    22-23 = pad 2
  *    24-25 = pad 3 (always 0)
  *    26-27 = reserved
  *    28-31 = general header type: 1 = Evio file
  *                                 2 = Evio extended file
  *                                 5 = HIPO file
  *                                 6 = HIPO extended file
  *
  * </pre>
  *
  * @version 6.0
  * @since 6.0 9/6/17
  */
/*
 * File:   hipofile.h
 * Author: gavalian
 *
 * Created on April 11, 2017, 2:07 PM
 */

#ifndef HIPOFILE_H
#define HIPOFILE_H


#define HIPO_FILE_HEADER_SIZE 72
/* Constants for endianness of the file */
#ifndef BIG_ENDIAN
#define BIG_ENDIAN     0
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN  1
#endif

#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <climits>
#include "record.h"
#include "utils.h"
#include "dictionary.h"

namespace hipo {

  typedef struct {
      long  recordPosition;
      int   recordLength;
      int   recordEvents;
      int   recordDataOffset;
      int   recordDataLengthCompressed;
      int   recordDataLengthUncompressed;
      int   startEvent;
      int   endEvent;
  } recordIndex_t;

  typedef struct {
    int  uniqueid;
    int  filenumber;
    int  headerLength; // in words (usually 14)
    int  recordCount;
    int  indexArrayLength; // in bytes
    int  bitInfo;
    int  version;
    int  userHeaderLength;
    int  magicNumber;
    long userRegister;
    long trailerPosition;
    long firstRecordPosition;
  } fileHeader_t;
  /**
  * READER index class is used to construct entire events
  * sequence from all records, and provides ability to canAdvance
  * through events where record number is automatically calculated
  * and triggers reading of the next record when events in the current
  * record are exhausted.
  */
  class reader_index {
     private:
       std::vector<int> recordEvents;
       int              currentRecord;
       int              currentEvent;
       int              currentRecordEvent;

     public:
        reader_index(){

        };
        ~reader_index(){};

        //bool canAdvance();
        bool advance();

        int  getEventNumber() { return currentEvent;}
        int  getRecordNumber() { return currentRecord;}
        int  getRecordEventNumber() { return currentRecordEvent;}
        int  getMaxEvents();
        void addSize(int size);

        void reset(){
          currentRecord = 0;
          currentEvent  = 0;
          currentRecordEvent = 0;
        }
  };
 /**
  * reader sequence class is used for sequancial readin of the file. When each
  * record is read, the next record pointer is kept in this class to be read
  * when all the events in the current record are exhausted.
  */
  class reader_sequence {
    private:

       long position;
       long length;
       long nextPosition;
       int  currentEvent;
       int  recordEvents;

    public:

      reader_sequence(){}
      virtual ~reader_sequence(){}

      void setPosition(long pos){position = pos;}
      void setLength(long rl){length = rl;}
      void setNextPosition(long np){ nextPosition = np;}
      void setCurrentEvent(int ce){ currentEvent = ce;}
      void setRecordEvents(int re){recordEvents = re;}

      int   getRecordEvents(){return recordEvents;}
      long  getPosition(){ return position;}
      long  getNextPosition() { return nextPosition;}
      bool  hasEvents(){ return (currentEvent<recordEvents);}
      int   getCurrentEvent(){ return currentEvent;}
  };

  class reader {

    private:

        std::vector<std::string>        fileDictionary;

        hipo::dictionary                schemaDictionary;

        std::vector<char>               headerBuffer;
        std::ifstream                   inputStream;
        std::vector<recordIndex_t>      recordIndex;
        fileHeader_t                    header;
        hipo::utils                     hipoutils;
        /**
        * Internal buffers for record and events to be
        * read in sequence. When the next() is called on The
        * reader class;
        */
        hipo::record                    inRecordStream;
        hipo::event                     inEventStream;
        hipo::reader_index              inReaderIndex;
        int                             inReaderCurrentRecord;
        reader_sequence                 sequence;

        long    inputStreamSize;
        bool    isRandomAccess;
        bool    verifyFile();
        void    readHeader();
        void    readRecordIndex();
        void    readDictionary();

    public:

        reader();
        reader(bool ra);
        ~reader();

        std::vector<std::string>  getDictionary();

        hipo::dictionary         *getSchemaDictionary();

        void  open(const char *filename);
        void  readRecord(int index);
        void  readRecord(hipo::record &record, int index);
        void  readHeaderRecord(hipo::record &record);
        int   getRecordCount();
        bool  isOpen();
        void  showInfo();
        void  printWarning();
        bool  next();
        hipo::event    *getEvent(){return &inEventStream;}
        template<class T> hipo::node<T> *getBranch(int group, int item);
        template<class T> hipo::node<T> *getBranch(const char* group, const char* item);
    };

}

namespace hipo {
  template<class T> hipo::node<T> *reader::getBranch(int group, int item){
      return inEventStream.getBranch<T>(group,item);
  }
  template<class T> hipo::node<T> *reader::getBranch(const char* group, const char* item){
      if(schemaDictionary.hasSchema(group)==true){
        hipo::schema schema = schemaDictionary.getSchema(group);
        if(schema.hasEntry(item)==true){
          int group_id = schema.getGroup();
          int item__id = schema.getItem(item);
          return inEventStream.getBranch<T>(group_id,item__id);
        }
      }
      return NULL;
  }
}
#endif /* HIPOFILE_H */
