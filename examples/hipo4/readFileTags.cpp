#include "hipo4/reader.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {

  std::cout << " reading file example program (HIPO) " << __cplusplus << std::endl;

  char inputFile[256];

  if (argc > 1) {
    sprintf(inputFile, "%s", argv[1]);
    // sprintf(outputFile,"%s",argv[2]);
  } else {
    std::cout << " *** please provide a file name..." << std::endl;
    exit(0);
  }

  hipo::reader reader;
  reader.setTags(0);
  reader.open(inputFile);
  hipo::dictionary factory;
  reader.readDictionary(factory);

  factory.show();

  hipo::bank helicity(factory.getSchema("HEL::flip"));

  hipo::event event;

  int counter = 0;

  while (reader.next() == true) {
    reader.read(event);

    // event.getStructure(helicity);
    // helicity.show();

    counter++;
  }
  printf("processed events = %d\n", counter);
}
//### END OF GENERATED CODE
