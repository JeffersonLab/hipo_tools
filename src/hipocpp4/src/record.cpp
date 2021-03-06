/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo4/record.h"
//#include "hipoexceptions.h"

#ifdef __LZ4__
#include <lz4.h>
#endif

namespace hipo {

  record::record() {}

  record::~record() {}

  /**
   */
  void record::readRecord(std::ifstream& stream, long position, int dataOffset) {

    recordHeaderBuffer.resize(80);
    stream.seekg(position, std::ios::beg);

    stream.read((char*)&recordHeaderBuffer[0], 80);
    if (recordHeader.signatureString == 0xc0da0100)
      recordHeader.dataEndianness = 0;
    recordHeader.recordLength     = *(reinterpret_cast<int*>(&recordHeaderBuffer[0]));
    recordHeader.headerLength     = *(reinterpret_cast<int*>(&recordHeaderBuffer[8]));
    recordHeader.numberOfEvents   = *(reinterpret_cast<int*>(&recordHeaderBuffer[12]));
    recordHeader.bitInfo          = *(reinterpret_cast<int*>(&recordHeaderBuffer[20]));
    recordHeader.signatureString  = *(reinterpret_cast<int*>(&recordHeaderBuffer[28]));
    recordHeader.recordDataLength = *(reinterpret_cast<int*>(&recordHeaderBuffer[32]));
    recordHeader.userHeaderLength = *(reinterpret_cast<int*>(&recordHeaderBuffer[24]));
    int compressedWord            = *(reinterpret_cast<int*>(&recordHeaderBuffer[36]));

    if (recordHeader.signatureString == 0x0001dac0) {
      recordHeader.dataEndianness   = 1;
      recordHeader.recordLength     = __builtin_bswap32(recordHeader.recordLength);
      recordHeader.headerLength     = __builtin_bswap32(recordHeader.headerLength);
      recordHeader.numberOfEvents   = __builtin_bswap32(recordHeader.numberOfEvents);
      recordHeader.recordDataLength = __builtin_bswap32(recordHeader.recordDataLength);
      recordHeader.userHeaderLength = __builtin_bswap32(recordHeader.userHeaderLength);
      recordHeader.bitInfo          = __builtin_bswap32(recordHeader.bitInfo);
      compressedWord                = __builtin_bswap32(compressedWord);
    }

    int compressedDataLengthPadding = (recordHeader.bitInfo >> 24) & 0x00000003;
    int headerLengthBytes           = recordHeader.headerLength * 4;
    int dataBufferLengthBytes       = recordHeader.recordLength * 4 - headerLengthBytes;

    recordHeader.userHeaderLengthPadding    = (recordHeader.bitInfo >> 20) & 0x00000003;
    recordHeader.recordDataLengthCompressed = compressedWord & 0x0FFFFFFF;
    recordHeader.compressionType            = (compressedWord >> 28) & 0x0000000F;
    recordHeader.indexDataLength            = 4 * recordHeader.numberOfEvents;

    if (dataBufferLengthBytes > recordCompressedBuffer.size()) {
      int newSize = dataBufferLengthBytes + 5 * 1024;
      recordCompressedBuffer.resize(newSize);
    }

    long dataposition = position + headerLengthBytes;

    stream.seekg(dataposition, std::ios::beg);

    stream.read((&recordCompressedBuffer[0]), dataBufferLengthBytes);

    int decompressedLength = recordHeader.indexDataLength + recordHeader.userHeaderLength +
                             recordHeader.userHeaderLengthPadding + recordHeader.recordDataLength;

    if (recordBuffer.size() < decompressedLength) {
      recordBuffer.resize(decompressedLength + 1024);
    }

    if (recordHeader.compressionType == 0) {
      memcpy((&recordBuffer[0]), (&recordCompressedBuffer[0]), decompressedLength);
    } else {
      int unc_result =
          getUncompressed((&recordCompressedBuffer[0]), (&recordBuffer[0]),
                          dataBufferLengthBytes - compressedDataLengthPadding, decompressedLength);
    }

    /**
     * converting index array from lengths of each buffer in the
     * record to relative positions in the record stream.
     */

    int eventPosition = 0;
    for (int i = 0; i < recordHeader.numberOfEvents; i++) {
      int* ptr  = reinterpret_cast<int*>(&recordBuffer[i * 4]);
      int  size = *ptr;
      if (recordHeader.dataEndianness == 1)
        size = __builtin_bswap32(size);
      eventPosition += size;
      *ptr = eventPosition;
    }
  }

