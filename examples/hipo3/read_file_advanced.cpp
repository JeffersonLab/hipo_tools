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

#include "hipo3/data.h"
#include "hipo3/event.h"
#include "hipo3/node.h"
#include "hipo3/reader.h"
#include "hipo3/text.h"
#include "hipo3/writer.h"

using namespace std;
/*
 *
 */
int main(int argc, char** argv) {

  std::cout << "start file reading test" << '\n';

  char filename[128];

  if (argc > 1) {
    sprintf(filename, "%s", argv[1]);
  } else {
    printf(" \n\n Please provide a filename to read....\n\n");
    exit(0);
  }
  printf("-----> open file : %s\n", filename);
  hipo::reader reader;
  reader.open(filename);

  reader.showInfo();

  hipo::node<uint8_t>*  node_sector    = reader.getBranch<uint8_t>("ECAL::adc", "sector");
  hipo::node<uint8_t>*  node_layer     = reader.getBranch<uint8_t>("ECAL::adc", "layer");
  hipo::node<uint16_t>* node_component = reader.getBranch<uint16_t>("ECAL::adc", "component");
  hipo::node<uint32_t>* node_adc       = reader.getBranch<uint32_t>("ECAL::adc", "ADC");
  hipo::node<float>*    node_time      = reader.getBranch<float>("ECAL::adc", "time");

  int nrecords = reader.getRecordCount();

  printf("-----> file contains %d records\n", nrecords);
  printf("\n\n");
  printf("-----> start reading records.\n");

  std::vector<string> dict = reader.getDictionary();
  for (int i = 0; i < dict.size(); i++) {
    printf(" Schema # %4d : %s\n", i, dict[i].c_str());
  }

  int ecounter = 0;
  while (reader.next() == true) {
    int length = node_sector->getLength();
    printf("event # %d , ECAL data size = %d\n", ecounter, length);
    for (int k = 0; k < length; k++) {
      printf("%4d %4d %4d %6d %f\n", node_sector->getValue(k), node_layer->getValue(k),
             node_component->getValue(k), node_adc->getValue(k), node_time->getValue(k));
      // printf(" %d ",node_pid->getValue(k));
    }
    ecounter++;
  }
  printf("-----> done reading records.\n");
  return 0;
}
