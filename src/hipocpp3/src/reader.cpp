/*
 * This sowftware was developed at Jefferson National Laboratory.
 * (c) 2017.
 */

#include "hipo3/hipoexceptions.h"
#include "hipo3/reader.h"
#include "hipo3/record.h"

#include <cstdlib>
/**
 * HIPO namespace is used for the classes that read write
 * files and records.
 */
namespace hipo {
  /**
   * The constructor for reader, printWarning routine
   * will printout a warning message if the library
   * was not compiled with compression libraries LZ4 or GZIP
   */
  reader::reader() {
    printWarning();
    // hipoutils.printLogo();
    isRandomAccess = false;
  }

  reader::reader(std::string filename) {
    printWarning();
    // hipoutils.printLogo();
    isRandomAccess = false;
    open(filename.c_str());
  }
  /**
   * Default destructor. Does nothing
   */
  reader::~reader() {
    if (inputStream.is_open() == true) {
      inputStream.close();
    }
  }
  /**
   * Open file, if file stream is open, it is closed first.
   * At open time verification of file structure is performed.
   * If the signature does not match EVIO/HIPO template, the
   * file will be closed and warning message is printed.
   */
  void reader::open(const char* filename) {
    if (inputStream.is_open() == true) {
      inputStream.close();
    }

    inputStream.open(filename, std::ios::binary);
    inputStream.seekg(0, std::ios_base::end);
    inputStreamSize = inputStream.tellg();
    inputStream.seekg(0, std::ios_base::beg);
    if (inputStream.is_open() == false) {
      std::cerr << "[ERROR] something went wrong with openning file : " << filename << std::endl;
      return;
    }
    readHeader();
    bool status = verifyFile();
    if (status == false) {
      inputStream.close();
    }

    if (isRandomAccess == true) {
      readRecordIndex();
    } else {
      //--------------------------------------------------------
      // This part is for sequancial access of the file
      //--------------------------------------------------------
      long positionOffset = header.firstRecordPosition;
      inRecordStream.readRecord(inputStream, positionOffset, 0);
      int length = inRecordStream.getRecordSizeCompressed() * 4;
      sequence.setRecordEvents(inRecordStream.getEventCount());
      sequence.setPosition(positionOffset);
      sequence.setCurrentEvent(0);

      if ((positionOffset + length) >= (inputStreamSize - 56)) {
        sequence.setNextPosition(-1);
      } else {
        sequence.setNextPosition(positionOffset + length);
      }
      // printf(" read record = %12ld next %12ld  nevenets = %d length = %d\n",
      // sequence.getPosition(),
      //       sequence.getNextPosition(), sequence.getRecordEvents(), length);
    }

    readDictionary();
  }

  /**
   * Reads the file header. The endiannes is determined for bytes
   * swap. The header structure will be filled with file parameters.
   */
  void reader::readHeader() {
    headerBuffer.resize(80);
    inputStream.read(&headerBuffer[0], 80);
    header.uniqueid     = *(reinterpret_cast<int*>(&headerBuffer[0]));
    header.filenumber   = *(reinterpret_cast<int*>(&headerBuffer[4]));
    header.headerLength = *(reinterpret_cast<int*>(&headerBuffer[8]));
    header.recordCount  = *(reinterpret_cast<int*>(&headerBuffer[12]));

    header.indexArrayLength = *(reinterpret_cast<int*>(&headerBuffer[16]));
    int word_8              = *(reinterpret_cast<int*>(&headerBuffer[20]));

    header.userHeaderLength = *(reinterpret_cast<int*>(&headerBuffer[24]));
    header.magicNumber      = *(reinterpret_cast<int*>(&headerBuffer[28]));
    header.userRegister     = *(reinterpret_cast<long*>(&headerBuffer[32]));

    // If magic word is reversed, then the file was written in BIG_ENDIAN
    // format, the bytes have to be swapped
    if (header.magicNumber == 0x0001dac0) {
      printf(" THIS FILE IS BIG ENDIAN: SWAPPING BYTES\n");
      header.uniqueid         = __builtin_bswap32(header.uniqueid);
      header.filenumber       = __builtin_bswap32(header.filenumber);
      header.headerLength     = __builtin_bswap32(header.headerLength);
      header.recordCount      = __builtin_bswap32(header.recordCount);
      header.userHeaderLength = __builtin_bswap32(header.userHeaderLength);
      header.indexArrayLength = __builtin_bswap32(header.indexArrayLength);
      word_8                  = __builtin_bswap32(word_8);
      header.userRegister     = __builtin_bswap64(header.userRegister);
    }

    header.version             = word_8 & 0x000000FF;
    header.bitInfo             = (word_8 >> 8) & 0x00FFFFFF;
    header.firstRecordPosition = 4 * header.headerLength + header.userHeaderLength;
    // int *signature = reinterpret_cast<int *>(&headerBuffer[0]);
    // printf("signature = %X\n",(unsigned int) *signature);
    // std::cout << "signature = " << std::ios::hex << (*signature) << '\n';
  }
  /**
   * Verify if the file has a proper format, magic word is checked
   * to and endianness is determined. Byte swap is performed if neccessary.
   */
  bool reader::verifyFile() { return true; }
  /**
   * Checks to determine if the file is open.
   */
  bool reader::isOpen() { return inputStream.is_open(); }

