#include "TCanvas.h"
#include "TChain.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <chrono>

std::vector<int>*   pid;
std::vector<float>* p;
std::vector<float>* px;
std::vector<float>* py;
std::vector<float>* pz;
std::vector<int>*   charge;
std::vector<float>* ec_tot_energy;
std::vector<int>*   ec_pcal_sec;

static const double MASS_E = 0.000511;
static const double MASS_P = 0.93827203;

// Calcuating Q^2
// q^mu^2 = (e^mu - e^mu')^2 = -Q^2
double Q2_calc(TLorentzVector e_mu, TLorentzVector e_mu_prime) {
  TLorentzVector q_mu = (e_mu - e_mu_prime);
  return -q_mu.Mag2();
}
//	Calcualting W
//	Gotten from s channel [(gamma - P)^2 == s == w^2]
//	Sqrt√[M_p^2 - Q^2 + 2 M_p gamma]
double W_calc(TLorentzVector e_mu, TLorentzVector e_mu_prime) {
  TLorentzVector q_mu = (e_mu - e_mu_prime);
  TVector3       p_mu_3(0, 0, 0);
  TLorentzVector p_mu;
  p_mu.SetVectM(p_mu_3, MASS_P);
  return (p_mu + q_mu).Mag();
}

TH2D* wq2 = new TH2D("wq2", "W vs Q^{2} no Sector", 500, 0, 3.5, 500, 0, 6.0);
TH1D* w   = new TH1D("w", "W no Sector", 500, 0, 3.5);

TH2D* wq2_s[6];
TH1D* w_s[6];

TChain* clas12 = new TChain("clas12", "clas12");

int WvsQ2(std::string file = "test.root", double BEAM = 2.2) {
  for (size_t i = 0; i < 6; i++) {
    wq2_s[i] = new TH2D(Form("wq2_%zu", i), Form("W vs Q^{2} Sector: %zu", i + 1), 500, 0, 3.5, 500,
                        0, 6.0);
    w_s[i]   = new TH1D(Form("w_%zu", i), Form("W Sector: %zu", i + 1), 500, 0, 3.5);
  }

  TLorentzVector e_mu(0.0, 0.0, BEAM, BEAM);
  TLorentzVector e_mu_prime;
  clas12->Add(file.c_str());
  clas12->SetBranchAddress("pid", &pid);
  clas12->SetBranchAddress("p", &p);
  clas12->SetBranchAddress("px", &px);
  clas12->SetBranchAddress("py", &py);
  clas12->SetBranchAddress("pz", &pz);
  clas12->SetBranchAddress("charge", &charge);
  clas12->SetBranchAddress("ec_tot_energy", &ec_tot_energy);
  clas12->SetBranchAddress("ec_pcal_sec", &ec_pcal_sec);

  int  num_of_events = (int)clas12->GetEntries();
  auto start_full    = std::chrono::high_resolution_clock::now();
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    if (current_event % 10000 == 0)
      std::cout << "\t" << (current_event * 100 / num_of_events) << "%\r\r" << std::flush;
    clas12->GetEntry(current_event);
    if (pid->size() == 0 || pid->at(0) != 11)
      continue;

    e_mu_prime.SetXYZM(px->at(0), py->at(0), pz->at(0), MASS_E);

    if (ec_pcal_sec->at(0) > 0) {
      int sector = ec_pcal_sec->at(0) - 1;
      w_s[sector]->Fill(W_calc(e_mu, e_mu_prime));
      wq2_s[sector]->Fill(W_calc(e_mu, e_mu_prime), Q2_calc(e_mu, e_mu_prime));
    } else {
      wq2->Fill(W_calc(e_mu, e_mu_prime), Q2_calc(e_mu, e_mu_prime));
      w->Fill(W_calc(e_mu, e_mu_prime));
    }
  }

  std::chrono::duration<double> elapsed_full =
      (std::chrono::high_resolution_clock::now() - start_full);
  std::cout << "Elapsed time for " << num_of_events << " events: " << elapsed_full.count() << " s"
            << std::endl;
  std::cout << "Events/Sec: " << (num_of_events / elapsed_full.count()) << " Hz" << std::endl;

  TCanvas* c1 = new TCanvas("c1", "c1");
  c1->cd();
  wq2->Draw("colz");

  TCanvas* c2 = new TCanvas("c2", "c2");
  c2->cd();
  w->Draw("same");

  TCanvas* c3 = new TCanvas("c3", "c3", 1600, 900);
  gStyle->SetOptFit(1111);
  c3->Divide(3, 2);
  for (size_t i = 0; i < 6; i++) {
    c3->cd(i + 1);
    w_s[i]->Draw("same");
    w_s[i]->Fit("gaus", "QMR+", "", 0.5, 1.05);
  }

  TCanvas* c4 = new TCanvas("c4", "c4", 1600, 900);
  c4->Divide(3, 2);
  for (size_t i = 0; i < 6; i++) {
    c4->cd(i + 1);
    wq2_s[i]->Draw("samecolor");
  }

  return 0;
}
