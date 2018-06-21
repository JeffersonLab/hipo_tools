/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   text.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef TEXT_H
#define TEXT_H

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <vector>

#include "utils.h"

namespace text {

  class reader {
  private:

    std::vector<std::string> textLines;
    std::string delim;
    std::ifstream inputStream;
    hipo::utils   stringUtils;

  public:

    reader();
    ~reader();

    void open(const char *file);
    void setDalim(std::string &d);
    bool readLine();
    std::vector<int>  getIntVector();
    std::string getLine();

  };

}

#endif /* TEXT_H */