  void reader::readDictionary() {
    hipo::record dictionary;
    hipo::event  schema;
    readHeaderRecord(dictionary);
    int nentries = dictionary.getEventCount();
    for (int d = 0; d < nentries; d++) {
      dictionary.readHipoEvent(schema, d);
      std::string schemaString = schema.getString(31111, 1);
      fileDictionary.push_back(schemaString);
      schemaDictionary.parse(schemaString);
    }
  }

  std::vector<std::string> reader::getDictionary() { return fileDictionary; }

  bool reader::next() {
    // printf("random access = %d\n", isRandomAccess);
    if (isRandomAccess == true) {
      if (inReaderCurrentRecord < 0) {
        inReaderCurrentRecord = 0;
        readRecord(inRecordStream, inReaderCurrentRecord);
        inRecordStream.readHipoEvent(inEventStream, 0);
        return true;
      }

      bool status = inReaderIndex.advance();
      if (status == false)
        return false;
      if (inReaderIndex.getRecordNumber() != inReaderCurrentRecord) {
        inReaderCurrentRecord = inReaderIndex.getRecordNumber();
        readRecord(inRecordStream, inReaderCurrentRecord);
      }
      inRecordStream.readHipoEvent(inEventStream, inReaderIndex.getRecordEventNumber());
    } else {
      // int current_event = sequence.getCurrentEvent();
      // printf("next() : current event %d has event %d\n",current_event,sequence.hasEvents());
      if (sequence.hasEvents() == false) {
        // printf(" READING NEXT BANCH \n");
        if (sequence.getNextPosition() < 0)
          return false;
        long positionOffset = sequence.getNextPosition();
        inRecordStream.readRecord(inputStream, positionOffset, 0);
        int length = inRecordStream.getRecordSizeCompressed() * 4;
        // printf(" READING DONE %d %d \n",length,inRecordStream.getEventCount());
        sequence.setRecordEvents(inRecordStream.getEventCount());
        sequence.setPosition(positionOffset);
        sequence.setCurrentEvent(0);

        if ((positionOffset + length) >= (inputStreamSize - 56)) {
          sequence.setNextPosition(-1);
        } else {
          sequence.setNextPosition(positionOffset + length);
        }
      }
      int current_event = sequence.getCurrentEvent();
      // printf("1\n");
      inRecordStream.readHipoEvent(inEventStream, current_event);
      // printf("2\n");
      sequence.setCurrentEvent(current_event + 1);
      return true;
    }
    return true;
  }
  /**
   * Reads records indicies, it hopes through file Reading
   * only header for each records and fills a vector with
   * record header descriptors, identifying the position,
   * and number of events in each record.
   * If it encounters mistake it will preserve all recovered
   * record information.
   */
  void reader::readRecordIndex() {
    inputStream.seekg(0, std::ios::end);
    long hipoFileSize   = inputStream.tellg();
    long positionOffset = header.firstRecordPosition;
    recordIndex.clear();
    inputStream.seekg(positionOffset, std::ios::beg);
    std::vector<char> recheader(80);
    int               icounter   = 0;
    int               eventCount = 0;
    inReaderIndex.reset();
    inReaderCurrentRecord = -1;

    while (positionOffset + 56 < hipoFileSize) {
      inputStream.read((char*)&recheader[0], 56);
      recordIndex_t recIndex;

      recIndex.recordPosition = positionOffset;
      recIndex.recordLength   = *(reinterpret_cast<int*>(&recheader[0]));
      recIndex.recordEvents   = *(reinterpret_cast<int*>(&recheader[12]));
      int compressWord        = *(reinterpret_cast<int*>(&recheader[36]));
      int version             = *(reinterpret_cast<int*>(&recheader[20]));
      int magic_number        = *(reinterpret_cast<int*>(&recheader[28]));

      recIndex.recordDataLengthCompressed = compressWord & 0x0FFFFFFF;
      // recIndex.compressionType            = (compressWord&0xF0000000)>>28;

      if (magic_number == 0x0001dac0) {
        recIndex.recordLength = __builtin_bswap32(recIndex.recordLength);
        recIndex.recordEvents = __builtin_bswap32(recIndex.recordEvents);
        compressWord          = __builtin_bswap32(compressWord);
        version               = __builtin_bswap32(version);
      }

      inReaderIndex.addSize(recIndex.recordEvents);

      version = (version & 0x000000FF);
      if (version != 6) {
        std::cerr << " version error : " << version << std::endl;
        break;
      }
      if (magic_number != 0xc0da0100 && magic_number != 0x0001dac0) {
        std::cerr << "magic number error : " << (unsigned int)magic_number << std::endl;
        break;
      }

      // printf("version = %d , magic number = %X\n",version,(unsigned int) magic_number);
      positionOffset += recIndex.recordLength * 4;
      inputStream.seekg(positionOffset, std::ios::beg);
      recordIndex.push_back(recIndex);
      // positionOffset +=
      icounter++;
      /*printf(" next position (%4d) : %16ld file size %ld events = %d\n",
        icounter,positionOffset,hipoFileSize, recIndex.recordEvents);*/
    }
    // printf("total records = %d  index array = %d\n", icounter, (unsigned int)recordIndex.size());
  }

