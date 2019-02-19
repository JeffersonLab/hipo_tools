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
  int nrecords = reader.getRecordCount();
  printf("-----> file contains %d records\n", nrecords);
  printf("\n\n");
  printf("-----> start reading records.\n");

  hipo::record record;
  for (int i = 0; i < nrecords; i++) {
    reader.readRecord(record, i);
  }
  printf("-----> done reading records.\n");
  return 0;
}
