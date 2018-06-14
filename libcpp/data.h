/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   node.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef DATA_H
#define DATA_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <vector>
#include <stdint.h>
#include <stdlib.h>

namespace data {

  class data {
  private:

  public:
    data();
    ~data();

    static void print(const std::vector<int> &vec);
    static void print(const std::vector<uint16_t> &vec);

    void  getVector(std::vector<int> &pulse, std::vector<char> &encoded);
    void  decompose(std::vector<int> &pulse, std::vector<uint16_t> &low,
                    std::vector<uint16_t> &high);

    void  encode(std::vector<int> &pulse, std::vector<char> &dest);
    void  encodeLossy(std::vector<int> &pulse, std::vector<char> &dest);
    void  decode(std::vector<char> &dest, std::vector<uint16_t> &pulse);

    int getMinimum(const std::vector<int> &vec);


    std::vector<int> getSubtracted(const std::vector<int> &vec);
    std::vector<int> getReiman(const std::vector<int> &vec);
    std::vector<int> getReduced(const std::vector<int> &vec);
    std::vector<char> getLowerHalf(std::vector<int> &vec);
  };

}

#endif /* UTILS_H */