  hipo::dictionary* reader::getSchemaDictionary() { return &schemaDictionary; }

  void reader::readHeaderRecord(hipo::record& record) {
    int offset  = header.headerLength * 4;
    int rlenght = header.userHeaderLength;
    record.readRecord(inputStream, offset, 0);
  }

  void reader::readRecord(hipo::record& record, int index) {
    long position = recordIndex[index].recordPosition;
    // printf("\n\n***** reading record %d with length
    // %d\n",index,recordIndex[index].recordLength*4);
    // record.readRecord__(inputStream,position,recordIndex[index].recordLength*4);
    record.readRecord(inputStream, position, 0);
  }
  void reader::readRecord(int index) {
    hipo::record rec;
    long         position = recordIndex[index].recordPosition;
    rec.readRecord(inputStream, position, 0);
  }
  /**
   * Print the file information on the screen.
   */
  void reader::showInfo() {
    printf("FILE: \n");
    printf(" %18s : %X\n", "uniqueid", (unsigned int)header.uniqueid);
    printf(" %18s : %d\n", "file #", header.filenumber);
    printf(" %18s : %d\n", "header length", header.headerLength);
    printf(" %18s : %d\n", "record count", header.recordCount);
    printf(" %18s : %d\n", "index length", header.indexArrayLength);
    printf(" %18s : %d\n", "version", header.version);
    printf(" %18s : %X\n", "bit info", (unsigned int)header.bitInfo);
    printf(" %18s : %d\n", "user header", header.userHeaderLength);
    printf(" %18s : %X\n", "magic number", (unsigned int)header.magicNumber);
    printf(" %18s : %ld\n", "first record", header.firstRecordPosition);
    if (recordIndex.size() < 1) {
      printf(" there are no records in the file : %d\n", inputStream.is_open());
      return;
    }
    long  recordPosition = 1000; // recordIndex[recordIndex.size()-1].recordPosition;
    float sizePos        = recordPosition / 1024.0 / 1024.0;
    printf("-------------------------------------\n");
    // printf("     signature : %X\n", (unsigned int) getSignature());
    // printf(" header Length : %d bytes\n", (unsigned int) getHeaderLength());
    printf("   file Length : %.2f MB\n", sizePos);
    printf("-------------------------------------\n");
  }

  int reader::getRecordCount() { return recordIndex.size(); }
  /**
   * Print warning if the library was not compiled with LZ4 library.
   * When this message appears, the compressed files will be unreadable.
   */
  void reader::printWarning() {
#ifndef __LZ4__
    std::cout << "******************************************************" << std::endl;
    std::cout << "* WARNING:                                           *" << std::endl;
    std::cout << "*   This library war compiled without LZ4 support.   *" << std::endl;
    std::cout << "*   Reading and writing compressed buffers will not  *" << std::endl;
    std::cout << "*   work. However un-compressed file I/O will work.  *" << std::endl;
    std::cout << "******************************************************" << std::endl;
#endif
  }
} // namespace hipo

//*************************************************************************
// implementation of record_index class
//*************************************************************************
namespace hipo {

  void reader_index::addSize(int size) {
    if (recordEvents.size() == 0) {
      recordEvents.push_back(0);
      recordEvents.push_back(size);
    } else {
      int cz = recordEvents[recordEvents.size() - 1] + size;
      recordEvents.push_back(cz);
    }
  }

  bool reader_index::advance() {
    if (recordEvents.size() == 0)
      return false;

    if (currentEvent + 1 < recordEvents[currentRecord + 1]) {
      currentEvent++;
      currentRecordEvent++;
      return true;
    }

    if (recordEvents.size() < currentRecord + 2 + 1) {
      printf("advance(): Warning, reached the limit of events.\n");
      return false;
    }
    currentEvent++;
    currentRecord++;
    currentRecordEvent = 0;
    return true;
  }

  int reader_index::getMaxEvents() {
    if (recordEvents.size() == 0)
      return 0;
    return recordEvents[recordEvents.size() - 1];
  }
} // namespace hipo
