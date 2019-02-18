/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo4/utils.h"

namespace hipo {

  utils::utils() {}
  utils::~utils() {}

  void utils::tokenize(const std::string& str, std::vector<std::string>& tokens,
                       const std::string& delimiters) {
    // Skip delimiters at beginning.
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::string::size_type pos = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos) {
      // Found a token, add it to the vector.
      tokens.push_back(str.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = str.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = str.find_first_of(delimiters, lastPos);
    }
  }
  /**
   * finds postion of the dalim in the string, while skipping "order" times.
   */
  int utils::findposition(const std::string& str, const char* delim, int order) {
    int                    found    = 0;
    int                    position = 0;
    std::string::size_type firstPos = str.find_first_of(delim, position);
    if (firstPos == std::string::npos)
      return -1;
    position = firstPos;
    while (found != order) {
      firstPos = str.find_first_of(delim, position + 1);
      if (firstPos == std::string::npos)
        return -1;
      position = firstPos;
      found++;
    }
    return position;
  }
  /**
   * returns a substring from a string that is enclosed between
   * start_delim and end_delim. order variable decides which enclosed
   * string to return as and order of occurance. 0 - first one. 1 - second one.
   */
  std::string utils::substring(const std::string& str, const char* start_delim,
                               const char* end_delim, int order) {
    int position = 0;
    int firstPos = hipo::utils::findposition(str, start_delim, order);
    if (firstPos < 0)
      return std::string();
    std::string::size_type lastPos = str.find_first_of(end_delim, firstPos + 1);
    if (lastPos == std::string::npos)
      return std::string();
    int length = lastPos - firstPos - 1;
    return str.substr(firstPos + 1, length);
  }

  void utils::writeInt(char* buffer, int position, int value) {
    int* ptr = reinterpret_cast<int*>(&buffer[position]);
    *ptr     = value;
  }

  void utils::writeLong(char* buffer, int position, long value) {
    int64_t* ptr = reinterpret_cast<int64_t*>(&buffer[position]);
    *ptr         = value;
  }

  void utils::writeByte(char* buffer, int position, uint8_t value) {
    int8_t* ptr = reinterpret_cast<int8_t*>(&buffer[position]);
    *ptr        = value;
  }

} // namespace hipo
