/*
 * File:   read_file.cpp
 * Author: gavalian
 *
 * Created on April 11, 2017, 4:30 PM
 */
#include <cstdlib>
#include <iostream>

#include "clipp.h"

#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

#include "hipo/reader.h"
#include "hipo/writer.h"
#include "hipo/event.h"
#include "hipo/node.h"
#include "hipo/text.h"
#include "hipo/data.h"
#include "hipo/dictionary.h"

using namespace std;
using namespace clipp; using std::cout; using std::string;

void printUsage(){

}

/*
 *
 */
int main(int argc, char** argv) {
  bool   rec       = false;
  string infile    = "";
  string outfile   = "";
  bool   force_out = false;
  int    N_events  = 0;
  bool   help      = false;
  bool   verbose   = false;
  string out_banks   = "";
  bool   is_list_cmd = false;
  bool   is_dump_cmd = false;
  bool   is_help_cmd = false;


  auto list_cli = (
    command("list").set(is_list_cmd) % "list the banks",
    option("-b", "--bank") & value("banks",out_banks) % "banks which should be used."
    );

  auto dump_cli = (
    command("dump").set(is_dump_cmd) % "list the banks",
    option("-i","--input")  & value("input file" , infile) ,
    option("-o","--output") & value("output file", outfile),
    // banks should be made repeatable
    option("-b", "--bank") & value("banks",out_banks) % "banks which should be used.",
    // todo: add starting event number
    option("-N", "--Nevents") & value("Nevents",N_events) % "Number of events to output."
    );
  auto help_cli = (
    command("help").set(is_help_cmd) % "print help"
    );


  auto cli = (
    list_cli | 
    dump_cli | 
    help_cli | 
    option("-h","--help").set(help) % "print help"
    );

  if (!parse(argc, argv, cli)) {
    cout << make_man_page(cli, argv[0]);
  }

  if(help || is_help_cmd){
    cout << make_man_page(cli, argv[0]);
    return 0;
  }

  fs::path in_path = infile;

  if (!fs::exists(in_path)) {
    std::cerr << "File : " << infile << " not found.\n";
    return -127;
  }

  if (in_path.extension() != fs::path(".hipo")) {
    std::cerr << "File : " << infile << " not a hipo file.\n";
    return -127;
  }

  fs::path out_path = outfile;
  if (outfile.empty()) {
    out_path = in_path;
    out_path.replace_extension(".root");
    outfile = out_path.string();
  }
  if (out_path.extension() != fs::path(".root")) {
    out_path.replace_extension(".root");
    outfile = out_path.string();
  }

  if (fs::exists(out_path)) {
    if(force_out){
      std::cout << out_path << " will be overwritten\n";
    } else {
      std::cerr << "error: file " << out_path << " exists (use -f to force overwrite).\n";
      return -127;
    }
  }

  cout << "converting " << in_path  << "\n";
  cout << "to         " << out_path << "\n";


    //char command[128];
    //char filename[128];
    //char bankname[128];
    //bool doAll = false;

    //std::vector<std::string>  banks;
    //std::string str_command;

    //if(argc>2){
    //    sprintf( command,"%s",argv[1]);
    //    sprintf( filename,"%s",argv[2]);
    //    str_command = command;
    //    if(argc>3){
    //      sprintf(bankname,"%s",argv[3]);
    //      std::string b_str(bankname);
    //      if(b_str.compare("all")==0) doAll = true;
    //      if(doAll==false){
    //        int nbanks = argc;
    //        for(int i = 3; i < nbanks; i++){
    //          sprintf(bankname,"%s",argv[i]);
    //          banks.push_back(std::string(bankname));
    //        }
    //      }
    //    }
    //} else {
    //  printf(" \n\n Please provide a filename to read....\n\n");
    //  exit(0);
    //}


  hipo::reader hip_reader;
  hip_reader.open(in_path.string().c_str());
  //hip_reader.showInfo();
  //hipo::dictionary *dict = hip_reader.getSchemaDictionary();

    //if(str_command.compare("--list")==0){
    //  dict->ls();
    //}

    //if(str_command.compare("--dump")==0){
    //  dict->ls(1);
    //}

    //if(str_command.compare("--code")==0){
    //  printf("\n\n");

    //  if(banks.size()==0&&doAll==false){
    //    printf("[--code] error : please provide bank names..\n\n");
    //  } else {
    //    printf("[code] ----> starting code generation....\n");
    //    printf("[open] ----> writing file : runFileLoop.cc\n");
    //    ofstream my_cc_file ("runFileLoop.cc");
    //    my_cc_file << hipo::utils::getFileHeader().c_str() << std::endl;

    //    if(doAll==true){
    //      banks.clear();
    //      banks = dict->getSchemaList();
    //    }

    //    for(int i = 0; i < banks.size(); i++){
    //      if(dict->hasSchema(banks[i].c_str())==true){
    //        std::vector<std::string>  code = dict->getSchema(banks[i].c_str()).branchesCode();

    //        for(int v = 0; v < code.size(); v++){
    //          //printf("%s\n",code[v].c_str());
    //          my_cc_file << code[v].c_str() << std::endl;
    //        }
    //      } else {
    //        printf("// ** error ** can not find bank with name [%s]\n",banks[i].c_str());
    //      }
    //    }
    //    std::vector<std::string>  access_code = dict->getSchema(banks[0].c_str()).branchesAccessCode();
    //    //printf("%s\n",hipo::utils::getFileTrailer(access_code[0].c_str()).c_str());
    //    my_cc_file << hipo::utils::getFileTrailer(access_code[0].c_str()).c_str() << std::endl;
    //    my_cc_file.close();
    //  }
    //}


    if(is_dump_cmd) {
      hipo::writer hip_writer;
      hip_writer.open(out_path.string().c_str());

      hipo::record record;
      hipo::event  event;

      int nrecords = hip_reader.getRecordCount();
      int ecounter = 0;

      std::cout << "  N events : " << N_events << "\n";
      std::cout << "  nrecords : " << nrecords << "\n";
      while( hip_reader.next() ) {
        if( (N_events>0) && (ecounter >= N_events) ) {
          break;
        }

        event.showInfo();
        event = *(hip_reader.getEvent());
        hip_writer.writeEvent(event);
        //printf(" event # %d\n",ecounter);
        ecounter++;
      }
      hip_writer.close();
    }


    return 0;
}
