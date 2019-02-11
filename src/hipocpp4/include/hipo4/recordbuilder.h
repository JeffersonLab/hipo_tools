/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   hiporecord.h
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:47 PM
 */

#ifndef HIPORECORDBUILDER_H
#define HIPORECORDBUILDER_H

#include <fstream>
#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "event.h"
#include "utils.h"

namespace hipo {

  class recordbuilder {

  private:
    const int defaultNumberOfEvents = 100000;
    const int defaultRecordSize     = 8 * 1024 * 1024;
    // std::vector< std::vector<char> > eventBuffer;
    std::vector<char> bufferIndex;
    std::vector<char> bufferEvents;
    std::vector<char> bufferData;
    std::vector<char> bufferRecord;

    int bufferIndexEntries;
    int bufferEventsPosition;

    int compressRecord(int src_size);
    int getRecordLengthRounding(int bufferSize);

  public:
    recordbuilder(int maxEvents, int maxLength);
    recordbuilder();
    virtual ~recordbuilder(){};

    bool addEvent(std::vector<char>& vec, int start, int length);
    bool addEvent(hipo::event& evnt);

    int                getRecordSize();
    std::vector<char>& getRecordBuffer() { return bufferRecord; };
    void               reset();
    void               build();
  };
} // namespace hipo
#endif /* HIPORECORD_H */
