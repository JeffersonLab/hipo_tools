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
  std::vector<string> out_banks;
  string code_file_out   = "myAnalysis.cxx";
  bool   is_convert_cmd = false;
  bool   is_list_cmd = false;
  bool   is_dump_cmd = false;
  bool   is_code_cmd = false;
  bool   is_help_cmd = false;
  bool   all_banks = false;


  auto code_cli = (
    command("code").set(is_code_cmd) % "list the banks",
    option("-c","--code") & value("codefile", code_file_out) % "output code file",
    (
      required("-b", "--banks") & values("banks",out_banks) % "banks which should be used." |
      required("-a", "--all-banks").set(all_banks) % "all banks",
      option("-i","--input")  & value("input file" , infile) 
    )
    );

  auto list_cli = (
    command("list").set(is_list_cmd) % "list the banks",
    required("-i","--input")  & value("input file" , infile) % "input file" ,
    option("-b", "--bank") & values("banks",out_banks) % "banks which should be used."
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
    code_cli |
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


  //cout << "converting " << in_path  << "\n";
  //cout << "to         " << out_path << "\n";

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
  //for(int i = 0; i < out_banks.size(); i++){
  //  if(dict->hasSchema(out_banks[i].c_str())==true){
  //  }
  //  }

  hipo::reader hip_reader;
  hip_reader.open(in_path.string().c_str());
  hip_reader.showInfo();
  hipo::dictionary *dict = hip_reader.getSchemaDictionary();

  if(is_list_cmd) {
    if(out_banks.size() == 0){
      dict->ls();
    } else {
      // get list of all the banks and search for the containing string
      auto all_banks = dict->getSchemaList();
      for(const auto& an_out_bank : out_banks ){
        for(const auto& abank : all_banks ){
          if (abank.find(an_out_bank) != std::string::npos) {
            std::cout << abank << "\n";
          }
        }
      }
    }
  }
  //if(str_command.compare("--dump")==0){
  //  dict->ls(1);
  //}

  if( is_code_cmd ) {
    std::vector<std::string> code_nodes;
    if(all_banks==true){
      //banks.clear();
      code_nodes = dict->getSchemaList();
    } else {
      // get list of all the banks and search for the containing string
      auto all_banks = dict->getSchemaList();
      for(const auto& an_out_bank : out_banks ){
        for(const auto& abank : all_banks ){
          if (abank.find(an_out_bank) != std::string::npos) {
            code_nodes.push_back(abank);
            //std::cout << abank << "\n";
          }
        }
      }
    }
    //if(out_banks.size()==0){
    //  printf("[--code] error : please provide bank names..\n\n");
    //} else {
    
    fs::path out_path = code_file_out;
    if(out_path.extension().empty()) {
      out_path.replace_extension(".cxx");
    }

    std::cout << "[code] ----> starting code generation....\n";
    std::cout << "[open] ----> writing file : " << code_file_out << "\n";
    ofstream my_cc_file (out_path);
    my_cc_file << hipo::utils::getFileHeader(out_path.stem().string()).c_str() << std::endl;


    for(int i = 0; i < code_nodes.size(); i++){
      if(dict->hasSchema(code_nodes[i].c_str())==true){
        std::vector<std::string>  code = dict->getSchema(code_nodes[i].c_str()).branchesCode();

        for(int v = 0; v < code.size(); v++){
          //printf("%s\n",code[v].c_str());
          my_cc_file << code[v].c_str() << std::endl;
        }
      } else {
        std::cout << "// ** error ** can not find bank with name [" << code_nodes[i].c_str() <<"]\n";
      }
    }
    std::vector<std::string>  access_code = dict->getSchema(code_nodes[0].c_str()).branchesAccessCode();
    //printf("%s\n",hipo::utils::getFileTrailer(access_code[0].c_str()).c_str());
    my_cc_file << hipo::utils::getFileTrailer(access_code[0].c_str()).c_str() << std::endl;
    my_cc_file.close();
    //}
  }

  if(is_dump_cmd) {

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

    std::cout << " UNABLE TO WRITE FROM HIPO FILES FROM C++\n";
    std::cout << " Please make a request for support by emailing:\n";   
    std::cout << " clas12_software@jlab.org\n";
    std::cout << "   or create a new issue on github \n";
    std::cout << " https://github.com/JeffersonLab/hipo_tools\n";   

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
