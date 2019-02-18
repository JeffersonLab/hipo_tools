/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo3/data.h"

namespace data {

  data::data() {}
  data::~data() {}

  void data::print(const std::vector<int>& vec) {
    int n = vec.size();
    for (int i = 0; i < n; i++)
      printf("%d  ", vec[i]);
    printf("\n");
  }

  void data::print(const std::vector<uint16_t>& vec) {
    int n = vec.size();
    for (int i = 0; i < n; i++)
      printf("%d  ", (unsigned int)vec[i]);
    printf("\n");
  }

  int data::getMinimum(const std::vector<int>& vec) {
    int min = vec[0];
    for (int i = 1; i < vec.size(); i++) {
      if (vec[i] < min)
        min = vec[i];
    }
    return min;
  }

  std::vector<int> data::getSubtracted(const std::vector<int>& vec) {
    int              min = getMinimum(vec);
    std::vector<int> svec;
    for (int i = 0; i < vec.size(); i++) {
      svec.push_back(vec[i] - min);
    }
    return svec;
  }

  std::vector<int> data::getReiman(const std::vector<int>& vec) {
    std::vector<int> v;
    v.push_back(vec[0]);
    for (int i = 1; i < vec.size(); i++) {
      int prev = v[i - 1];
      v.push_back(prev + vec[i]);
    }
    return v;
  }

  std::vector<int> data::getReduced(const std::vector<int>& vec) {
    // int bits = 0x000001F0;
    int bits = 0x00000007;

    std::vector<int> v;
    int              item = (vec[0] & bits);
    v.push_back(item);
    for (int i = 1; i < vec.size(); i++) {
      item = vec[i] - vec[i - 1];
      item = (item & bits);
      v.push_back(item);
    }
    return v;
  }

  std::vector<char> data::getLowerHalf(std::vector<int>& vec) {
    std::vector<char> v;
    int               minimum = getMinimum(vec);
    v.resize(52);

    uint16_t* ped          = reinterpret_cast<uint16_t*>(&v[0]);
    *ped                   = minimum;
    std::vector<int> vsub  = getSubtracted(vec);
    std::vector<int> vreim = getReiman(vsub);
    std::vector<int> vred  = getReduced(vreim);

    for (int i = 0; i < vec.size(); i++) {
      int   offset = (int)i / 2;
      int   odd    = i % 2;
      char* ptr    = &v[offset + 2];
      char  moment = *ptr;
      // printf(" moment before = %d (%d) \n", moment);
      if (odd == 0) {
        moment = moment | (0x07 & vec[i]);
      } else {
        moment = moment | ((vec[i] << 4) & 0x70);
      }
      // printf(" moment after = %d\n",moment);
      *ptr = moment;
    }
    return v;
  }

  void data::decode(std::vector<char>& dest, std::vector<uint16_t>& pulse) {}

  void data::encode(std::vector<int>& pulse, std::vector<char>& dest) {
    std::vector<uint16_t> low;
    std::vector<uint16_t> high;

    decompose(pulse, low, high);

    int minimum = pulse[0];
    for (int i = 1; i < pulse.size(); i++) {
      if (pulse[i] < minimum)
        minimum = pulse[i];
    }
    dest.clear();
    dest.resize(2 + low.size() / 2, 0);
    uint16_t* ped = reinterpret_cast<uint16_t*>(&dest[0]);
    *ped          = minimum;
    for (int i = 0; i < low.size(); i++) {
      int  index = 2 + i / 2;
      int  order = i % 2;
      char value = dest[index];
      char ibyte = low[i];
      // printf("value before %X  %d %d\n", (unsigned int) value, order, low[i]);
      if (order == 0) {
        value = (value | (ibyte & 0x000F));
      } else {
        value = (value | ((ibyte & 0x000F) << 4));
      }
      // printf("value after %X\n", (unsigned int) value);
      // printf("\n");
      dest[index] = value;
    }

    int start_byte = 0;
    int end_byte   = high.size() - 1;
    for (int i = 0; i < high.size(); i++)
      if (high[i] != 0) {
        start_byte = i;
        break;
      };
    for (int i = high.size() - 1; i >= 0; i--)
      if (high[i] != 0) {
        end_byte = i;
        break;
      };
    // printf(" array range = %d %d\n",start_byte, end_byte);
    dest.push_back(start_byte - 1);
    dest.push_back(end_byte - start_byte + 1);
    for (int i = start_byte; i <= end_byte; i++) {
      dest.push_back(high[i]);
    }
  }

