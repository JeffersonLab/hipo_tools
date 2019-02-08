/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   utils.h
 * Author: gavalian
 *
 * Created on April 27, 2017, 10:01 AM
 */

#ifndef UTILS_H
#define UTILS_H

#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

namespace hipo {

  class utils {
  private:
  public:
    utils();
    ~utils();
    static void tokenize(const std::string& str, std::vector<std::string>& tokens,
                         const std::string& delimiters = " ");

    static std::string substring(const std::string& str, const char* start_delim,
                                 const char* end_delim, int order);

    static int findposition(const std::string& str, const char* delim, int order);

    static void writeInt(char* buffer, int position, int value);
    static void writeLong(char* buffer, int position, long value);
    static void writeByte(char* buffer, int position, uint8_t value);
  };

} // namespace hipo

#endif /* UTILS_H */