  bool record::readRecord(std::ifstream& stream, long position, int dataOffset, long inputSize) {
    if ((position + 80) >= inputSize)
      return false;

    recordHeaderBuffer.resize(80);
    stream.seekg(position, std::ios::beg);

    stream.read((char*)&recordHeaderBuffer[0], 80);
    recordHeader.recordLength     = *(reinterpret_cast<int*>(&recordHeaderBuffer[0]));
    recordHeader.headerLength     = *(reinterpret_cast<int*>(&recordHeaderBuffer[8]));
    recordHeader.numberOfEvents   = *(reinterpret_cast<int*>(&recordHeaderBuffer[12]));
    recordHeader.bitInfo          = *(reinterpret_cast<int*>(&recordHeaderBuffer[20]));
    recordHeader.signatureString  = *(reinterpret_cast<int*>(&recordHeaderBuffer[28]));
    recordHeader.recordDataLength = *(reinterpret_cast<int*>(&recordHeaderBuffer[32]));
    recordHeader.userHeaderLength = *(reinterpret_cast<int*>(&recordHeaderBuffer[24]));
    int compressedWord            = *(reinterpret_cast<int*>(&recordHeaderBuffer[36]));

    if (recordHeader.signatureString == 0xc0da0100)
      recordHeader.dataEndianness = 0;
    if (recordHeader.signatureString == 0x0001dac0)
      recordHeader.dataEndianness = 1;

    if (recordHeader.signatureString == 0x0001dac0) {
      recordHeader.recordLength     = __builtin_bswap32(recordHeader.recordLength);
      recordHeader.headerLength     = __builtin_bswap32(recordHeader.headerLength);
      recordHeader.numberOfEvents   = __builtin_bswap32(recordHeader.numberOfEvents);
      recordHeader.recordDataLength = __builtin_bswap32(recordHeader.recordDataLength);
      recordHeader.userHeaderLength = __builtin_bswap32(recordHeader.userHeaderLength);
      recordHeader.bitInfo          = __builtin_bswap32(recordHeader.bitInfo);
      compressedWord                = __builtin_bswap32(compressedWord);
    }

    int compressedDataLengthPadding = (recordHeader.bitInfo >> 24) & 0x00000003;
    int headerLengthBytes           = recordHeader.headerLength * 4;
    int dataBufferLengthBytes       = recordHeader.recordLength * 4 - headerLengthBytes;

    recordHeader.userHeaderLengthPadding    = (recordHeader.bitInfo >> 20) & 0x00000003;
    recordHeader.recordDataLengthCompressed = compressedWord & 0x0FFFFFFF;
    recordHeader.compressionType            = (compressedWord >> 28) & 0x0000000F;
    recordHeader.indexDataLength            = 4 * recordHeader.numberOfEvents;

    if (dataBufferLengthBytes > recordCompressedBuffer.size()) {
      int newSize = dataBufferLengthBytes + 5 * 1024;
      recordCompressedBuffer.resize(newSize);
    }

    long dataposition = position + headerLengthBytes;

    stream.seekg(dataposition, std::ios::beg);

    if (position + dataBufferLengthBytes + recordHeader.headerLength > inputSize) {
      std::cerr << "**** warning : record at position " << position << " is incomplete."
                << std::endl;
      return false;
    }
    stream.read((&recordCompressedBuffer[0]), dataBufferLengthBytes);

    int decompressedLength = recordHeader.indexDataLength + recordHeader.userHeaderLength +
                             recordHeader.userHeaderLengthPadding + recordHeader.recordDataLength;

    if (recordBuffer.size() < decompressedLength) {
      recordBuffer.resize(decompressedLength + 1024);
    }

    if (recordHeader.compressionType == 0) {
      memcpy((&recordBuffer[0]), (&recordCompressedBuffer[0]), decompressedLength);
    } else {
      int unc_result =
          getUncompressed((&recordCompressedBuffer[0]), (&recordBuffer[0]),
                          dataBufferLengthBytes - compressedDataLengthPadding, decompressedLength);
    }

    int eventPosition = 0;
    for (int i = 0; i < recordHeader.numberOfEvents; i++) {
      int* ptr  = reinterpret_cast<int*>(&recordBuffer[i * 4]);
      int  size = *ptr;
      if (recordHeader.dataEndianness == 1)
        size = __builtin_bswap32(size);
      eventPosition += size;
      *ptr = eventPosition;
    }
    return true;
  }

