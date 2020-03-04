#include "hipo4/reader.h"
#include "hipo4/writer.h"
#include <cstdlib>
#include <iostream>

int main(int argc, char** argv) {
  hipo::reader reader;
  reader.open(argv[1]);
  hipo::dictionary factory;
  reader.readDictionary(factory);

  hipo::bank  particles(factory.getSchema("REC::Particle"));
  hipo::event event;

  int counter = 0;
  while (reader.next() == true && counter < 500) {
    counter++;

    reader.read(event);
    event.getStructure(particles);

    int nrows = particles.getRows();
    std::cout << nrows << std::endl;
  }
}
