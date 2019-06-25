#include "TChain.h"
#include "TFile.h"
#include "TROOT.h"
#include <chrono>
#include <iostream>

int main(int argc, char const* argv[]) {
  short                    n_files = 10;
  std::vector<std::string> fin;
  std::string              base_name;

  if (argc < 3) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\tsplit_clas12 num_files base_name input_file*.root" << std::endl;
    exit(1);
  } else {
    n_files   = atoi(argv[1]);
    base_name = argv[2];
    for (int i = 3; i < argc; i++)
      fin.push_back(argv[i]);
  }

  auto chain = std::make_shared<TChain>("clas12");
  for (auto f : fin)
    chain->AddFile(f.c_str());

  size_t num_of_events = (size_t)chain->GetEntries();

  std::vector<std::unique_ptr<TTree>> skim(n_files);
  for (int i = 0; i < n_files; i++)
    skim[i] = static_cast<std::unique_ptr<TTree>>(chain->CloneTree(0));

  ROOT::EnableImplicitMT(n_files);
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    chain->GetEntry(current_event);
    skim[current_event % n_files]->Fill();
  }
  ROOT::DisableImplicitMT();

  std::vector<std::unique_ptr<TFile>> RootOutputFile(n_files);

  for (int i = 0; i < n_files; i++) {
    base_name = base_name.substr(0, base_name.find(".root"));
    base_name += "_" + std::to_string(i) + ".root";
    RootOutputFile[i] = std::make_unique<TFile>(base_name.c_str(), "RECREATE");
    RootOutputFile[i]->cd();
    skim[i]->Write();
    RootOutputFile[i]->Write();
    RootOutputFile[i]->Close();
  }

  return 0;
}
