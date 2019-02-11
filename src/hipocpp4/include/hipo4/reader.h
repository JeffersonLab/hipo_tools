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
#define BIG_ENDIAN 0
#endif
#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN 1
#endif

#include "record.h"
#include "utils.h"
#include <climits>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

namespace hipo {

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
  class readerIndex {
  private:
    std::vector<int>  recordEvents;
    std::vector<long> recordPosition;

    int currentRecord;
    int currentEvent;
    int currentRecordEvent;

  public:
    readerIndex(){

    };
    ~readerIndex(){};

    bool canAdvance();
    bool advance();

    int  getEventNumber() { return currentEvent; }
    int  getRecordNumber() { return currentRecord; }
    int  getRecordEventNumber() { return currentRecordEvent; }
    int  getMaxEvents();
    void addSize(int size);
    void addPosition(long position) { recordPosition.push_back(position); }
    long getPosition(int index) { return recordPosition[index]; }
    void rewind() {
      currentRecord      = -1;
      currentEvent       = -1;
      currentRecordEvent = -1;
    }
    void reset() {
      currentRecord      = 0;
      currentEvent       = 0;
      currentRecordEvent = 0;
    }
  };

  class reader {
  private:
    fileHeader_t  header;
    hipo::utils   hipoutils;
    std::ifstream inputStream;
    long          inputStreamSize;

    hipo::record      inputRecord;
    hipo::readerIndex readerEventIndex;

    void readHeader();
    void readIndex();

  public:
    reader();
    reader(std::string filename) { open(filename.c_str()); }
    reader(const char* filename) { open(filename); }
    ~reader();

    void              readDictionary(hipo::dictionary& dict);
    hipo::dictionary* dictionary();
    void              open(const char* filename);
    void              open(std::string filename) { open(filename.c_str()); };
    bool              hasNext();
    bool              next();
    long              numEvents() { return readerEventIndex.getMaxEvents(); }
    bool              next(hipo::event& dataevent);
    void              read(hipo::event& dataevent);
    void              printWarning();
  };
} // namespace hipo
#endif /* HIPOFILE_H */
