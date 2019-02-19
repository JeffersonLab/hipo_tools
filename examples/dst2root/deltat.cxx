#include "TCanvas.h"
#include "TChain.h"
#include "TH2.h"
#include "TLorentzVector.h"
#include <chrono>
#include <iostream>

std::vector<int>*   pid;
std::vector<float>* p;
std::vector<int>*   charge;
std::vector<float>* STTime;

std::vector<float>* sc_ftof_1a_time;
std::vector<float>* sc_ftof_1a_path;
std::vector<int>*   sc_ftof_1a_component;

std::vector<float>* sc_ftof_1b_time;
std::vector<float>* sc_ftof_1b_path;
std::vector<int>*   sc_ftof_1b_component;

std::vector<float>* sc_ftof_2_time;
std::vector<float>* sc_ftof_2_path;
std::vector<int>*   sc_ftof_2_component;

std::vector<float>* sc_ctof_time;
std::vector<float>* sc_ctof_path;
std::vector<int>*   sc_ctof_component;

static const double MASS_P          = 0.93827203;
static const double MASS_PIP        = 0.13957018;
const double        c_special_units = 29.9792458;

double vertex_time(double sc_time, double sc_pathlength, double relatavistic_b) {
  return sc_time - sc_pathlength / (relatavistic_b * c_special_units);
}

double delta_t_calc(double vertex, double momentum, double sc_time, double sc_pathlength,
                    double mass) {
  double b  = 1.0 / sqrt(1.0 + (mass / momentum) * (mass / momentum));
  double dt = vertex - vertex_time(sc_time, sc_pathlength, b);
  return dt;
}