  int record::getRecordSizeCompressed() { return recordHeader.recordLength; }

  int record::getEventCount() { return recordHeader.numberOfEvents; }

  void record::readEvent(std::vector<char>& vec, int index) {}

  void record::getData(hipo::data& data, int index) {
    int first_position = 0;
    if (index > 0) {
      first_position = *(reinterpret_cast<uint32_t*>(&recordBuffer[(index - 1) * 4]));
    }
    int last_position = *(reinterpret_cast<uint32_t*>(&recordBuffer[index * 4]));
    int offset        = recordHeader.indexDataLength + recordHeader.userHeaderLength +
                 recordHeader.userHeaderLengthPadding;
    data.setDataPtr(&recordBuffer[first_position + offset]);
    data.setDataSize(last_position - first_position);
    data.setDataOffset(first_position + offset);
  }

  void record::readHipoEvent(hipo::event& event, int index) {
    hipo::data event_data;
    getData(event_data, index);
    event.init(event_data.getDataPtr(), event_data.getDataSize());
  }
  /**
   * prints the content of given buffer in HEX format. Used for debugging.
   */
  void record::showBuffer(const char* data, int wrapping, int maxsize) {
    for (int i = 0; i < maxsize; i++) {
      printf("%X ", 0x000000FF & ((unsigned int)data[i]));
      if ((i + 1) % wrapping == 0)
        printf("\n");
    }
    printf("\n");
  }
  /**
   * decompresses the buffer given with pointed *data, into a destination array
   * provided. The arguments indicate the compressed data length (dataLength),
   * and maximum decompressed length.
   * returns the number of bytes that were decompressed by LZ4
   */
  int record::getUncompressed(const char* data, char* dest, int dataLength,
                              int dataLengthUncompressed) {
#ifdef __LZ4__
    int result = LZ4_decompress_safe(data, dest, dataLength, dataLengthUncompressed);
    return result;
#else
    std::cerr >> "LZ4 compression is not supported." << std::endl;
    std::cerr >> "check if libz4 is installed on your system." << std::endl;
    std::cerr >> "recompile the library with liblz4 installed." << std::endl;
    return NULL;
#endif
  }
  /**
   * deompresses the content of given buffer ( *data), into a newly allocated
   * memory. User is responsible for free-ing the allocated memory.
   */
  char* record::getUncompressed(const char* data, int dataLength, int dataLengthUncompressed) {

#ifdef __LZ4__
    char* output = (char*)malloc(dataLengthUncompressed);
    int result = LZ4_decompress_safe(data, output, dataLength, dataLengthUncompressed);
    return output;
#else
    std::cerr >> "LZ4 compression is not supported." << std::endl;
    std::cerr >> "check if libz4 is installed on your system." << std::endl;
    std::cerr >> "recompile the library with liblz4 installed." << std::endl;
    return NULL;
#endif
  }

} // namespace hipo
