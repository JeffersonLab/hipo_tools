/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo3/event.h"

namespace hipo {

  event::event() {
    reset();
    // printf("creating event class.....\n");
    // hipo::node<int> *type = new hipo::node<int>();
    // nodes.push_back(type);
  }

  event::~event() {}

  hipo::node<int>* event::getIntNode(int group, int item) {
    int size             = nodes.size();
    int key              = ((0x00000000 | group) << 16) | ((0x00000000 | item) << 8);
    registeredNodes[key] = size;

    hipo::node<int>* type = new hipo::node<int>(group, item);
    nodes.push_back(type);
    return type;
  }

  void event::init(std::vector<char>& buffer) {
    dataBuffer.resize(buffer.size());
    std::memcpy(&dataBuffer[0], &buffer[0], buffer.size());
    scanEvent();
  }

  void event::init(const char* buffer, int size) {
    if (dataBuffer.size() <= size) {
      dataBuffer.resize(size);
    }
    std::memcpy(&dataBuffer[0], buffer, size);
    *(reinterpret_cast<uint32_t*>(&dataBuffer[8])) = size;
    scanEvent();
  }

  void event::appendNode(int group, int item, std::string& vec) {
    int size     = dataBuffer.size();
    int datasize = vec.length();
    dataBuffer.resize(size + datasize + 8, 0);
    uint16_t* group_ptr  = reinterpret_cast<uint16_t*>(&dataBuffer[size]);
    uint8_t*  item_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 2]);
    uint8_t*  type_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 3]);
    uint32_t* length_ptr = reinterpret_cast<uint32_t*>(&dataBuffer[size + 4]);
    *group_ptr           = group;
    *item_ptr            = item;
    *type_ptr            = 6;
    *length_ptr          = datasize;
    std::memcpy(&dataBuffer[size + 8], (char*)&vec[0], datasize);
  }

  void event::resetNodes() {
    for (int i = 0; i < nodes.size(); i++) {
      nodes[i]->length(0);
    }
  }

  void event::appendNode(int group, int item, std::vector<int8_t>& vec) {
    int size     = dataBuffer.size();
    int datasize = vec.size() * sizeof(int8_t);

    dataBuffer.resize(size + datasize + 8, 0);
    uint16_t* group_ptr  = reinterpret_cast<uint16_t*>(&dataBuffer[size]);
    uint8_t*  item_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 2]);
    uint8_t*  type_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 3]);
    uint32_t* length_ptr = reinterpret_cast<uint32_t*>(&dataBuffer[size + 4]);
    *group_ptr           = group;
    *item_ptr            = item;
    *type_ptr            = 2;
    *length_ptr          = datasize;
    std::memcpy(&dataBuffer[size + 8], (char*)&vec[0], datasize);
  }

  void event::appendNode(int group, int item, std::vector<int16_t>& vec) {
    int size     = dataBuffer.size();
    int datasize = vec.size() * sizeof(int16_t);

    dataBuffer.resize(size + datasize + 8, 0);
    uint16_t* group_ptr  = reinterpret_cast<uint16_t*>(&dataBuffer[size]);
    uint8_t*  item_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 2]);
    uint8_t*  type_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 3]);
    uint32_t* length_ptr = reinterpret_cast<uint32_t*>(&dataBuffer[size + 4]);
    *group_ptr           = group;
    *item_ptr            = item;
    *type_ptr            = 2;
    *length_ptr          = datasize;
    std::memcpy(&dataBuffer[size + 8], (char*)&vec[0], datasize);
  }

  void event::appendNode(int group, int item, std::vector<int>& vec) {
    int size     = dataBuffer.size();
    int datasize = vec.size() * sizeof(int);

    dataBuffer.resize(size + datasize + 8, 0);
    uint16_t* group_ptr  = reinterpret_cast<uint16_t*>(&dataBuffer[size]);
    uint8_t*  item_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 2]);
    uint8_t*  type_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 3]);
    uint32_t* length_ptr = reinterpret_cast<uint32_t*>(&dataBuffer[size + 4]);
    *group_ptr           = group;
    *item_ptr            = item;
    *type_ptr            = 3;
    *length_ptr          = datasize;
    std::memcpy(&dataBuffer[size + 8], (char*)&vec[0], datasize);
  }

  void event::appendNode(int group, int item, std::vector<float>& vec) {
    int size     = dataBuffer.size();
    int datasize = vec.size() * sizeof(float);

    dataBuffer.resize(size + datasize + 8, 0);
    uint16_t* group_ptr  = reinterpret_cast<uint16_t*>(&dataBuffer[size]);
    uint8_t*  item_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 2]);
    uint8_t*  type_ptr   = reinterpret_cast<uint8_t*>(&dataBuffer[size + 3]);
    uint32_t* length_ptr = reinterpret_cast<uint32_t*>(&dataBuffer[size + 4]);
    *group_ptr           = group;
    *item_ptr            = item;
    *type_ptr            = 4;
    *length_ptr          = datasize;
    std::memcpy(&dataBuffer[size + 8], (char*)&vec[0], datasize);
  }

  void event::reset() {
    dataBuffer.resize(8);
    dataBuffer[0] = 'E';
    dataBuffer[1] = 'V';
    dataBuffer[2] = 'N';
    dataBuffer[3] = 'T';
  }

  int event::getEventNode(int group, int item) {
    int position = 8;
    while (position + 8 < dataBuffer.size()) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      // printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
      if (gid == group && iid == item)
        return position;
      position += (length + 8);
    }

    return -1;
  }
  std::vector<long> event::getLong(int group, int item) {
    int               position = getEventNode(group, item);
    std::vector<long> vector;
    if (position >= 0) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      if (type == 8) {
        int iter = length;
        for (int i = 0; i < iter; i++) {
          long* ptr = reinterpret_cast<long*>(&dataBuffer[position + 8 + i * 8]);
          vector.push_back((long)*ptr);
        }
      }
    }
    return vector;
  }
  std::vector<int> event::getInt(int group, int item) {
    int              position = getEventNode(group, item);
    std::vector<int> vector;
    if (position >= 0) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));

      if (type == 1) {
        int iter = length;
        for (int i = 0; i < iter; i++) {
          int8_t* ptr = reinterpret_cast<int8_t*>(&dataBuffer[position + 8 + i]);
          vector.push_back((int)*ptr);
        }
      }

      if (type == 2) {
        int iter = length / 2;
        for (int i = 0; i < iter; i++) {
          int16_t* ptr = reinterpret_cast<int16_t*>(&dataBuffer[position + 8 + i * 2]);
          vector.push_back((int)*ptr);
        }
      }

      if (type == 3) {
        int iter = length / 4;
        for (int i = 0; i < iter; i++) {
          int* ptr = reinterpret_cast<int*>(&dataBuffer[position + 8 + i * 4]);
          vector.push_back((int)*ptr);
        }
      }
    }
    return vector;
    ;
  }

  std::string event::getString(int group, int item) {
    std::string result;
    int         position = getEventNode(group, item);
    if (position >= 0) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      if (type == 6) {
        char* string_ch = (char*)malloc(length + 1);
        std::memcpy(string_ch, &dataBuffer[position + 8], length);
        string_ch[length] = '\0';
        result            = string_ch;
      }
    }
    return result;
  }

  std::vector<float> event::getFloat(int group, int item) {
    int                position = getEventNode(group, item);
    std::vector<float> vector;
    if (position >= 0) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      if (type == 4) {
        int iter = length / 4;
        for (int i = 0; i < iter; i++) {
          float* ptr = reinterpret_cast<float*>(&dataBuffer[position + 8 + i * 4]);
          vector.push_back(*ptr);
        }
      }
    }
    return vector;
  }

  int event::getNodeAddress(int group, int item) {
    int key = ((0x00000000 | group) << 16) | ((0x00000000 | item) << 8);
    if (eventNodes.count(key) == 0)
      return -1;
    int position = eventNodes[key];
    int address  = (position)&0x00FFFFFF;
    // printf(" request for %d %d %X %X \n",group,item, position,address);
    return address;
  }

  int event::getNodeType(int address) {
    uint8_t type = *(reinterpret_cast<uint8_t*>(&dataBuffer[address + 3]));
    return type;
    // int     length = *(reinterpret_cast<int*>(&dataBuffer[position+4]));
  }

  void event::scanEvent() {
    eventNodes.clear();
    resetNodes();
    // int position = 8;
    int position  = 16;
    int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[8]));

    while (position + 8 < eventSize) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      // printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
      // if(gid==group&&iid==item) return position;

      int key  = ((0x00000000 | gid) << 16) | ((0x00000000 | iid) << 8);
      int info = ((0x00000000 | type) << 24) | (position);
      // eventNodes.insert(std::make_pair(key,info));
      // printf("map count = %d \n" ,registeredNodes.size());
      if (registeredNodes.count(key) > 0) {
        int order = registeredNodes[key];
        // nodes[order]->setType(type);
        int elements = length;
        switch (type) {
        case 2:
          elements = length / 2;
          break;
        case 3:
          elements = length / 4;
          break;
        case 4:
          elements = length / 4;
          break;
        case 5:
          elements = length / 8;
          break;
        case 8:
          elements = length / 8;
          break;
        default:
          break;
        }
        nodes[order]->length(elements);
        nodes[order]->setAddress(&dataBuffer[position + 8]);
        // printf(" found the key %d %d order = %d\n" , gid,iid, order);
      }
      // printf(" adding node : %4d %4d %X\n",gid,iid,position);
      // printf("     key = %6d , position = %6X\n",key,info);
      position += (length + 8);
    }
  }

  void event::scanEventMap() {
    eventNodes.clear();
    // resetNodes();
    // int position = 8;
    int position  = 16;
    int eventSize = *(reinterpret_cast<uint32_t*>(&dataBuffer[8]));

    while (position + 8 < eventSize) {
      uint16_t gid    = *(reinterpret_cast<uint16_t*>(&dataBuffer[position]));
      uint8_t  iid    = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 2]));
      uint8_t  type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[position + 3]));
      int      length = *(reinterpret_cast<int*>(&dataBuffer[position + 4]));
      // printf("group = %4d , item = %4d\n",(unsigned int) gid, (unsigned int) iid);
      // if(gid==group&&iid==item) return position;
      int key  = ((0x00000000 | gid) << 16) | ((0x00000000 | iid) << 8);
      int info = ((0x00000000 | type) << 24) | (position);
      eventNodes.insert(std::make_pair(key, info));
      // printf("map count = %d \n" ,registeredNodes.size());
      // printf(" adding node : %4d %4d %X\n",gid,iid,position);
      // printf("     key = %6d , position = %6X\n",key,info);
      position += (length + 8);
    }
  }

  int event::getNodeLength(int address) {
    int length = *(reinterpret_cast<int*>(&dataBuffer[address + 4]));
    return length;
  }

  int event::getNodeSize(int address) {
    uint8_t type   = *(reinterpret_cast<uint8_t*>(&dataBuffer[address + 3]));
    int     length = *(reinterpret_cast<int*>(&dataBuffer[address + 4]));
    switch (type) {
    case 2:
      return length / 2;
    case 3:
      return length / 4;
    case 4:
      return length / 4;
    case 5:
      return length / 8;
    case 8:
      return length / 8;
    default:
      return length;
    }
  }

  char* event::getNodePtr(int address) { return &dataBuffer[address + 8]; }

  std::vector<char> event::getEventBuffer() { return dataBuffer; }
  /*
  template<class T>   node<T> event::getNode(){
      node<T> en;
      en.setLength(4);
      en.setAddress(NULL);
  } */

  void event::showInfo() {
    printf(" EVENT SIGNATURE =  SIZE = %lu\n", dataBuffer.size());
    std::map<int, int>::iterator it = eventNodes.begin();
    while (it != eventNodes.end()) {
      printf(" key = %5d , info = %5d \n", it->first, it->second);
    }
  }
} // namespace hipo
