#include <iostream>
#include "clipp.h"
#include "THipo.h"
#ifdef __cpp_lib_filesystem
#include <filesystem>
namespace fs = std::filesystem;
#else
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#endif

using namespace clipp; using std::cout; using std::string;

int main(int argc, char* argv[]) {
  bool   rec = false, utf16 = false;
  string infile = "", outfile = "";
  bool force_out = false;

  auto cli = ((value("input file", infile)) | (option("-i") & value("input file", infile)) ,
              option("-o") & value("output file", outfile),
              option("-f").set(force_out) % "force the output to overwrite existing files",
              option("-utf16").set(utf16).doc("use UTF-16 encoding"));

  if (!parse(argc, argv, cli)) {
    cout << make_man_page(cli, argv[0]);
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

  hipo::THipo h;
  h.ToRoot(in_path.string() ,
           out_path.string() );
  return 0;
}
