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

    reader.open(filename);
    reader.showInfo();

    int nrecords = reader.getRecordCount();

    printf("-----> file contains %d records\n",nrecords);
    printf("\n\n");
    printf("-----> start reading records.\n");

    int ecounter = 0;
    for(int i = 0; i < nrecords; i++){

       reader.readRecord(record,i);
       int nevents = record.getEventCount();
       for(int k = 0; k < nevents; k++){
         record.readHipoEvent(event,k);
       }
       printf(" event # %d\n",ecounter);
         ecounter++;
    }
    printf("-----> done reading records. n events = %d\n",ecounter);

    /*printf("-----> readin records with next() \n");
    hipo::reader reader2;
    hipo::node<float>       *mc__event_mass = reader2.getBranch<float>("mc::event","mass");
    reader2.open(filename);
    while(reader2.next()==true){

    }*/
    return 0;
}