  void data::encodeLossy(std::vector<int>& pulse, std::vector<char>& dest) {
    std::vector<uint16_t> lowF;
    std::vector<uint16_t> high;

    decompose(pulse, lowF, high);

    int minimum = pulse[0];
    for (int i = 1; i < pulse.size(); i++) {
      if (pulse[i] < minimum)
        minimum = pulse[i];
    }
    dest.clear();

    std::vector<uint16_t> low;

    for (int i = 0; i < lowF.size(); i += 2) {
      uint16_t value = (uint16_t)((lowF[i] + lowF[i + 1]) / 2);
      low.push_back(value);
    }

    dest.resize(7 + low.size() / 2, 0);
    dest[0]       = 'P';
    dest[1]       = 'U';
    dest[2]       = 'L';
    dest[3]       = 'S';
    dest[4]       = 'E';
    uint16_t* ped = reinterpret_cast<uint16_t*>(&dest[5]);
    *ped          = minimum;

    for (int i = 0; i < low.size(); i++) {
      int  index = 7 + i / 2;
      int  order = i % 2;
      char value = dest[index];
      char ibyte = low[i];
      // printf("value before %X  %d %d\n", (unsigned int) value, order, low[i]);
      if (order == 0) {
        value = (value | (ibyte & 0x000F));
      } else {
        value = (value | ((ibyte & 0x000F) << 4));
      }
      // printf("value after %X\n", (unsigned int) value);
      // printf("\n");
      dest[index] = value;
    }

    int start_byte = 0;
    int end_byte   = high.size() - 1;
    for (int i = 0; i < high.size(); i++)
      if (high[i] != 0) {
        start_byte = i;
        break;
      };
    for (int i = high.size() - 1; i >= 0; i--)
      if (high[i] != 0) {
        end_byte = i;
        break;
      };
    // printf(" array range = %d %d\n",start_byte, end_byte);
    dest.push_back(start_byte - 1);
    dest.push_back(end_byte - start_byte + 1);
    for (int i = start_byte; i <= end_byte; i++) {
      dest.push_back(high[i]);
    }
  }

  void data::decompose(std::vector<int>& pulse, std::vector<uint16_t>& low,
                       std::vector<uint16_t>& high) {
    int minimum = pulse[0];
    for (int i = 1; i < pulse.size(); i++) {
      if (pulse[i] < minimum)
        minimum = pulse[i];
    }
    low.resize(pulse.size());
    high.resize(pulse.size());
    low[0] = pulse[0] - minimum;
    // high[0] = pulse[0] - minimum;
    int integral = 0;
    for (int i = 1; i < pulse.size(); i++) {
      int value = pulse[i] - minimum;
      low[i]    = low[i - 1] + value;
    }
    // printf(" REIMAN = ");
    // print(low);
    high[0] = low[0];
    for (int i = 1; i < pulse.size(); i++) {
      high[i] = low[i] - low[i - 1];
    }
    // printf(" SEQUENCE = ");
    // print(high);

    for (int i = 0; i < pulse.size(); i++) {
      uint16_t value = high[i];
      high[i]        = (value & 0x1FF0) >> 4;
      low[i]         = value & 0x000F;
    }
  }

  void data::getVector(std::vector<int>& pulse, std::vector<char>& encoded) {
    int minimum = pulse[0];
    for (int i = 1; i < pulse.size(); i++) {
      if (pulse[i] < minimum)
        minimum = pulse[i];
    }
    /*
    std::vector<uint16_t> buffer;
    buffer.resize(pulse.size());
    buffer[0] = pulse[i] - minimum;
    int integral = buffer[0];
    for(int i = 1 ; i < pulse.size(); i++){
        integral = integral + (pulse[i] -  minimum);

    }*/
  }

} // namespace data
