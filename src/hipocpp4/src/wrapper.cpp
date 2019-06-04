#include "hipo4/reader.h"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, hipo::bank*> hipo_bank_map;
hipo::reader*                        hipo_FORT_Reader;
hipo::dictionary*                    hipo_FORT_dict;
hipo::event*                         hipo_FORT_event;

extern "C" {

void hipo_open_file_(int* nevents, const char* filename, int length) {
  char* buffer = (char*)malloc(length + 1);
  memcpy(buffer, filename, length);
  buffer[length]   = '\0';
  hipo_FORT_Reader = new hipo::reader(buffer);
  free(buffer);

  hipo_FORT_dict = new hipo::dictionary();
  hipo_FORT_Reader->readDictionary(*hipo_FORT_dict);
  hipo_FORT_event = new hipo::event();
  *nevents        = hipo_FORT_Reader->numEvents();
}

int reader_next_() {
  if (hipo_FORT_Reader->next()) {
    hipo_FORT_Reader->read(*hipo_FORT_event);
    for (auto const& x : hipo_bank_map) {
      hipo_FORT_event->getStructure(*x.second);
    }
    return 1;
  } else {
    for (auto const& x : hipo_bank_map) {
      delete x.second;
    }
    delete hipo_FORT_Reader;
    delete hipo_FORT_dict;
    delete hipo_FORT_event;
    return 0;
  }
}

void show_dict_() { hipo_FORT_dict->show(); }

int get_rows_(int* scheme) { return hipo_bank_map.find(*scheme)->second->getRows(); }

int add_schema_(char* name, int length) {
  char* buffer = (char*)malloc(length + 1);
  memcpy(buffer, name, length);
  buffer[length] = '\0';
  int __name     = std::hash<std::string>()(buffer);
  hipo_bank_map.insert(std::make_pair(__name, new hipo::bank(hipo_FORT_dict->getSchema(buffer))));
  free(buffer);
  return __name;
}

void get_int_node_(int* scheme, int* bank, int* buffer) {
  int length = hipo_bank_map.find(*scheme)->second->getRows();
  for (int i = 0; i < length; i++) {
    buffer[i] = hipo_bank_map.find(*scheme)->second->getInt(*bank, i);
  }
}

void get_float_node_(int* scheme, int* bank, float* buffer) {
  int length = hipo_bank_map.find(*scheme)->second->getRows();
  for (int i = 0; i < length; i++) {
    buffer[i] = hipo_bank_map.find(*scheme)->second->getFloat(*bank, i);
  }
}
}
