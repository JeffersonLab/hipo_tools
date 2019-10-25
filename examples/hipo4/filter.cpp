#include "hipo4/reader.h"
#include "hipo4/writer.h"
#include <TMath.h>
#include <cstdlib>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
  int          Nwritten = 0, counter = 0;
  hipo::writer myWriter;
  hipo::reader reader;
  reader.open(argv[1]);
  hipo::dictionary factory;
  reader.readDictionary(factory);

  hipo::bank particles(factory.getSchema("REC::Particle"));

  myWriter.getDictionary().addSchema(factory.getSchema("RUN::config"));
  myWriter.getDictionary().addSchema(factory.getSchema("REC::Event"));
  myWriter.getDictionary().addSchema(factory.getSchema("REC::Particle"));
  myWriter.open(argv[2]);
  hipo::event event;

  while (reader.next() == true && counter < 50000000) {
    counter++;

    reader.read(event);

    event.getStructure(particles);

    int nrows = particles.getRows();

    bool isSelected = false;
    for (int row = 0; row < nrows && !isSelected; row++) {
      int  pid    = particles.getInt("pid", row);
      int  status = particles.getInt("status", row);
      int  stat   = TMath::Abs(status);
      bool inDC   = (stat >= 2000 && stat < 4000);

      if (pid == 11 && inDC) {
        isSelected = true;
      }
    }
    if (isSelected) {
      Nwritten++;
      myWriter.addEvent(event);
    }
  }
  myWriter.close();
}
