//******************************************************************************
//*       ██╗  ██╗██╗██████╗  ██████╗     ██╗  ██╗    ██████╗                  *
//*       ██║  ██║██║██╔══██╗██╔═══██╗    ██║  ██║   ██╔═████╗                 *
//*       ███████║██║██████╔╝██║   ██║    ███████║   ██║██╔██║                 *
//*       ██╔══██║██║██╔═══╝ ██║   ██║    ╚════██║   ████╔╝██║                 *
//*       ██║  ██║██║██║     ╚██████╔╝         ██║██╗╚██████╔╝                 *
//*       ╚═╝  ╚═╝╚═╝╚═╝      ╚═════╝          ╚═╝╚═╝ ╚═════╝                  *
//************************ Jefferson National Lab (2017) ***********************
/*
 *   Copyright (c) 2017.  Jefferson Lab (JLab). All rights reserved. Permission
 *   to use, copy, modify, and distribute  this software and its documentation
 *   for educational, research, and not-for-profit purposes, without fee and
 *   without a signed licensing agreement.
 *
 *   IN NO EVENT SHALL JLAB BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT, SPECIAL
 *   INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST PROFITS, ARISING
 *   OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF JLAB HAS
 *   BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *   JLAB SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *   PURPOSE. THE HIPO DATA FORMAT SOFTWARE AND ACCOMPANYING DOCUMENTATION, IF
 *   ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS". JLAB HAS NO OBLIGATION TO
 *   PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 *
 *   This software was developed under the United States Government license.
 *   For more information contact author at gavalian@jlab.org
 *   Department of Experimental Nuclear Physics, Jefferson Lab.
 */
/*******************************************************************************
 * File:   bank.cc
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
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

  void structure::setSize(int size) { *reinterpret_cast<uint32_t*>(structureAddress + 4) = size; }
  // return the type of the structure
  int structure::getType() {
    auto type = (int)(*reinterpret_cast<uint8_t*>(structureAddress + 3));
    return type;
  }
  // returns the group number of the object
  int structure::getGroup() {
    auto group = (int)(*reinterpret_cast<uint16_t*>(structureAddress));
    return group;
  }
  // returns the item number of the structure
  int structure::getItem() {
    auto item = (int)(*reinterpret_cast<uint8_t*>(structureAddress + 2));
    return item;
  }
  void structure::initNoCopy(const char* buffer, int size) {
    structureAddress = const_cast<char*>(buffer);
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
    auto* string_ch = (char*)malloc(length + 1);
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

  bank::bank() = default;

  bank::~bank() = default;

  void bank::setRows(int rows) {
    bankRows = rows;
    int size = bankSchema.getSizeForRows(bankRows);
    initStructureBySize(bankSchema.getGroup(), bankSchema.getItem(), 11, size);
  }

  void bank::reset() {
    setSize(0);
    bankRows = 0;
  }

  void bank::notify() {
    if (bankSchema.getRowLength() == 0)
      return;

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
    // printf("---- put float %f at position = %d\n",value,offset);
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

    printf("BANK :: NAME %24s , ROWS %6d \n", bankSchema.getName().c_str(), getRows());
    for (int i = 0; i < bankSchema.getEntries(); i++) {
      // printf("%14d : ", i);
      printf("%18s : ", bankSchema.getEntryName(i).c_str());
      for (int k = 0; k < bankRows; k++) {
        if (bankSchema.getEntryType(i) < 4) {
          printf("%8d ", getInt(i, k));
        } else {
          if (bankSchema.getEntryType(i) == 4) {
            printf("%8.5f ", getFloat(i, k));
          }
          if (bankSchema.getEntryType(i) == 5) {
            printf("%8.5f ", getDouble(i, k));
          }
          if (bankSchema.getEntryType(i) == 8) {
            printf("%14ld ", getLong(i, k));
          }
        }
      }
      printf("\n");
    }
  }

} // namespace hipo
