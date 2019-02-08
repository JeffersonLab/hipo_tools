/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo4/bank.h"
#include "hipo4/utils.h"

namespace hipo {
  //==============================================================
  // Definition of class structure, this will class will be extended
  // to represent different objects that will be appended to the event
  //==============================================================

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

  const char* structure::getAddress() { return structureAddress; }
  //====================================================================
  // END of structure class
  //====================================================================
  bank::bank() {}

  bank::~bank() {}

  void bank::notify() {
    int size = bankSchema.getRowLength();
    bankRows = getSize() / size;
    // printf("---> bank notify called structure size = %8d (size = %5d)  rows = %d\n",
    //    getSize(),size, bankRows);
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
      printf("---> error : requested INT for [%s] type = %d\n",
             bankSchema.getEntryName(item).c_str(), type);
      break;
    }
    return 0;
  }
  int bank::getShort(int item, int index) {
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    case 2:
      return (int)getShortAt(offset);
    default:
      printf("---> error : requested SHORT for [%s] type = %d\n",
             bankSchema.getEntryName(item).c_str(), type);
      break;
    }
    return 0;
  }
  int bank::getByte(int item, int index) {
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    default:
      printf("---> error : requested BYTE for [%s] type = %d\n",
             bankSchema.getEntryName(item).c_str(), type);
      break;
    }
    return 0;
  }
  float bank::getFloat(int item, int index) {
    if (bankSchema.getEntryType(item) == 4) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getFloatAt(offset);
    }
    return 0.0;
  }
  double bank::getDouble(int item, int index) {
    if (bankSchema.getEntryType(item) == 5) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getDoubleAt(offset);
    }
    return 0.0;
  }

  long bank::getLong(int item, int index) {
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongAt(offset);
    }
    return 0;
  }

  int bank::getInt(const char* name, int index) {
    int item   = bankSchema.getEntryOrder(name);
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
      printf("---> error : requested INT for [%s] type = %d\n", name, type);
      break;
    }
    return 0;
  }

  int bank::getShort(const char* name, int index) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    case 2:
      return (int)getShortAt(offset);
    default:
      printf("---> error : requested SHORT for [%s] type = %d\n",
             bankSchema.getEntryName(item).c_str(), type);
      break;
    }
    return 0;
  }
  int bank::getByte(const char* name, int index) {
    int item   = bankSchema.getEntryOrder(name);
    int type   = bankSchema.getEntryType(item);
    int offset = bankSchema.getOffset(item, index, bankRows);
    switch (type) {
    case 1:
      return (int)getByteAt(offset);
    default:
      printf("---> error : requested BYTE for [%s] type = %d\n",
             bankSchema.getEntryName(item).c_str(), type);
      break;
    }
    return 0;
  }

  float bank::getFloat(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 4) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getFloatAt(offset);
    }
    return 0.0;
  }

  double bank::getDouble(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 5) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getDoubleAt(offset);
    }
    return 0.0;
  }

  long bank::getLong(const char* name, int index) {
    int item = bankSchema.getEntryOrder(name);
    if (bankSchema.getEntryType(item) == 8) {
      int offset = bankSchema.getOffset(item, index, bankRows);
      return getLongAt(offset);
    }
    return 0;
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
