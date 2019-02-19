#include "TCanvas.h"
#include "TChain.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <iostream>

std::vector<int>*   pid;
std::vector<float>* p;
std::vector<float>* b;
std::vector<int>*   charge;

TH2D* MomVsBeta = new TH2D("MomVsBeta", "Momentum vs Beta", 500, 0, 3.5, 500, 0, 1.2);

TChain* clas12 = new TChain("clas12", "clas12");

int pvsb(std::string file = "test.root") {
  clas12->Add(file.c_str());
  clas12->SetBranchAddress("pid", &pid);
  clas12->SetBranchAddress("p", &p);
  clas12->SetBranchAddress("beta", &b);
  clas12->SetBranchAddress("charge", &charge);

  int num_of_events = (int)clas12->GetEntries();
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    clas12->GetEntry(current_event);
    for (size_t i = 1; i < pid->size(); i++) {
      if (b->at(i) < 0.05 || charge->at(i) == 0)
        continue;
      MomVsBeta->Fill(p->at(i), b->at(i));
    }
  }

  TCanvas* c1 = new TCanvas("c1", "c1");
  c1->cd();
  MomVsBeta->Draw("colz");

  return 0;
}

#ifndef __CLING__
int main(int argc, char const* argv[]) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\tpvsb dst2root_file.root" << std::endl;
    exit(1);
  }
  return pvsb(argv[1]);
}
#endif
