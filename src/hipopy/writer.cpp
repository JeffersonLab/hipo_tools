/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo/writer.h"
#include "hipo/utils.h"

namespace hipo {

    writer::writer(){
        __MAX_RECORD_SIZE__ = 8*1024*1024;
        printWarning();
    }

    writer::~writer(){
        if(outputStream.is_open()){
            outputStream.close();
        }
    }

    void writer::open(const char* filename){
        utils::printLogo();
        outputStream.open(filename,std::ios::out | std::ios::binary);
        std::vector<char> headerVec(72,0);
        char *header = &headerVec[0];

        header[0] = 'H'; header[1] = 'I'; header[2] = 'P'; header[3] ='O';
        header[4] = 'V'; header[5] = '0'; header[6] = '.'; header[7] ='2';
        uint32_t *headerRecordLength = reinterpret_cast<uint32_t *>(&header[12]);
        *headerRecordLength = 0;
        outputStream.write(&header[0],72);
    }

    void writer::open(const char* filename, std::vector<char> &headerRec){
        outputStream.open(filename,std::ios::out | std::ios::binary);
        std::vector<char> headerVec(72,0);
        char *header = &headerVec[0];

        header[0] = 'H'; header[1] = 'I'; header[2] = 'P'; header[3] ='O';
        header[4] = 'V'; header[5] = '0'; header[6] = '.'; header[7] ='2';
        uint32_t *headerRecordLength = reinterpret_cast<uint32_t *>(&header[12]);
        *headerRecordLength = headerRec.size();
        outputStream.write(&header[0],72);
        outputStream.write(&headerRec[0],headerRec.size());
    }

    void writer::writeRecord(hipo::record& record){
      /*  std::vector<char> recordVec = record.build();
        outputStream.write(&recordVec[0],recordVec.size());*/
    }

    void writer::writeEvent(hipo::event& event){
        /*writerRecord.addEvent(event);*/
    }

    void writer::writeEvent(std::vector<char> &event){
      /*  writerRecord.addEvent(event);
        if(writerRecord.getDataSize()>__MAX_RECORD_SIZE__){
            writeRecord(writerRecord);
            writerRecord.reset();
        }*/
    }

    void writer::close(){
      /*  if(writerRecord.getEventCount()>0){
            writeRecord(writerRecord);
            writerRecord.reset();
        }
        outputStream.close();*/
    }


    void writer::printWarning(){
#ifndef __LZ4__
        std::cout << "******************************************************" << std::endl;
        std::cout << "* WARNING:                                           *" << std::endl;
        std::cout << "*   This library war compiled without LZ4 support.   *" << std::endl;
        std::cout << "*   Reading and writing compressed buffers will not  *" << std::endl;
        std::cout << "*   work. However un-compressed file I/O will work.  *" << std::endl;
        std::cout << "******************************************************" << std::endl;
#endif
    }
}
