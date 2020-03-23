/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "hipo4/dictionary.h"
#include "hipo4/utils.h"
#include <stdlib.h>

namespace hipo {

  void schema::parse(std::string schString) {
    std::vector<std::string> entries;
    std::vector<std::string> entry;
    hipo::utils::tokenize(schString, entries, ",");
    int offset = 0;
    for (int i = 0; i < entries.size(); i++) {
      entry.clear();
      hipo::utils::tokenize(entries[i], entry, "/");
      schemaEntry_t e;
      e.name     = entry[0];
      e.type     = entry[1];
      e.typeId   = getTypeByString(e.type);
      e.typeSize = getTypeSize(e.typeId);
      e.offset   = offset;
      offset += e.typeSize;
      schemaEntries.push_back(e);
      schemaEntriesMap[e.name] = i;
    }
  }

  int schema::getTypeByString(std::string& typeName) {
    if (typeName == "B") {
      return 1;
    } else if (typeName == "S") {
      return 2;
    } else if (typeName == "I") {
      return 3;
    } else if (typeName == "F") {
      return 4;
    } else if (typeName == "D") {
      return 5;
    } else if (typeName == "L") {
      return 8;
    }
    return -1;
  }

  int schema::getTypeSize(int id) {
    switch (id) {
    case 1:
      return 1;
    case 2:
      return 2;
    case 3:
      return 4;
    case 4:
      return 4;
    case 5:
      return 8;
    case 8:
      return 8;
    default:
      return 0;
    }
    return 0;
  }

  void schema::show() {
    printf("schema : %14s , group = %6d, item = %3d\n", schemaName.c_str(), groupid, itemid);
    for (int i = 0; i < schemaEntries.size(); i++) {
      printf("%16s : (%3s) %5d %5d , offset = %3d\n", schemaEntries[i].name.c_str(),
             schemaEntries[i].type.c_str(), schemaEntries[i].typeId, schemaEntries[i].typeSize,
             schemaEntries[i].offset);
    }
  }

  std::string schema::json() {
    std::string out;
    out += "{\"name\": \"";
    out += schemaName;
    out += "\",";
    out += "\"entries\":[ ";
    for (int i = 0; i < schemaEntries.size(); i++) {
      out += "{";
      out += "\"name\" : \"";
      out += schemaEntries[i].name;
      out += "\" , \"id\" : \"";
      out += std::to_string(getEntryOrder(schemaEntries[i].name));
      out += "\" , \"type\" : \"";
      out += schemaEntries[i].type;
      out += (i == schemaEntries.size() - 1) ? ("\" }") : ("\" },");
    }
    out += "]";
    out += "}";
    return out;
  }

  int schema::getOffset(int item, int order, int rows) {
    int offset = rows * schemaEntries[item].offset + order * schemaEntries[item].typeSize;
    return offset;
  }
  int schema::getOffset(const char* name, int order, int rows) {
    int item = schemaEntriesMap[name];
    return getOffset(item, order, rows);
  }

  int schema::getEntryOrder(std::string name) { return schemaEntriesMap[name]; }
  int schema::getSizeForRows(int rows) {
    int nentries = schemaEntries.size();
    int offset   = getOffset(nentries - 1, rows - 1, rows) + schemaEntries[nentries - 1].typeSize;
    return offset;
  }

  int schema::getRowLength() {
    int nentries = schemaEntries.size();
    int size     = schemaEntries[nentries - 1].offset + schemaEntries[nentries - 1].typeSize;
    return size;
  }

  std::string schema::getSchemaString() {
    char        parts[256];
    std::string result;
    sprintf(parts, "{%s/%d/%d}{", schemaName.c_str(), groupid, itemid);
    result.append(parts);
    for (int loop = 0; loop < schemaEntries.size(); loop++) {
      sprintf(parts, "%s/%s", schemaEntries[loop].name.c_str(), schemaEntries[loop].type.c_str());
      if (loop != 0)
        result.append(",");
      result.append(parts);
    }
    result.append("}");
    return result;
  }

  std::string schema::getSchemaStringJson() {
    char        parts[256];
    std::string result;
    sprintf(parts, "{ \"name\": \"%s\", \"group\": %d, \"item\": %d, \"info\": \" \",",
            schemaName.c_str(), groupid, itemid);
    result.append(parts);
    result.append("\"entries\": [ ");
    for (int loop = 0; loop < schemaEntries.size(); loop++) {
      sprintf(parts, "{\"name\":\"%s\", \"type\":\"%s\", \"info\":\" \"}",
              schemaEntries[loop].name.c_str(), schemaEntries[loop].type.c_str());
      if (loop != 0)
        result.append(",");
      result.append(parts);
    }
    result.append("] }");
    return result;
  }

  //=============================================
  // Implementation of dictionary class
  //=============================================
  std::vector<std::string> dictionary::getSchemaList() {
    std::vector<std::string>                          vec;
    for (auto && it : factory) {
      vec.push_back(it.first);
    }
    return vec;
  }

  bool dictionary::parse(const char* schemaString) {
    std::vector<std::string> tokens;
    std::string              schemahead = hipo::utils::substring(schemaString, "{", "}", 0);
    hipo::utils::tokenize(schemahead, tokens, "/");
    int          group = std::atoi(tokens[1].c_str());
    int          item  = std::atoi(tokens[2].c_str());
    hipo::schema schema(tokens[0].c_str(), group, item);
    std::string  schemabody = hipo::utils::substring(schemaString, "{", "}", 1);
    schema.parse(schemabody.c_str());
    addSchema(schema);
    return true;
  }

  void dictionary::show() {
    std::vector<std::string> list = getSchemaList();
    for (int i = 0; i < list.size(); i++) {
      schema sc = getSchema(list[i].c_str());
      printf("%24s : %5d %5d %5d\n", sc.getName().c_str(), sc.getGroup(), sc.getItem(),
             sc.getEntries());
    }
  }
} // namespace hipo
