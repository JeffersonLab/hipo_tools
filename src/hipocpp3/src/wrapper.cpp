#include "hipo3/event.h"
#include "hipo3/reader.h"
#include <iostream>

hipo::reader hipo_FORT_Reader;
hipo::record hipo_FORT_Record;
hipo::record hipo_FORT_Record_Dictionary;
hipo::event  hipo_FORT_Event;

extern "C" {

void hipo_open_file_(int* nrecords, const char* filename, int length) {
  char* buffer = (char*)malloc(length + 1);
  memcpy(buffer, filename, length);
  buffer[length] = '\0';
  printf("[FORTRAN] opening file : %s\n", buffer);
  hipo_FORT_Reader.open(buffer);
  hipo_FORT_Reader.showInfo();
  hipo_FORT_Reader.readHeaderRecord(hipo_FORT_Record_Dictionary);
  int dictionary_event_count = hipo_FORT_Record_Dictionary.getEventCount();
  printf("[FORTRAN] dictionary length %d\n", dictionary_event_count);
  *nrecords = hipo_FORT_Reader.getRecordCount();
  free(buffer);
}

int hipo_file_next_() {
  bool status = hipo_FORT_Reader.next();
  if (status == false)
    return 0;
  hipo_FORT_Reader.getEvent()->scanEventMap();
  return 1;
}

void get_dict_length_(int* len) { *len = hipo_FORT_Record_Dictionary.getEventCount(); }

void read_schema_(int* order, int* max_char, int* str_Length, char* entry, int entryLength) {
  int         i = *order;
  hipo::event schema;
  hipo_FORT_Record_Dictionary.readHipoEvent(schema, i);
  std::string schemaString = schema.getString(31111, 1);
  int         length       = schemaString.length();
  // printf("schema %d (%d) : %s \n",i,length,schemaString.c_str());
  for (int k = 0; k < length; k++) {
    entry[k] = schemaString[k];
  }
  int max = *max_char;
  for (int j = length; j < max; j++) {
    entry[j] = ' ';
  }
  *str_Length = length;
}

void hipo_read_record_(int* record, int* n_events) {
  int pos = (*record) - 1;
  hipo_FORT_Reader.readRecord(hipo_FORT_Record, pos);
  int nevt  = hipo_FORT_Record.getEventCount();
  *n_events = nevt;
}

void hipo_read_event_(int* n_event) {
  int event_index = (*n_event) - 1;
  // std::vector<char> record_event;
  // hipo_FORT_Record.readEvent(record_event,event_index);
  hipo_FORT_Record.readHipoEvent(hipo_FORT_Event, event_index);
}

void hipo_read_node_float_(int* group, int* item, int* nread, float* buffer) {
  int                id_g = *group;
  int                id_i = *item;
  std::vector<float> vec  = hipo_FORT_Reader.getEvent()->getFloat(id_g, id_i);
  for (int i = 0; i < vec.size(); i++) {
    buffer[i] = vec[i];
  }
  *nread = vec.size();
}

void hipo_read_node_int_(int* group, int* item, int* nread, int* buffer) {
  int              id_g = *group;
  int              id_i = *item;
  std::vector<int> vec  = hipo_FORT_Reader.getEvent()->getInt(id_g, id_i);
  for (int i = 0; i < vec.size(); i++) {
    buffer[i] = vec[i];
  }
  *nread = vec.size();
}
}
