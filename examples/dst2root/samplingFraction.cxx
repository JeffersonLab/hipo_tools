#include "TCanvas.h"
#include "TChain.h"
#include "TFile.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <iostream>

std::vector<int>*   pid;
std::vector<float>* p;
std::vector<float>* ec_tot_energy;

TH2D* sf_hist = new TH2D("sf_hist", "Electron Sampling Fraction", 500, 0, 5.5, 500, 0, 0.5);

TChain* clas12 = new TChain("clas12", "clas12");

int samplingFraction(std::string file = "test.root") {
  clas12->Add(file.c_str());

  clas12->SetBranchAddress("pid", &pid);
  clas12->SetBranchAddress("p", &p);

  clas12->SetBranchAddress("ec_tot_energy", &ec_tot_energy);

  int num_of_events = (int)clas12->GetEntries();
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    clas12->GetEntry(current_event);
    if (pid->size() == 0)
      continue;
    if (pid->at(0) != 22 && pid->at(0) != 0 && p->at(0) != 0)
      sf_hist->Fill(p->at(0), ec_tot_energy->at(0) / p->at(0));
  }

  TCanvas* c1 = new TCanvas("c1", "c1");
  c1->cd();
  sf_hist->Draw("colz");

#ifndef __CLING__
  TFile* outFile = new TFile("samplingFraction_output.root", "RECREATE");
  outFile->cd();
  sf_hist->Write();
  c1->Write();
  outFile->Write();
  outFile->Close();
#endif

  return 0;
}

#ifndef __CLING__
int main(int argc, char const* argv[]) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\tsamplingFraction dst2root_file.root" << std::endl;
    exit(1);
  }
  return samplingFraction(argv[1]);
}
#endif
