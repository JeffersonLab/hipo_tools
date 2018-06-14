/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   read_file.cpp
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:30 PM
 */

#include <cstdlib>
#include <iostream>

#include "reader.h"
#include "writer.h"
#include "event.h"
#include "node.h"
#include "text.h"
#include "data.h"


using namespace std;
/*
 *
 */
int main(int argc, char** argv) {

    std::cout << "start file reading test" << '\n';

    char filename[128];

    if(argc>1){
        sprintf(filename,"%s",argv[1]);
    } else {
      printf(" \n\n Please provide a filename to read....\n\n");
      exit(0);
    }
    printf("-----> open file : %s\n",filename);
    hipo::reader reader;
    hipo::record record;
    hipo::record dictionary;


    hipo::event  event;

    //hipo::node<int> *node_pid = event.getIntNode(21611,5);
    hipo::node<uint8_t>   *node_sector    = event.getBranch<uint8_t>(21611,1);
    hipo::node<uint8_t>   *node_layer     = event.getBranch<uint8_t>(21611,2);
    hipo::node<uint16_t>  *node_component = event.getBranch<uint16_t>(21611,3);
    hipo::node<uint32_t>  *node_adc       = event.getBranch<uint32_t>(21611,5);
    hipo::node<float>     *node_time      = event.getBranch<float>(21611,6);
  /*  hipo::node<int> *node_px  = event.getIntNode(331,2);
    hipo::node<int> *node_py  = event.getIntNode(331,3);
    hipo::node<int> *node_pz  = event.getIntNode(331,4);
    hipo::node<int> *node_vx  = event.getIntNode(331,5);
    hipo::node<int> *node_vy  = event.getIntNode(331,6);
*/
    reader.open(filename);
    reader.showInfo();

    reader.readHeaderRecord(dictionary);

    printf(" DICTIONARY READ COUNT = %d\n",dictionary.getEventCount());


    hipo::event  schema;
    for(int d = 0; d < dictionary.getEventCount(); d++){
        dictionary.readHipoEvent(schema,d);
        printf("Dictionary event %d is read\n",d);
        std::string schemaString = schema.getString(31111,1);
        printf("schema : %s\n",schemaString.c_str());
        //schema.showInfo();
    }


    int nrecords = reader.getRecordCount();

    printf("-----> file contains %d records\n",nrecords);
    printf("\n\n");
    printf("-----> start reading records.\n");

    int ecounter = 0;
    for(int i = 0; i < nrecords; i++){
       reader.readRecord(record,i);
       int nevents = record.getEventCount();
       //printf(" RECORD # %d has %d events\n", i, nevents);
       for(int k = 0; k < nevents; k++){
         record.readHipoEvent(event,k);
         int length = node_sector->getLength();
         printf("event # %d , LTCC data size = %d\n",ecounter,length);
         for(int k = 0; k < length; k++){
           printf("%4d %4d %4d %6d %f\n",
               node_sector->getValue(k),
               node_layer->getValue(k),
               node_component->getValue(k),
               node_adc->getValue(k),
               node_time->getValue(k)
            );
           //printf(" %d ",node_pid->getValue(k));
         }
         //printf("\n");
         ecounter++;
       }
    }
    printf("-----> done reading records. n events = %d\n",ecounter);
    return 0;
}
