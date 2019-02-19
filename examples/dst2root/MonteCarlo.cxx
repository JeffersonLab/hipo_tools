#include "TCanvas.h"
#include "TChain.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <chrono>
#include <iostream>

std::vector<int>*   pid;
std::vector<float>* p;
std::vector<float>* px;
std::vector<float>* py;
std::vector<float>* pz;
std::vector<float>* b;
std::vector<int>*   charge;

std::vector<int>*   MC_pid;
std::vector<float>* MC_px;
std::vector<float>* MC_py;
std::vector<float>* MC_pz;

TChain* clas12 = new TChain("clas12", "clas12");
TH1D*   deltaP[8];

int MonteCarlo(std::string file = "test.root", double BEAM = 2.2) {

  deltaP[0] = new TH1D("delta_px_e", "#Delta P_{x}/P vetrex e^{-}", 100, -2, 2);
  deltaP[1] = new TH1D("delta_py_e", "#Delta P_{y}/P vetrex e^{-}", 100, -2, 2);
  deltaP[2] = new TH1D("delta_pz_e", "#Delta P_{z}/P vetrex e^{-}", 100, -2, 2);
  deltaP[3] = new TH1D("delta_p_e", "#Delta P/P vetrex e^{-}", 100, -2, 2);

  deltaP[4] = new TH1D("delta_px_p", "#Delta P_{x}/P Particles", 100, -2, 2);
  deltaP[5] = new TH1D("delta_py_p", "#Delta P_{y}/P Particles", 100, -2, 2);
  deltaP[6] = new TH1D("delta_pz_p", "#Delta P_{z}/P Particles", 100, -2, 2);
  deltaP[7] = new TH1D("delta_p_p", "#Delta P/P Particles", 100, -2, 2);

  clas12->Add(file.c_str());
  clas12->SetBranchAddress("pid", &pid);
  clas12->SetBranchAddress("p", &p);
  clas12->SetBranchAddress("px", &px);
  clas12->SetBranchAddress("py", &py);
  clas12->SetBranchAddress("pz", &pz);
  clas12->SetBranchAddress("beta", &b);
  clas12->SetBranchAddress("charge", &charge);
  clas12->SetBranchAddress("mc_pid", &MC_pid);
  clas12->SetBranchAddress("mc_px", &MC_px);
  clas12->SetBranchAddress("mc_py", &MC_py);
  clas12->SetBranchAddress("mc_pz", &MC_pz);

  int  num_of_events = (int)clas12->GetEntries();
  auto start_full    = std::chrono::high_resolution_clock::now();
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    clas12->GetEntry(current_event);
    if (pid->size() == 0)
      continue;
    double mc_P = NAN;
    if (pid->at(0) == 11) {
      mc_P = sqrt(MC_px->at(0) * MC_px->at(0) + MC_py->at(0) * MC_py->at(0) +
                  MC_pz->at(0) * MC_pz->at(0));
      deltaP[0]->Fill((px->at(0) - MC_px->at(0)) / MC_px->at(0));
      deltaP[1]->Fill((py->at(0) - MC_py->at(0)) / MC_py->at(0));
      deltaP[2]->Fill((pz->at(0) - MC_pz->at(0)) / MC_pz->at(0));
      deltaP[3]->Fill((p->at(0) - mc_P) / mc_P);
    }

    for (size_t mc_part = 1; mc_part < MC_pid->size(); mc_part++) {
      for (size_t part = 1; part < pid->size(); part++) {
        if (MC_pid->at(mc_part) == pid->at(part)) {
          mc_P = sqrt(MC_px->at(mc_part) * MC_px->at(mc_part) +
                      MC_py->at(mc_part) * MC_py->at(mc_part) +
                      MC_pz->at(mc_part) * MC_pz->at(mc_part));
          deltaP[4]->Fill((px->at(part) - MC_px->at(mc_part)) / MC_px->at(mc_part));
          deltaP[5]->Fill((py->at(part) - MC_py->at(mc_part)) / MC_py->at(mc_part));
          deltaP[6]->Fill((pz->at(part) - MC_pz->at(mc_part)) / MC_pz->at(mc_part));
          deltaP[7]->Fill((p->at(part) - mc_P) / mc_P);
        }
      }
    }
  }

  std::chrono::duration<double> elapsed_full =
      (std::chrono::high_resolution_clock::now() - start_full);
  std::cout << "Elapsed time for " << num_of_events << " events: " << elapsed_full.count() << " s"
            << std::endl;
  std::cout << "Events/Sec: " << (num_of_events / elapsed_full.count()) << " Hz" << std::endl;

  TCanvas* c1 = new TCanvas("c1", "c1", 1500, 800);
  c1->Divide(4, 2);

  for (int i = 0; i < 8; i++) {
    c1->cd(i + 1);
    deltaP[i]->Fit("gaus", "QM+", "", -0.5, 0.5);
    deltaP[i]->Draw();
  }

  return 0;
}

#if !defined(__CLING__)
int main(int argc, char const* argv[]) {
  if (argc < 3) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\tMonteCarlo dst2root_file.root beam_energy" << std::endl;
    exit(1);
  }
  return MonteCarlo(argv[1], atof(argv[2]));
}
#endif
