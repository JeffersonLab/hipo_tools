#include "TH2.h"
#include "TLorentzVector.h"

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

  return 0;
}
