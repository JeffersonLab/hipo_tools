/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   read_file.cpp
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:30 PM
 */

#include <cstdlib>
#include <iostream>

#include "hipo3/data.h"
#include "hipo3/dictionary.h"
#include "hipo3/event.h"
#include "hipo3/node.h"
#include "hipo3/reader.h"
#include "hipo3/text.h"
#include "hipo3/writer.h"

using namespace std;

void printUsage() {}

/*
 *
 */
int main(int argc, char** argv) {

  std::cout << "start file reading test" << '\n';

  char command[128];
  char filename[128];
  char bankname[128];
  bool doAll = false;

  std::vector<std::string> banks;
  std::string              str_command;

  if (argc > 2) {
    sprintf(command, "%s", argv[1]);
    sprintf(filename, "%s", argv[2]);
    str_command = command;
    if (argc > 3) {
      sprintf(bankname, "%s", argv[3]);
      std::string b_str(bankname);
      if (b_str.compare("all") == 0)
        doAll = true;
      if (doAll == false) {
        int nbanks = argc;
        for (int i = 3; i < nbanks; i++) {
          sprintf(bankname, "%s", argv[i]);
          banks.push_back(std::string(bankname));
        }
      }
    }
  } else {
    printf(" \n\n Please provide a filename to read....\n\n");
    exit(0);
  }

  hipo::reader reader;
  reader.open(filename);
  hipo::dictionary* dict = reader.getSchemaDictionary();

  if (str_command.compare("--list") == 0) {
    dict->ls();
  }

  if (str_command.compare("--dump") == 0) {
    dict->ls(1);
  }

  if (str_command.compare("--code") == 0) {
    printf("\n\n");

    if (banks.size() == 0 && doAll == false) {
      printf("[--code] error : please provide bank names..\n\n");
    } else {
      printf("[code] ----> starting code generation....\n");
      printf("[open] ----> writing file : runFileLoop.cc\n");
      ofstream my_cc_file("runFileLoop.cc");
      my_cc_file << hipo::utils::getFileHeader("runFileLoop").c_str() << std::endl;

      if (doAll == true) {
        banks.clear();
        banks = dict->getSchemaList();
      }

      for (int i = 0; i < banks.size(); i++) {
        if (dict->hasSchema(banks[i].c_str()) == true) {
          std::vector<std::string> code = dict->getSchema(banks[i].c_str()).branchesCode();

          for (int v = 0; v < code.size(); v++) {
            // printf("%s\n",code[v].c_str());
            my_cc_file << code[v].c_str() << std::endl;
          }
        } else {
          printf("// ** error ** can not find bank with name [%s]\n", banks[i].c_str());
        }
      }
      std::vector<std::string> access_code = dict->getSchema(banks[0].c_str()).branchesAccessCode();
      // printf("%s\n",hipo::utils::getFileTrailer(access_code[0].c_str()).c_str());
      my_cc_file << hipo::utils::getFileTrailer(access_code[0].c_str()).c_str() << std::endl;
      my_cc_file.close();
      printf("[open] ----> writing file : Sconstruct\n");
      ofstream my_scons_file("SConstruct");
      my_scons_file << hipo::utils::getSConstruct().c_str();
      my_scons_file.close();
      printf("\n\ndone..\n");
    }
  }

  return 0;
}
