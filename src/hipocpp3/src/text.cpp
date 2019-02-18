/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo3/text.h"

namespace text {
  reader::reader() { delim = " "; }
  reader::~reader() {}

  void reader::open(const char* file) { inputStream.open(file); }

  bool reader::readLine() {
    std::string str;
    std::getline(inputStream, str);
    textLines.clear();
    stringUtils.tokenize(str, textLines, delim);
    // textLines.push_back(str);
    if (inputStream.eof())
      return false;
    return true;
  }

  void reader::setDalim(std::string& d) { delim = d; }

  std::string reader::getLine() {
    if (textLines.size() == 0)
      return "";
    return textLines[0];
  }

  std::vector<int> reader::getIntVector() {
    std::vector<int> intVec;
    for (int i = 0; i < textLines.size(); i++) {
      intVec.push_back(atoi(textLines[i].c_str()));
    }
    return intVec;
  }
} // namespace text