int deltat(std::string file = "test.root") {
  TH2D* deltaT_1a_prot = new TH2D("deltaT_1a_prot", "#Deltat Proton", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_1a_pip  = new TH2D("deltaT_1a_pion", "#Deltat #pi^{+}", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_1a_pim  = new TH2D("deltaT_1a_pion_m", "#Deltat #pi^{-}", 500, 0, 7.0, 500, -10, 10);

  TH2D* deltaT_1b_prot = new TH2D("deltaT_1b_prot", "#Deltat Proton", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_1b_pip  = new TH2D("deltaT_1b_pion", "#Deltat #pi^{+}", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_1b_pim  = new TH2D("deltaT_1b_pion_m", "#Deltat #pi^{-}", 500, 0, 7.0, 500, -10, 10);

  TH2D* deltaT_2_prot = new TH2D("deltaT_2_prot", "#Deltat Proton", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_2_pip  = new TH2D("deltaT_2_pion", "#Deltat #pi^{+}", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_2_pim  = new TH2D("deltaT_2_pion_m", "#Deltat #pi^{-}", 500, 0, 7.0, 500, -10, 10);

  TH2D* deltaT_ctof_prot =
      new TH2D("deltaT_ctof_prot", "#Deltat Proton", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_ctof_pip =
      new TH2D("deltaT_ctof_pion", "#Deltat #pi^{+}", 500, 0, 7.0, 500, -10, 10);
  TH2D* deltaT_ctof_pim =
      new TH2D("deltaT_ctof_pion_m", "#Deltat #pi^{-}", 500, 0, 7.0, 500, -10, 10);

  TH2D* deltaT_ctof_prot_component = new TH2D(
      "deltaT_ctof_prot_component", "#Deltat Proton vs Component", 50, 0, 50, 500, -10, 10);
  TH2D* deltaT_ctof_pip_component = new TH2D(
      "deltaT_ctof_pion_component", "#Deltat #pi^{+} vs Component", 50, 0, 50, 500, -10, 10);
  TH2D* deltaT_ctof_pim_component = new TH2D(
      "deltaT_ctof_pion_m_component", "#Deltat #pi^{-} vs Component", 50, 0, 50, 500, -10, 10);
  TH2D* deltaT_component =
      new TH2D("deltaT_component", "#Deltat vs Component", 50, 0, 50, 500, -10, 10);

  TChain* clas12 = new TChain("clas12", "clas12");

  clas12->Add(file.c_str());

  clas12->SetBranchAddress("pid", &pid);
  clas12->SetBranchAddress("p", &p);
  clas12->SetBranchAddress("charge", &charge);

  clas12->SetBranchAddress("sc_ftof_1a_time", &sc_ftof_1a_time);
  clas12->SetBranchAddress("sc_ftof_1a_path", &sc_ftof_1a_path);
  clas12->SetBranchAddress("sc_ftof_1a_component", &sc_ftof_1a_component);

  clas12->SetBranchAddress("sc_ftof_1b_time", &sc_ftof_1b_time);
  clas12->SetBranchAddress("sc_ftof_1b_path", &sc_ftof_1b_path);
  clas12->SetBranchAddress("sc_ftof_1b_component", &sc_ftof_1b_component);

  clas12->SetBranchAddress("sc_ftof_2_time", &sc_ftof_2_time);
  clas12->SetBranchAddress("sc_ftof_2_path", &sc_ftof_2_path);
  clas12->SetBranchAddress("sc_ftof_2_component", &sc_ftof_2_component);

  clas12->SetBranchAddress("sc_ctof_time", &sc_ctof_time);
  clas12->SetBranchAddress("sc_ctof_path", &sc_ctof_path);
  clas12->SetBranchAddress("sc_ctof_component", &sc_ctof_component);

  int    num_of_events = (int)clas12->GetEntries();
  auto   start_full    = std::chrono::high_resolution_clock::now();
  double dt_P, dt_PIP;
  for (int current_event = 0; current_event < num_of_events; current_event++) {
    clas12->GetEntry(current_event);
    if (pid->size() == 0)
      continue;
    double vertex = 0.0;

    if (sc_ftof_1b_time->at(0) == sc_ftof_1b_time->at(0)) {
      vertex = vertex_time(sc_ftof_1b_time->at(0), sc_ftof_1b_path->at(0), 1.0);
    } else if (sc_ftof_1a_time->at(0) == sc_ftof_1a_time->at(0)) {
      vertex = vertex_time(sc_ftof_1a_time->at(0), sc_ftof_1a_path->at(0), 1.0);
    } else {
      continue;
    }

    for (size_t part = 0; part < pid->size(); part++) {
      if (p->at(part) == 0)
        continue;
      deltaT_component->Fill(sc_ctof_component->at(part),
                             delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                          sc_ctof_path->at(part), MASS_P));
      deltaT_component->Fill(sc_ctof_component->at(part),
                             delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                          sc_ctof_path->at(part), MASS_PIP));
      if (charge->at(part) == 1) {
        deltaT_1a_prot->Fill(p->at(part),
                             delta_t_calc(vertex, p->at(part), sc_ftof_1a_time->at(part),
                                          sc_ftof_1a_path->at(part), MASS_P));
        deltaT_1a_pip->Fill(p->at(part),
                            delta_t_calc(vertex, p->at(part), sc_ftof_1a_time->at(part),
                                         sc_ftof_1a_path->at(part), MASS_PIP));

        deltaT_1b_prot->Fill(p->at(part),
                             delta_t_calc(vertex, p->at(part), sc_ftof_1b_time->at(part),
                                          sc_ftof_1b_path->at(part), MASS_P));
        deltaT_1b_pip->Fill(p->at(part),
                            delta_t_calc(vertex, p->at(part), sc_ftof_1b_time->at(part),
                                         sc_ftof_1b_path->at(part), MASS_PIP));

        deltaT_2_prot->Fill(p->at(part), delta_t_calc(vertex, p->at(part), sc_ftof_2_time->at(part),
                                                      sc_ftof_2_path->at(part), MASS_P));
        deltaT_2_pip->Fill(p->at(part), delta_t_calc(vertex, p->at(part), sc_ftof_2_time->at(part),
                                                     sc_ftof_2_path->at(part), MASS_PIP));

        deltaT_ctof_prot->Fill(p->at(part),
                               delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                            sc_ctof_path->at(part), MASS_P));
        deltaT_ctof_pip->Fill(p->at(part), delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                                        sc_ctof_path->at(part), MASS_PIP));

        deltaT_ctof_prot_component->Fill(sc_ctof_component->at(part),
                                         delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                                      sc_ctof_path->at(part), MASS_P));
        deltaT_ctof_pip_component->Fill(sc_ctof_component->at(part),
                                        delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                                     sc_ctof_path->at(part), MASS_PIP));

      } else if (charge->at(part) == -1) {
        deltaT_1a_pim->Fill(p->at(part),
                            delta_t_calc(vertex, p->at(part), sc_ftof_1a_time->at(part),
                                         sc_ftof_1a_path->at(part), MASS_PIP));

        deltaT_1b_pim->Fill(p->at(part),
                            delta_t_calc(vertex, p->at(part), sc_ftof_1b_time->at(part),
                                         sc_ftof_1b_path->at(part), MASS_PIP));

        deltaT_2_pim->Fill(p->at(part), delta_t_calc(vertex, p->at(part), sc_ftof_2_time->at(part),
                                                     sc_ftof_2_path->at(part), MASS_PIP));

        deltaT_ctof_pim->Fill(p->at(part), delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                                        sc_ctof_path->at(part), MASS_PIP));

        deltaT_ctof_pim_component->Fill(sc_ctof_component->at(part),
                                        delta_t_calc(vertex, p->at(part), sc_ctof_time->at(part),
                                                     sc_ctof_path->at(part), MASS_PIP));
      }
    }
  }

  std::chrono::duration<double> elapsed_full =
      (std::chrono::high_resolution_clock::now() - start_full);
  std::cout << "Elapsed time for " << num_of_events << " events: " << elapsed_full.count() << " s"
            << std::endl;
  std::cout << "Events/Sec: " << num_of_events / elapsed_full.count() << " Hz" << std::endl;

  TCanvas* c1 = new TCanvas("c1", "c1", 10, 10, 900, 600);
  c1->Divide(3, 3);
  c1->cd(1);
  deltaT_1a_prot->Draw("colz");
  c1->cd(2);
  deltaT_1a_pip->Draw("colz");
  c1->cd(3);
  deltaT_1a_pim->Draw("colz");
  c1->cd(4);
  deltaT_1b_prot->Draw("colz");
  c1->cd(5);
  deltaT_1b_pip->Draw("colz");
  c1->cd(6);
  deltaT_1b_pim->Draw("colz");
  c1->cd(7);
  deltaT_2_prot->Draw("colz");
  c1->cd(8);
  deltaT_2_pip->Draw("colz");
  c1->cd(9);
  deltaT_2_pim->Draw("colz");

  TCanvas* c2 = new TCanvas("c2", "c2", 910, 10, 900, 600);
  c2->Divide(3);
  c2->cd(1);
  deltaT_ctof_prot->Draw("colz");
  c2->cd(2);
  deltaT_ctof_pip->Draw("colz");
  c2->cd(3);
  deltaT_ctof_pim->Draw("colz");

  TCanvas* c3 = new TCanvas("c3", "c3", 910, 10, 900, 600);
  c3->Divide(2, 2);
  c3->cd(1);
  deltaT_ctof_prot_component->Draw("colz");
  c3->cd(2);
  deltaT_ctof_pip_component->Draw("colz");
  c3->cd(3);
  deltaT_ctof_pim_component->Draw("colz");
  c3->cd(4);
  deltaT_component->Draw("colz");
  c3->SaveAs("delta_t_components.pdf");

  return 0;
}

#ifndef __CLING__
int main(int argc, char const* argv[]) {
  if (argc < 2) {
    std::cerr << "Not enough arguments" << std::endl;
    std::cerr << "To Use:\tdeltat dst2root_file.root" << std::endl;
    exit(1);
  }
  return deltat(argv[1]);
}
#endif
