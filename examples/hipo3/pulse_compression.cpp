/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   hipodebug.cpp
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

/***********************************************************
 * Read , separated lines from a file, construct a pulses
 * fomr each line, then compress them (bit pack) with
 * data compression utilities.
 ***********************************************************/

int main(int argc, char** argv) {

  std::cout << "start debug program" << '\n';

  char filename[128];

  if (argc > 0) {
    sprintf(filename, "%s", argv[1]);
  } else {
    printf(" \n\n Please provide a filename to read....\n\n");
    exit(0);
  }
  printf("--> open file : %s\n", filename);

  // readFile(filename);
  text::reader reader;
  reader.open(filename);
  std::string delim = ",";
  reader.setDalim(delim);

  ofstream myFileRaw;
  myFileRaw.open("data_raw.bin", std::ios::out | std::ios::binary);
  ofstream myFileComp;
  myFileComp.open("data_comp.bin", std::ios::out | std::ios::binary);
  ofstream myFileCompLossy;
  myFileCompLossy.open("data_comp_lossy.bin", std::ios::out | std::ios::binary);

  data::data decoder;

  std::vector<uint16_t> low, high;
  std::vector<char>     encoded;
  std::vector<char>     encodedLossy;

  std::vector<char> vec16;
  int               i = 0;
  while (reader.readLine()) {
    std::vector<int> vec = reader.getIntVector();

    vec16.resize(vec.size() * 2);
    for (int p = 0; p < vec.size(); p++) {
      uint16_t* ptr   = reinterpret_cast<uint16_t*>(&vec16[p * 2]);
      uint16_t  value = (uint16_t)vec[p];
      *ptr            = value;
    }

    i++;
    // printf("********************** EVENTS \n");
    decoder.decompose(vec, low, high);
    decoder.encode(vec, encoded);
    decoder.encodeLossy(vec, encodedLossy);
    printf("#----->\n");

    std::vector<int> vecSUB = decoder.getSubtracted(vec);
    std::vector<int> vecREM = decoder.getReiman(vecSUB);
    decoder.print(vecSUB);
    decoder.print(vecREM);
    decoder.print(vec);
    decoder.print(low);
    decoder.print(high);
    myFileComp.write(&encoded[0], encoded.size());
    myFileCompLossy.write(&encodedLossy[0], encodedLossy.size());
    myFileRaw.write(&vec16[0], vec16.size());
  }

  myFileComp.close();
  myFileRaw.close();
  printf("----> processed %d pulses\n", i);
  return 0;
}
