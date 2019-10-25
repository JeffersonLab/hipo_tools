/*
 * This sowftware was developed at Jefferson National Laboratory.
 * (c) 2017.
 */

#include "hipo4/writer.h"
#include <cstdlib>

namespace hipo {
  writer::writer(const char* filename) { writer::open(filename); }
  writer::writer(const std::string& filename) { writer::open(filename.c_str()); }

  void writer::open(const std::string& filename) { writer::open(filename.c_str()); }
  void writer::open(const char* filename) {
    outputStream.open(filename);

    std::vector<std::string> schemaList = writerDictionary.getSchemaList();

    recordbuilder builder;
    event         schemaEvent;

    for (int i = 0; i < schemaList.size(); i++) {
      std::string schemaString =
          writerDictionary.getSchema(schemaList[i].c_str()).getSchemaString();
      std::string schemaStringJson =
          writerDictionary.getSchema(schemaList[i].c_str()).getSchemaStringJson();
      schemaEvent.reset();
      structure schemaNode(120, 2, schemaString);
      structure schemaNodeJson(120, 1, schemaStringJson);
      schemaEvent.addStructure(schemaNode);
      schemaEvent.addStructure(schemaNodeJson);
      schemaEvent.show();
      builder.addEvent(schemaEvent);
    }

    builder.build();

    int dictionarySize = builder.getRecordSize();

    hipoFileHeader_t header;

    header.uniqueid         = 0x43455248;
    header.filenumber       = 1;
    header.headerLength     = 14;
    header.recordCount      = 0;
    header.indexArrayLength = 0;
    header.bitInfoVersion   = (0x000000FF & 6);
    header.userHeaderLength = dictionarySize; // will change with the dictionary
    header.magicNumber      = 0xc0da0100;
    header.userRegister     = 0;
    header.trailerPosition  = 0;
    header.userIntegerOne   = 0;
    header.userIntegerTwo   = 0;

    outputStream.write(reinterpret_cast<char*>(&header), sizeof(header));
    long position = outputStream.tellp();

    outputStream.write(reinterpret_cast<char*>(&builder.getRecordBuffer()[0]), dictionarySize);
    position = outputStream.tellp();
  }

  void writer::addEvent(hipo::event& hevent) {
    bool status = recordBuilder.addEvent(hevent);
    if (status == false) {
      writeRecord(recordBuilder);
      recordBuilder.addEvent(hevent);
    }
  }

  void writer::writeRecord(recordbuilder& builder) {
    builder.build();
    recordInfo_t recordInfo;
    recordInfo.recordPosition = outputStream.tellp();
    recordInfo.recordEntries  = builder.getEntries();
    recordInfo.recordLength   = builder.getRecordSize();
    recordInfo.userWordOne    = builder.getUserWordOne();
    recordInfo.userWordTwo    = builder.getUserWordTwo();
    if (recordInfo.recordEntries > 0) {
      outputStream.write(reinterpret_cast<char*>(&builder.getRecordBuffer()[0]),
                         recordInfo.recordLength);
      writerRecordInfo.push_back(recordInfo);
    }
    builder.reset();
  }

  void writer::showSummary() {
    for (int loop = 0; loop < writerRecordInfo.size(); loop++) {
      recordInfo_t recordInfo = writerRecordInfo[loop];
      printf(
          " %6d : record INFO : size = %8d, entries = %8d, position = %12ld word = %12ld %12ld\n",
          loop, recordInfo.recordLength, recordInfo.recordEntries, recordInfo.recordPosition,
          recordInfo.userWordOne, recordInfo.userWordTwo);
    }
  }

  void writer::writeIndexTable() {
    hipo::schema indexSchema("file::index", 32111, 1);
    indexSchema.parse("position/L,length/I,entries/I,userWordOne/L,userWordTwo/L");
    int        nEntries      = writerRecordInfo.size();
    long       indexPosition = outputStream.tellp();
    hipo::bank indexBank(indexSchema, nEntries);
    for (int i = 0; i < nEntries; i++) {
      recordInfo_t recordInfo = writerRecordInfo[i];
      indexBank.putLong("position", i, recordInfo.recordPosition);
      indexBank.putInt("length", i, recordInfo.recordLength);
      indexBank.putInt("entries", i, recordInfo.recordEntries);
      indexBank.putLong("userWordOne", i, recordInfo.userWordOne);
      indexBank.putLong("userWordTwo", i, recordInfo.userWordTwo);
    }

    int eventSize = 32 * nEntries + 1024;

    hipo::event indexEvent(eventSize);
    indexEvent.addStructure(indexBank);
    recordBuilder.reset();
    recordBuilder.addEvent(indexEvent);
    writeRecord(recordBuilder);
    outputStream.seekp(40);
    outputStream.write(reinterpret_cast<char*>(&indexPosition), 8);
  }

  void writer::close() {
    writeRecord(recordBuilder);
    writeIndexTable();
    outputStream.close();
  }

} // namespace hipo
