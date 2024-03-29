/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   bank.h
 * Author: gavalian
 *
 * Created on April 12, 2017, 10:14 AM
 */

#ifndef HIPO_BANK_H
#define HIPO_BANK_H
#include "dictionary.h"
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <vector>

namespace hipo {

  class structure {

  private:
    std::vector<char> structureBuffer;
    char*             structureAddress;
    void              setAddress(const char* address);

  protected:
    void               initStructureBySize(int __group, int __item, int __type, int __size);
    std::vector<char>& getStructureBuffer() { return structureBuffer; }
    int                getStructureBufferSize() { return 8 + getSize(); }

  public:
    structure() { structureAddress = NULL; }
    structure(int size) { allocate(size); }
    structure(int __group, int __item, std::string& str);

    virtual ~structure() {}
    bool         allocate(int size);
    int          getSize();
    int          getType();
    int          getGroup();
    int          getItem();
    void         init(const char* buffer, int size);
    void         initNoCopy(const char* buffer, int size);
    const char*  getAddress();
    virtual void show();
    void         setSize(int size);

    int getIntAt(int index) { return *reinterpret_cast<int32_t*>(&structureAddress[index + 8]); }
    int16_t getShortAt(int index) {
      return *reinterpret_cast<int16_t*>(&structureAddress[index + 8]);
    }
    int8_t getByteAt(int index) { return *reinterpret_cast<int8_t*>(&structureAddress[index + 8]); }
    float  getFloatAt(int index) { return *reinterpret_cast<float*>(&structureAddress[index + 8]); }
    double getDoubleAt(int index) {
      return *reinterpret_cast<double*>(&structureAddress[index + 8]);
    }
    long getLongAt(int index) { return *reinterpret_cast<int64_t*>(&structureAddress[index + 8]); }

    long long getLongLongAt(int index) {
      return *reinterpret_cast<int64_t*>(&structureAddress[index + 8]);
    }

    std::string getStringAt(int index);

    void putIntAt(int index, int value) {
      *reinterpret_cast<int32_t*>(&structureAddress[index + 8]) = value;
    }

    void putShortAt(int index, int16_t value) {
      *reinterpret_cast<int16_t*>(&structureAddress[index + 8]) = value;
    }

    void putByteAt(int index, int8_t value) {
      *reinterpret_cast<int8_t*>(&structureAddress[index + 8]) = value;
    }

    void putFloatAt(int index, float value) {
      *reinterpret_cast<float*>(&structureAddress[index + 8]) = value;
    }

    void putDoubleAt(int index, double value) {
      *reinterpret_cast<double*>(&structureAddress[index + 8]) = value;
    }

    void putLongAt(int index, int64_t value) {
      *reinterpret_cast<int64_t*>(&structureAddress[index + 8]) = value;
    }

    void putStringAt(int index, std::string& str);

    virtual void notify() {}
    friend class event;
  };

  class bank : public hipo::structure {

  private:
    hipo::schema bankSchema;
    int          bankRows;

  protected:
    void setBankRows(int rows) { bankRows = rows; }

  public:
    bank();
    // constructor initializes the nodes in the bank
    // and they will be filled automatically by reader.next()
    // method.
    bank(hipo::schema __schema) {
      bankSchema = __schema;
      bankRows   = -1;
    }

    bank(hipo::schema __schema, int __rows) {
      bankSchema = __schema;
      bankRows   = __rows;
      int size   = bankSchema.getSizeForRows(__rows);
      initStructureBySize(bankSchema.getGroup(), bankSchema.getItem(), 11, size);
    }

    ~bank();
    // display the content of the bank
    // void show();

    hipo::schema& getSchema() { return bankSchema; }

    int  getRows() { return bankRows; }
    void setRows(int rows);

    template <typename T>
    T get(int item, int index) {
      int type   = bankSchema.getEntryType(item);
      int offset = bankSchema.getOffset(item, index, bankRows);
      switch (type) {
      case 1:
        return (int)getByteAt(offset);
      case 2:
        return (int)getShortAt(offset);
      case 3:
        return getIntAt(offset);
      case 4:
        return getFloatAt(offset);
      case 5:
        return getDoubleAt(offset);
      case 8:
        return getLongAt(offset);
      default:
        if (typeid(T) == typeid(float()) || typeid(T) == typeid(double()))
          return (float)NAN;
        else
          return -99;
      }
    }

    template <typename T>
    T get(std::string name, int index) {
      int item = bankSchema.getEntryOrder(name.c_str());
      return this->get<T>(item, index);
    }

    int       getInt(int item, int index);
    int       getShort(int item, int index);
    int       getByte(int item, int index);
    float     getFloat(int item, int index);
    double    getDouble(int item, int index);
    long      getLong(int item, int index);
    long long getLongLong(int item, int index);

    int       getInt(const char* name, int index);
    int       getShort(const char* name, int index);
    int       getByte(const char* name, int index);
    float     getFloat(const char* name, int index);
    double    getDouble(const char* name, int index);
    long      getLong(const char* name, int index);
    long long getLongLong(const char* name, int index);

    int       getInt(std::string name, int index) { return getInt(name.c_str(), index); }
    int       getShort(std::string name, int index) { return getShort(name.c_str(), index); }
    int       getByte(std::string name, int index) { return getByte(name.c_str(), index); }
    float     getFloat(std::string name, int index) { return getFloat(name.c_str(), index); }
    double    getDouble(std::string name, int index) { return getDouble(name.c_str(), index); }
    long      getLong(std::string name, int index) { return getLong(name.c_str(), index); }
    long long getLongLong(std::string name, int index) { return getLongLong(name.c_str(), index); }

    void putInt(const char* name, int index, int32_t value);
    void putShort(const char* name, int index, int16_t value);
    void putByte(const char* name, int index, int8_t value);
    void putFloat(const char* name, int index, float value);
    void putDouble(const char* name, int index, double value);
    void putLong(const char* name, int index, int64_t value);

    void show();
    void reset();
    // virtual  void notify(){ };

    virtual void notify();
  };

} // namespace hipo
#endif /* EVENT_H */
