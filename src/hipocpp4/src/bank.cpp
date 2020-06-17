/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "hipo4/bank.h"
#include "hipo4/utils.h"
#include <cmath>

namespace hipo {
  //==============================================================
  // Definition of class structure, this will class will be extended
  // to represent different objects that will be appended to the event
  //==============================================================

  structure::structure(int __group, int __item, std::string& str) {
    int length = str.length();
    initStructureBySize(__group, __item, 6, length);
    putStringAt(0, str);
  }

  bool structure::allocate(int size) {
    if (structureBuffer.size() < size) {
      structureBuffer.resize(size + 32);
    }
    return true;
  }
  void structure::initStructureBySize(int __group, int __item, int __type, int __size) {
    allocate(__size + 8);
    structureAddress                                   = &structureBuffer[0];
    *reinterpret_cast<uint16_t*>(structureAddress)     = (uint16_t)__group;
    *reinterpret_cast<uint8_t*>(&structureAddress[2])  = (uint8_t)__item;
    *reinterpret_cast<uint8_t*>(&structureAddress[3])  = (uint8_t)__type;
    *reinterpret_cast<uint32_t*>(&structureAddress[4]) = __size;
  }
  /**
   * returns the size of the structure
   */
  int structure::getSize() {
    int size = *reinterpret_cast<uint32_t*>(structureAddress + 4);
    return size;
  }
  // return the type of the structure
  int structure::getType() {
    int type = (int)(*reinterpret_cast<uint8_t*>(structureAddress + 3));
    return type;
  }
  // returns the group number of the object
  int structure::getGroup() {
    int group = (int)(*reinterpret_cast<uint16_t*>(structureAddress));
    return group;
  }
  // returns the item number of the structure
  int structure::getItem() {
    int item = (int)(*reinterpret_cast<uint8_t*>(structureAddress + 2));
    return item;
  }
  void structure::init(const char* buffer, int size) {
    allocate(size);
    memcpy(&structureBuffer[0], buffer, size);
    structureAddress = &structureBuffer[0];
  }

  void structure::show() {
    printf("structure : [%5d,%5d] type = %4d, length = %6d\n", getGroup(), getItem(), getType(),
           getSize());
  }

  std::string structure::getStringAt(int index) {
    int   length    = getSize();
    char* string_ch = (char*)malloc(length + 1);
    std::memcpy(string_ch, &structureBuffer[8], length);
    string_ch[length]  = '\0';
    std::string result = string_ch;
    free(string_ch);
    return result;
  }

  void structure::putStringAt(int index, std::string& str) {
    int strLen = str.length();
    std::memcpy(&structureBuffer[8], &str[0], strLen);
  }

  const char* structure::getAddress() { return structureAddress; }
  //====================================================================
  // END of structure class
  //====================================================================
  bank::bank() {}

  bank::~bank() {}

  void bank::notify() {
    int size = bankSchema.getRowLength();
    bankRows = getSize() / size;
  }

  int bank::getInt(int item, int index) {
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    case 2:
      return (int)getShortAt(offset);
    case 3:
      return getIntAt(offset);
    default:
#ifdef __DEBUG__
      std::cerr << "---> error : requested INT for [" << bankSchema.getEntryName(item)
                << "] type = " << type << std::endl;
      break;
#endif
      return -99;
    }
    return -99;
  }
  int bank::getShort(int item, int index) {
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    case 2:
      return (int)getShortAt(offset);
    case 3:
      return getIntAt(offset);
    default:
#ifdef __DEBUG__
      std::cerr << "---> error : requested SHORT for [" << bankSchema.getEntryName(item)
                << "] type = " << type << std::endl;
      break;
#endif
      return -99;
    }
    return -99;
  }
  int bank::getByte(int item, int index) {
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    case 2:
      return (int)getShortAt(offset);
    case 3:
      return getIntAt(offset);
    default:
#ifdef __DEBUG__
      std::cerr << "---> error : requested BYTE for [" << bankSchema.getEntryName(item)
                << "] type = " << type << std::endl;
      break;
#endif
      return -99;
    }
    return -99;
  }

  float bank::getFloat(int item, int index) {
    if (bankSchema.getEntryType(item) == 4) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getFloatAt(offset);
    }
    return std::nanf("-99");
  }
  double bank::getDouble(int item, int index) {
    if (bankSchema.getEntryType(item) == 5) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getDoubleAt(offset);
    }
    return std::nanf("-99");
  }

  long bank::getLong(int item, int index) {
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongAt(offset);
    }
    return -99;
  }

  long long bank::getLongLong(int item, int index) {
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongLongAt(offset);
    }
    return -99;
  }

  void bank::putInt(const char* name, int index, int32_t value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putIntAt(offset, value);
  }
  void bank::putShort(const char* name, int index, int16_t value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putShortAt(offset, value);
  }
  void bank::putByte(const char* name, int index, int8_t value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putByteAt(offset, value);
  }
  void bank::putFloat(const char* name, int index, float value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putFloatAt(offset, value);
  }
  void bank::putDouble(const char* name, int index, double value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putDoubleAt(offset, value);
  }
  void bank::putLong(const char* name, int index, int64_t value) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    putLongAt(offset, value);
  }

  int bank::getInt(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    return getInt(item, index);
  }

  int bank::getShort(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    return getInt(item, index);
  }
  int bank::getByte(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    return getInt(item, index);
  }

  float bank::getFloat(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 4) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getFloatAt(offset);
    }
    return std::nanf("-99");
  }

  double bank::getDouble(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 5) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getDoubleAt(offset);
    }
    return std::nanf("-99");
  }

  long bank::getLong(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongAt(offset);
    }
    return -99;
  }

  long long bank::getLongLong(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongLongAt(offset);
    }
    return -99;
  }

  void bank::show() {
    for (int i = 0; i < bankSchema.getEntries(); i++) {
      printf("%14d : ", i);
      for (int k = 0; k < bankRows; k++) {
        if (bankSchema.getEntryType(i) < 4) {
          printf("%8d ", getInt(i, k));
        } else if (bankSchema.getEntryType(i) == 4) {
          printf("%8.5f ", getFloat(i, k));
        }
      }
      printf("\n");
    }
  }

} // namespace hipo
