/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   writer.h
 * Author: gavalian
 *
 * Created on April 25, 2017, 3:04 PM
 */

#ifndef WRITER_H
#define WRITER_H

#include <iostream>
#include <fstream>

#include "record.h"
#include "event.h"

namespace hipo {
    class writer {
    private:

        record writerRecord;
        record headerRecord;
        int    __MAX_RECORD_SIZE__ ; //= 8*1024*1024;
        std::ofstream outputStream;

    public:

        writer();
        ~writer();

        void open(const char *filename);
        void open(const char *filename, std::vector<char> &headerRec);
        void close();
        void writeEvent(std::vector<char> &event);
        void writeEvent(hipo::event &event);
        void writeRecord(hipo::record  &record);

        void printWarning();
    };
}

#endif /* WRITER_H */
