/*
 *
 * Author: Nick Tyler, University Of South Carolina
 */
// Standard libs
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <time.h>
#include <vector>
// ROOT libs
#include "TFile.h"
#include "TTree.h"
// Hipo libs
#include "hipo4/reader.h"

// Headers in same folder
#include "clipp.h"
#include "constants.h"

int main(int argc, char** argv) {
  std::string InFileName  = "";
  std::string OutFileName = "";
  bool        is_mc       = false;
  bool        is_batch    = false;
  bool        print_help  = false;
  bool        good_rec    = false;
  bool        elec_first  = false;
  bool        cov         = false;

  auto cli = (clipp::option("-h", "--help").set(print_help) % "print help",
              clipp::option("-mc", "--MC").set(is_mc) % "Convert dst and mc banks",
              clipp::option("-b", "--batch").set(is_batch) % "Don't show progress and statistics",
              clipp::option("-r", "--rec").set(good_rec) %
                  "Only save events where number of partilces in the event > 0",
              clipp::option("-e", "--elec").set(elec_first) %
                  "Only save events with good electron as first particle",
              clipp::option("-c", "--cov").set(cov) % "Save Covariant Matrix for kinematic fitting",
              clipp::value("inputFile.hipo", InFileName),
              clipp::opt_value("outputFile.root", OutFileName));

  clipp::parse(argc, argv, cli);
  if (print_help || InFileName == "") {
    std::cout << clipp::make_man_page(cli, argv[0]);
    exit(0);
  }

  if (OutFileName == "")
    OutFileName = InFileName + ".root";

  auto   start_full = std::chrono::high_resolution_clock::now();
  TFile* OutputFile = new TFile(OutFileName.c_str(), "RECREATE");
  OutputFile->SetCompressionSettings(6);

  TTree*        clas12          = new TTree("clas12", "clas12");
  hipo::reader* reader          = new hipo::reader(InFileName);
  size_t        tot_hipo_events = reader->numEvents();

  hipo::dictionary* dict = new hipo::dictionary();
  reader->readDictionary(*dict);
  hipo::event* hipo_event = new hipo::event();

  hipo::bank* rec_ForwardTagger = new hipo::bank(dict->getSchema("REC::ForwardTagger"));
  hipo::bank* rec_VertDoca      = new hipo::bank(dict->getSchema("REC::VertDoca"));
  hipo::bank* rec_Track         = new hipo::bank(dict->getSchema("REC::Track"));
  hipo::bank* rec_Cherenkov     = new hipo::bank(dict->getSchema("REC::Cherenkov"));
  hipo::bank* rec_Event         = new hipo::bank(dict->getSchema("REC::Event"));
  hipo::bank* rec_Particle      = new hipo::bank(dict->getSchema("REC::Particle"));
  hipo::bank* rec_Scintillator  = new hipo::bank(dict->getSchema("REC::Scintillator"));
  hipo::bank* rec_Calorimeter   = new hipo::bank(dict->getSchema("REC::Calorimeter"));
  hipo::bank* rec_CovMat        = new hipo::bank(dict->getSchema("REC::CovMat"));

  int    NRUN;
  int    NEVENT;
  float  EVNTime;
  int    TYPE;
  int    TRG;
  float  BCG;
  float  STTime;
  float  RFTime;
  int    Helic;
  int    EvCAT;
  int    NPGP;
  double LT;
  float  PTIME;

  std::vector<int>   pid;
  std::vector<float> p;
  std::vector<float> p2;
  std::vector<float> px;
  std::vector<float> py;
  std::vector<float> pz;
  std::vector<float> vx;
  std::vector<float> vy;
  std::vector<float> vz;
  std::vector<int>   charge;
  std::vector<float> beta;
  std::vector<float> chi2pid;
  std::vector<int>   status;

  std::vector<int>   cal_pindex;
  std::vector<int>   cal_detector;
  std::vector<int>   cal_sector;
  std::vector<int>   cal_layer;
  std::vector<float> cal_energy;
  std::vector<float> cal_time;
  std::vector<float> cal_path;
  std::vector<float> cal_x;
  std::vector<float> cal_y;
  std::vector<float> cal_z;
  std::vector<float> cal_lu;
  std::vector<float> cal_lv;
  std::vector<float> cal_lw;

  std::vector<int>   chern_pindex;
  std::vector<int>   chern_detector;
  std::vector<int>   chern_sector;
  std::vector<float> chern_nphe;
  std::vector<float> chern_time;
  std::vector<float> chern_path;
  std::vector<float> chern_theta;
  std::vector<float> chern_phi;

  std::vector<int>   fortag_pindex;
  std::vector<int>   fortag_detector;
  std::vector<float> fortag_energy;
  std::vector<float> fortag_time;
  std::vector<float> fortag_path;
  std::vector<float> fortag_x;
  std::vector<float> fortag_y;
  std::vector<float> fortag_z;
  std::vector<float> fortag_dx;
  std::vector<float> fortag_dy;
  std::vector<float> fortag_radius;
  std::vector<int>   fortag_size;

  std::vector<int>   dc_sec;
  std::vector<float> dc_px;
  std::vector<float> dc_py;
  std::vector<float> dc_pz;
  std::vector<float> dc_vx;
  std::vector<float> dc_vy;
  std::vector<float> dc_vz;

  std::vector<float> cvt_px;
  std::vector<float> cvt_py;
  std::vector<float> cvt_pz;
  std::vector<float> cvt_vx;
  std::vector<float> cvt_vy;
  std::vector<float> cvt_vz;

  std::vector<float> ec_tot_energy;
  std::vector<float> ec_pcal_energy;
  std::vector<int>   ec_pcal_sec;
  std::vector<float> ec_pcal_time;
  std::vector<float> ec_pcal_path;
  std::vector<float> ec_pcal_x;
  std::vector<float> ec_pcal_y;
  std::vector<float> ec_pcal_z;
  std::vector<float> ec_pcal_lu;
  std::vector<float> ec_pcal_lv;
  std::vector<float> ec_pcal_lw;

  std::vector<float> ec_ecin_energy;
  std::vector<int>   ec_ecin_sec;
  std::vector<float> ec_ecin_time;
  std::vector<float> ec_ecin_path;
  std::vector<float> ec_ecin_x;
  std::vector<float> ec_ecin_y;
  std::vector<float> ec_ecin_z;
  std::vector<float> ec_ecin_lu;
  std::vector<float> ec_ecin_lv;
  std::vector<float> ec_ecin_lw;

  std::vector<float> ec_ecout_energy;
  std::vector<int>   ec_ecout_sec;
  std::vector<float> ec_ecout_time;
  std::vector<float> ec_ecout_path;
  std::vector<float> ec_ecout_x;
  std::vector<float> ec_ecout_y;
  std::vector<float> ec_ecout_z;
  std::vector<float> ec_ecout_lu;
  std::vector<float> ec_ecout_lv;
  std::vector<float> ec_ecout_lw;

  std::vector<float> cc_nphe_tot;
  std::vector<int>   cc_ltcc_sec;
  std::vector<float> cc_ltcc_nphe;
  std::vector<float> cc_ltcc_time;
  std::vector<float> cc_ltcc_path;
  std::vector<float> cc_ltcc_theta;
  std::vector<float> cc_ltcc_phi;
  std::vector<int>   cc_htcc_sec;
  std::vector<float> cc_htcc_nphe;
  std::vector<float> cc_htcc_time;
  std::vector<float> cc_htcc_path;
  std::vector<float> cc_htcc_theta;
  std::vector<float> cc_htcc_phi;
  std::vector<int>   cc_rich_sec;
  std::vector<float> cc_rich_nphe;
  std::vector<float> cc_rich_time;
  std::vector<float> cc_rich_path;
  std::vector<float> cc_rich_theta;
  std::vector<float> cc_rich_phi;

  std::vector<int>   sc_ftof_1a_sec;
  std::vector<float> sc_ftof_1a_time;
  std::vector<float> sc_ftof_1a_path;
  std::vector<float> sc_ftof_1a_energy;
  std::vector<int>   sc_ftof_1a_component;
  std::vector<float> sc_ftof_1a_x;
  std::vector<float> sc_ftof_1a_y;
  std::vector<float> sc_ftof_1a_z;
  std::vector<float> sc_ftof_1a_hx;
  std::vector<float> sc_ftof_1a_hy;
  std::vector<float> sc_ftof_1a_hz;

  std::vector<int>   sc_ftof_1b_sec;
  std::vector<float> sc_ftof_1b_time;
  std::vector<float> sc_ftof_1b_path;
  std::vector<float> sc_ftof_1b_energy;
  std::vector<int>   sc_ftof_1b_component;
  std::vector<float> sc_ftof_1b_x;
  std::vector<float> sc_ftof_1b_y;
  std::vector<float> sc_ftof_1b_z;
  std::vector<float> sc_ftof_1b_hx;
  std::vector<float> sc_ftof_1b_hy;
  std::vector<float> sc_ftof_1b_hz;

  std::vector<int>   sc_ftof_2_sec;
  std::vector<float> sc_ftof_2_time;
  std::vector<float> sc_ftof_2_path;
  std::vector<float> sc_ftof_2_energy;
  std::vector<int>   sc_ftof_2_component;
  std::vector<float> sc_ftof_2_x;
  std::vector<float> sc_ftof_2_y;
  std::vector<float> sc_ftof_2_z;
  std::vector<float> sc_ftof_2_hx;
  std::vector<float> sc_ftof_2_hy;
  std::vector<float> sc_ftof_2_hz;

  std::vector<float> sc_ctof_time;
  std::vector<float> sc_ctof_path;
  std::vector<float> sc_ctof_energy;
  std::vector<int>   sc_ctof_component;
  std::vector<float> sc_ctof_x;
  std::vector<float> sc_ctof_y;
  std::vector<float> sc_ctof_z;
  std::vector<float> sc_ctof_hx;
  std::vector<float> sc_ctof_hy;
  std::vector<float> sc_ctof_hz;

  std::vector<float> sc_cnd_time;
  std::vector<float> sc_cnd_path;
  std::vector<float> sc_cnd_energy;
  std::vector<int>   sc_cnd_component;
  std::vector<float> sc_cnd_x;
  std::vector<float> sc_cnd_y;
  std::vector<float> sc_cnd_z;
  std::vector<float> sc_cnd_hx;
  std::vector<float> sc_cnd_hy;
  std::vector<float> sc_cnd_hz;

  std::vector<float> ft_cal_energy;
  std::vector<float> ft_cal_time;
  std::vector<float> ft_cal_path;
  std::vector<float> ft_cal_x;
  std::vector<float> ft_cal_y;
  std::vector<float> ft_cal_z;
  std::vector<float> ft_cal_dx;
  std::vector<float> ft_cal_dy;
  std::vector<float> ft_cal_radius;

  std::vector<float> ft_hodo_energy;
  std::vector<float> ft_hodo_time;
  std::vector<float> ft_hodo_path;
  std::vector<float> ft_hodo_x;
  std::vector<float> ft_hodo_y;
  std::vector<float> ft_hodo_z;
  std::vector<float> ft_hodo_dx;
  std::vector<float> ft_hodo_dy;
  std::vector<float> ft_hodo_radius;

  std::vector<int>   MC_pid;
  std::vector<float> MC_helicity;
  std::vector<float> MC_px;
  std::vector<float> MC_py;
  std::vector<float> MC_pz;
  std::vector<float> MC_vx;
  std::vector<float> MC_vy;
  std::vector<float> MC_vz;
  std::vector<float> MC_vt;

  std::vector<int>   Lund_pid;
  std::vector<float> Lund_px;
  std::vector<float> Lund_py;
  std::vector<float> Lund_pz;
  std::vector<float> Lund_E;
  std::vector<float> Lund_vx;
  std::vector<float> Lund_vy;
  std::vector<float> Lund_vz;
  std::vector<float> Lund_ltime;

  std::vector<float> CovMat_11;
  std::vector<float> CovMat_12;
  std::vector<float> CovMat_13;
  std::vector<float> CovMat_14;
  std::vector<float> CovMat_15;
  std::vector<float> CovMat_22;
  std::vector<float> CovMat_23;
  std::vector<float> CovMat_24;
  std::vector<float> CovMat_25;
  std::vector<float> CovMat_33;
  std::vector<float> CovMat_34;
  std::vector<float> CovMat_35;
  std::vector<float> CovMat_44;
  std::vector<float> CovMat_45;
  std::vector<float> CovMat_55;

  clas12->Branch("NRUN", &NRUN);
  clas12->Branch("NEVENT", &NEVENT);
  clas12->Branch("EVNTime", &EVNTime);
  clas12->Branch("TYPE", &TYPE);
  clas12->Branch("TRG", &TRG);
  clas12->Branch("BCG", &BCG);
  clas12->Branch("STTime", &STTime);
  clas12->Branch("RFTime", &RFTime);
  clas12->Branch("Helic", &Helic);
  clas12->Branch("EvCAT", &EvCAT);
  clas12->Branch("NPGP", &NPGP);
  clas12->Branch("LT", &LT);
  clas12->Branch("PTIME", &PTIME);

  clas12->Branch("pid", &pid);
  clas12->Branch("p", &p);
  clas12->Branch("p2", &p2);
  clas12->Branch("px", &px);
  clas12->Branch("py", &py);
  clas12->Branch("pz", &pz);
  clas12->Branch("vx", &vx);
  clas12->Branch("vy", &vy);
  clas12->Branch("vz", &vz);
  clas12->Branch("charge", &charge);
  clas12->Branch("beta", &beta);
  clas12->Branch("chi2pid", &chi2pid);
  clas12->Branch("status", &status);
  if (is_mc) {
    clas12->Branch("mc_pid", &MC_pid);
    clas12->Branch("mc_px", &MC_px);
    clas12->Branch("mc_py", &MC_py);
    clas12->Branch("mc_pz", &MC_pz);
    clas12->Branch("mc_vx", &MC_vx);
    clas12->Branch("mc_vy", &MC_vy);
    clas12->Branch("mc_vz", &MC_vz);
    clas12->Branch("mc_vt", &MC_vt);
    clas12->Branch("mc_helicity", &MC_helicity);

    clas12->Branch("lund_pid", &Lund_pid);
    clas12->Branch("lund_px", &Lund_px);
    clas12->Branch("lund_py", &Lund_py);
    clas12->Branch("lund_pz", &Lund_pz);
    clas12->Branch("lund_E", &Lund_E);
    clas12->Branch("lund_vx", &Lund_vx);
    clas12->Branch("lund_vy", &Lund_vy);
    clas12->Branch("lund_vz", &Lund_vz);
    clas12->Branch("lund_ltime", &Lund_ltime);
  }

  clas12->Branch("ec_tot_energy", &ec_tot_energy);
  clas12->Branch("ec_pcal_energy", &ec_pcal_energy);
  clas12->Branch("ec_pcal_sec", &ec_pcal_sec);
  clas12->Branch("ec_pcal_time", &ec_pcal_time);
  clas12->Branch("ec_pcal_path", &ec_pcal_path);
  clas12->Branch("ec_pcal_x", &ec_pcal_x);
  clas12->Branch("ec_pcal_y", &ec_pcal_y);
  clas12->Branch("ec_pcal_z", &ec_pcal_z);
  clas12->Branch("ec_pcal_lu", &ec_pcal_lu);
  clas12->Branch("ec_pcal_lv", &ec_pcal_lv);
  clas12->Branch("ec_pcal_lw", &ec_pcal_lw);

  clas12->Branch("ec_ecin_energy", &ec_ecin_energy);
  clas12->Branch("ec_ecin_sec", &ec_ecin_sec);
  clas12->Branch("ec_ecin_time", &ec_ecin_time);
  clas12->Branch("ec_ecin_path", &ec_ecin_path);
  clas12->Branch("ec_ecin_x", &ec_ecin_x);
  clas12->Branch("ec_ecin_y", &ec_ecin_y);
  clas12->Branch("ec_ecin_z", &ec_ecin_z);
  clas12->Branch("ec_ecin_lu", &ec_ecin_lu);
  clas12->Branch("ec_ecin_lv", &ec_ecin_lv);
  clas12->Branch("ec_ecin_lw", &ec_ecin_lw);

  clas12->Branch("ec_ecout_energy", &ec_ecout_energy);
  clas12->Branch("ec_ecout_sec", &ec_ecout_sec);
  clas12->Branch("ec_ecout_time", &ec_ecout_time);
  clas12->Branch("ec_ecout_path", &ec_ecout_path);
  clas12->Branch("ec_ecout_x", &ec_ecout_x);
  clas12->Branch("ec_ecout_y", &ec_ecout_y);
  clas12->Branch("ec_ecout_z", &ec_ecout_z);
  clas12->Branch("ec_ecout_lu", &ec_ecout_lu);
  clas12->Branch("ec_ecout_lv", &ec_ecout_lv);
  clas12->Branch("ec_ecout_lw", &ec_ecout_lw);

  clas12->Branch("dc_sec", &dc_sec);
  clas12->Branch("dc_px", &dc_px);
  clas12->Branch("dc_py", &dc_py);
  clas12->Branch("dc_pz", &dc_pz);
  clas12->Branch("dc_vx", &dc_vx);
  clas12->Branch("dc_vy", &dc_vy);
  clas12->Branch("dc_vz", &dc_vz);

  clas12->Branch("cvt_px", &cvt_px);
  clas12->Branch("cvt_py", &cvt_py);
  clas12->Branch("cvt_pz", &cvt_pz);
  clas12->Branch("cvt_vx", &cvt_vx);
  clas12->Branch("cvt_vy", &cvt_vy);
  clas12->Branch("cvt_vz", &cvt_vz);

  clas12->Branch("cc_nphe_tot", &cc_nphe_tot);
  clas12->Branch("cc_ltcc_sec", &cc_ltcc_sec);
  clas12->Branch("cc_ltcc_nphe", &cc_ltcc_nphe);
  clas12->Branch("cc_ltcc_time", &cc_ltcc_time);
  clas12->Branch("cc_ltcc_path", &cc_ltcc_path);
  clas12->Branch("cc_ltcc_theta", &cc_ltcc_theta);
  clas12->Branch("cc_ltcc_phi", &cc_ltcc_phi);

  clas12->Branch("cc_htcc_sec", &cc_htcc_sec);
  clas12->Branch("cc_htcc_nphe", &cc_htcc_nphe);
  clas12->Branch("cc_htcc_time", &cc_htcc_time);
  clas12->Branch("cc_htcc_path", &cc_htcc_path);
  clas12->Branch("cc_htcc_theta", &cc_htcc_theta);
  clas12->Branch("cc_htcc_phi", &cc_htcc_phi);

  clas12->Branch("cc_rich_sec", &cc_rich_sec);
  clas12->Branch("cc_rich_nphe", &cc_rich_nphe);
  clas12->Branch("cc_rich_time", &cc_rich_time);
  clas12->Branch("cc_rich_path", &cc_rich_path);
  clas12->Branch("cc_rich_theta", &cc_rich_theta);
  clas12->Branch("cc_rich_phi", &cc_rich_phi);

  clas12->Branch("sc_ftof_1a_sec", &sc_ftof_1a_sec);
  clas12->Branch("sc_ftof_1a_time", &sc_ftof_1a_time);
  clas12->Branch("sc_ftof_1a_path", &sc_ftof_1a_path);
  clas12->Branch("sc_ftof_1a_energy", &sc_ftof_1a_energy);
  clas12->Branch("sc_ftof_1a_component", &sc_ftof_1a_component);
  clas12->Branch("sc_ftof_1a_x", &sc_ftof_1a_x);
  clas12->Branch("sc_ftof_1a_y", &sc_ftof_1a_y);
  clas12->Branch("sc_ftof_1a_z", &sc_ftof_1a_z);
  clas12->Branch("sc_ftof_1a_hx", &sc_ftof_1a_hx);
  clas12->Branch("sc_ftof_1a_hy", &sc_ftof_1a_hy);
  clas12->Branch("sc_ftof_1a_hz", &sc_ftof_1a_hz);

  clas12->Branch("sc_ftof_1b_sec", &sc_ftof_1b_sec);
  clas12->Branch("sc_ftof_1b_time", &sc_ftof_1b_time);
  clas12->Branch("sc_ftof_1b_path", &sc_ftof_1b_path);
  clas12->Branch("sc_ftof_1b_energy", &sc_ftof_1b_energy);
  clas12->Branch("sc_ftof_1b_component", &sc_ftof_1b_component);
  clas12->Branch("sc_ftof_1b_x", &sc_ftof_1b_x);
  clas12->Branch("sc_ftof_1b_y", &sc_ftof_1b_y);
  clas12->Branch("sc_ftof_1b_z", &sc_ftof_1b_z);
  clas12->Branch("sc_ftof_1b_hx", &sc_ftof_1b_hx);
  clas12->Branch("sc_ftof_1b_hy", &sc_ftof_1b_hy);
  clas12->Branch("sc_ftof_1b_hz", &sc_ftof_1b_hz);

  clas12->Branch("sc_ftof_2_sec", &sc_ftof_2_sec);
  clas12->Branch("sc_ftof_2_time", &sc_ftof_2_time);
  clas12->Branch("sc_ftof_2_path", &sc_ftof_2_path);
  clas12->Branch("sc_ftof_2_energy", &sc_ftof_2_energy);
  clas12->Branch("sc_ftof_2_component", &sc_ftof_2_component);
  clas12->Branch("sc_ftof_2_x", &sc_ftof_2_x);
  clas12->Branch("sc_ftof_2_y", &sc_ftof_2_y);
  clas12->Branch("sc_ftof_2_z", &sc_ftof_2_z);
  clas12->Branch("sc_ftof_2_hx", &sc_ftof_2_hx);
  clas12->Branch("sc_ftof_2_hy", &sc_ftof_2_hy);
  clas12->Branch("sc_ftof_2_hz", &sc_ftof_2_hz);

  clas12->Branch("sc_ctof_time", &sc_ctof_time);
  clas12->Branch("sc_ctof_path", &sc_ctof_path);
  clas12->Branch("sc_ctof_energy", &sc_ctof_energy);
  clas12->Branch("sc_ctof_component", &sc_ctof_component);
  clas12->Branch("sc_ctof_x", &sc_ctof_x);
  clas12->Branch("sc_ctof_y", &sc_ctof_y);
  clas12->Branch("sc_ctof_z", &sc_ctof_z);
  clas12->Branch("sc_ctof_hx", &sc_ctof_hx);
  clas12->Branch("sc_ctof_hy", &sc_ctof_hy);
  clas12->Branch("sc_ctof_hz", &sc_ctof_hz);

  clas12->Branch("sc_cnd_time", &sc_cnd_time);
  clas12->Branch("sc_cnd_path", &sc_cnd_path);
  clas12->Branch("sc_cnd_energy", &sc_cnd_energy);
  clas12->Branch("sc_cnd_component", &sc_cnd_component);
  clas12->Branch("sc_cnd_x", &sc_cnd_x);
  clas12->Branch("sc_cnd_y", &sc_cnd_y);
  clas12->Branch("sc_cnd_z", &sc_cnd_z);
  clas12->Branch("sc_cnd_hx", &sc_cnd_hx);
  clas12->Branch("sc_cnd_hy", &sc_cnd_hy);
  clas12->Branch("sc_cnd_hz", &sc_cnd_hz);

  clas12->Branch("ft_cal_energy", &ft_cal_energy);
  clas12->Branch("ft_cal_time", &ft_cal_time);
  clas12->Branch("ft_cal_path", &ft_cal_path);
  clas12->Branch("ft_cal_x", &ft_cal_x);
  clas12->Branch("ft_cal_y", &ft_cal_y);
  clas12->Branch("ft_cal_z", &ft_cal_z);
  clas12->Branch("ft_cal_dx", &ft_cal_dx);
  clas12->Branch("ft_cal_dy", &ft_cal_dy);
  clas12->Branch("ft_cal_radius", &ft_cal_radius);

  clas12->Branch("ft_hodo_energy", &ft_hodo_energy);
  clas12->Branch("ft_hodo_time", &ft_hodo_time);
  clas12->Branch("ft_hodo_path", &ft_hodo_path);
  clas12->Branch("ft_hodo_x", &ft_hodo_x);
  clas12->Branch("ft_hodo_y", &ft_hodo_y);
  clas12->Branch("ft_hodo_z", &ft_hodo_z);
  clas12->Branch("ft_hodo_dx", &ft_hodo_dx);
  clas12->Branch("ft_hodo_dy", &ft_hodo_dy);
  clas12->Branch("ft_hodo_radius", &ft_hodo_radius);

  if (cov) {
    clas12->Branch("CovMat_11", &CovMat_11);
    clas12->Branch("CovMat_12", &CovMat_12);
    clas12->Branch("CovMat_13", &CovMat_13);
    clas12->Branch("CovMat_14", &CovMat_14);
    clas12->Branch("CovMat_15", &CovMat_15);
    clas12->Branch("CovMat_22", &CovMat_22);
    clas12->Branch("CovMat_23", &CovMat_23);
    clas12->Branch("CovMat_24", &CovMat_24);
    clas12->Branch("CovMat_25", &CovMat_25);
    clas12->Branch("CovMat_33", &CovMat_33);
    clas12->Branch("CovMat_34", &CovMat_34);
    clas12->Branch("CovMat_35", &CovMat_35);
    clas12->Branch("CovMat_44", &CovMat_44);
    clas12->Branch("CovMat_45", &CovMat_45);
    clas12->Branch("CovMat_55", &CovMat_55);
  }

  int entry      = 0;
  int l          = 0;
  int len_pid    = 0;
  int len_pindex = 0;

  while (reader->next()) {
    reader->read(*hipo_event);
    hipo_event->getStructure(*rec_Particle);
    hipo_event->getStructure(*rec_ForwardTagger);
    hipo_event->getStructure(*rec_VertDoca);
    hipo_event->getStructure(*rec_Track);
    hipo_event->getStructure(*rec_Cherenkov);
    hipo_event->getStructure(*rec_Event);
    hipo_event->getStructure(*rec_Scintillator);
    hipo_event->getStructure(*rec_Calorimeter);
    hipo_event->getStructure(*rec_CovMat);

    if (!is_batch && (++entry % 10000) == 0)
      std::cout << "\t" << floor(100 * entry / tot_hipo_events) << "%\r\r" << std::flush;

    if (good_rec && rec_Particle->getRows() == 0)
      continue;
    if (elec_first && rec_Particle->getInt("pid", 0) != 11)
      continue;

    l = rec_Event->getRows();
    if (l != 0) {
      NRUN    = rec_Event->getInt("NRUN", 0);
      NEVENT  = rec_Event->getInt("NEVENT", 0);
      EVNTime = rec_Event->getFloat("EVNTime", 0);
      TYPE    = rec_Event->getByte("TYPE", 0);
      EvCAT   = rec_Event->getShort("EvCAT", 0);
      NPGP    = rec_Event->getShort("NPGP", 0);
      TRG     = rec_Event->getLong("TRG", 0);
      BCG     = rec_Event->getFloat("BCG", 0);
      LT      = rec_Event->getDouble("LT", 0);
      STTime  = rec_Event->getFloat("STTime", 0);
      RFTime  = rec_Event->getFloat("RFTime", 0);
      Helic   = rec_Event->getByte("Helic", 0);
      PTIME   = rec_Event->getFloat("PTIME", 0);
    }

    l = rec_Particle->getRows();
    pid.resize(l);
    p.resize(l);
    p2.resize(l);
    px.resize(l);
    py.resize(l);
    pz.resize(l);
    vx.resize(l);
    vy.resize(l);
    vz.resize(l);
    charge.resize(l);
    beta.resize(l);
    chi2pid.resize(l);
    status.resize(l);

    for (int i = 0; i < l; i++) {
      pid[i]    = rec_Particle->getInt("pid", i);
      p2[i]     = (rec_Particle->getFloat("px", i) * rec_Particle->getFloat("px", i) +
               rec_Particle->getFloat("py", i) * rec_Particle->getFloat("py", i) +
               rec_Particle->getFloat("pz", i) * rec_Particle->getFloat("pz", i));
      p[i]      = sqrt(p2[i]);
      px[i]     = rec_Particle->getFloat("px", i);
      py[i]     = rec_Particle->getFloat("py", i);
      pz[i]     = rec_Particle->getFloat("pz", i);
      vx[i]     = rec_Particle->getFloat("vx", i);
      vy[i]     = rec_Particle->getFloat("vy", i);
      vz[i]     = rec_Particle->getFloat("vz", i);
      charge[i] = rec_Particle->getInt("charge", i);
      beta[i] =
          ((rec_Particle->getFloat("beta", i) != -9999) ? rec_Particle->getFloat("beta", i) : NAN);
      chi2pid[i] = rec_Particle->getFloat("chi2pid", i);
      status[i]  = rec_Particle->getInt("status", i);
    }

    /*
        if (is_mc) {
          l = MC_pid_node->getLength();
          MC_helicity.resize(l);
          MC_pid.resize(l);
          MC_px.resize(l);
          MC_py.resize(l);
          MC_pz.resize(l);
          MC_vx.resize(l);
          MC_vy.resize(l);
          MC_vz.resize(l);
          MC_vt.resize(l);
          Lund_pid.resize(l);
          Lund_px.resize(l);
          Lund_py.resize(l);
          Lund_pz.resize(l);
          Lund_E.resize(l);
          Lund_vx.resize(l);
          Lund_vy.resize(l);
          Lund_vz.resize(l);
          Lund_ltime.resize(l);
          for (int i = 0; i < l; i++) {
            MC_helicity[i] = MC_Header_helicity_node->getValue(i);
            MC_pid[i] = MC_pid_node->getValue(i);
            MC_px[i] = MC_px_node->getValue(i);
            MC_py[i] = MC_py_node->getValue(i);
            MC_pz[i] = MC_pz_node->getValue(i);
            MC_vx[i] = MC_vx_node->getValue(i);
            MC_vy[i] = MC_vy_node->getValue(i);
            MC_vz[i] = MC_vz_node->getValue(i);
            MC_vt[i] = MC_vt_node->getValue(i);

            Lund_pid[i] = MC_Lund_pid_node->getValue(i);
            Lund_px[i] = MC_Lund_px_node->getValue(i);
            Lund_py[i] = MC_Lund_py_node->getValue(i);
            Lund_pz[i] = MC_Lund_pz_node->getValue(i);
            Lund_E[i] = MC_Lund_E_node->getValue(i);
            Lund_vx[i] = MC_Lund_vx_node->getValue(i);
            Lund_vy[i] = MC_Lund_vy_node->getValue(i);
            Lund_vz[i] = MC_Lund_vz_node->getValue(i);
            Lund_ltime[i] = MC_Lund_ltime_node->getValue(i);
          }
        }
    */
    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Calorimeter->getRows();

    ec_tot_energy.resize(len_pid);
    ec_pcal_energy.resize(len_pid);
    ec_pcal_sec.resize(len_pid);
    ec_pcal_time.resize(len_pid);
    ec_pcal_path.resize(len_pid);
    ec_pcal_x.resize(len_pid);
    ec_pcal_y.resize(len_pid);
    ec_pcal_z.resize(len_pid);
    ec_pcal_lu.resize(len_pid);
    ec_pcal_lv.resize(len_pid);
    ec_pcal_lw.resize(len_pid);
    ec_ecin_energy.resize(len_pid);
    ec_ecin_sec.resize(len_pid);
    ec_ecin_time.resize(len_pid);
    ec_ecin_path.resize(len_pid);
    ec_ecin_x.resize(len_pid);
    ec_ecin_y.resize(len_pid);
    ec_ecin_z.resize(len_pid);
    ec_ecin_lu.resize(len_pid);
    ec_ecin_lv.resize(len_pid);
    ec_ecin_lw.resize(len_pid);
    ec_ecout_energy.resize(len_pid);
    ec_ecout_sec.resize(len_pid);
    ec_ecout_time.resize(len_pid);
    ec_ecout_path.resize(len_pid);
    ec_ecout_x.resize(len_pid);
    ec_ecout_y.resize(len_pid);
    ec_ecout_z.resize(len_pid);
    ec_ecout_lu.resize(len_pid);
    ec_ecout_lv.resize(len_pid);
    ec_ecout_lw.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      ec_tot_energy[i]   = NAN;
      ec_pcal_energy[i]  = NAN;
      ec_pcal_sec[i]     = -1;
      ec_pcal_time[i]    = NAN;
      ec_pcal_path[i]    = NAN;
      ec_pcal_x[i]       = NAN;
      ec_pcal_y[i]       = NAN;
      ec_pcal_z[i]       = NAN;
      ec_pcal_lu[i]      = NAN;
      ec_pcal_lv[i]      = NAN;
      ec_pcal_lw[i]      = NAN;
      ec_ecin_energy[i]  = NAN;
      ec_ecin_sec[i]     = -1;
      ec_ecin_time[i]    = NAN;
      ec_ecin_path[i]    = NAN;
      ec_ecin_x[i]       = NAN;
      ec_ecin_y[i]       = NAN;
      ec_ecin_z[i]       = NAN;
      ec_ecin_lu[i]      = NAN;
      ec_ecin_lv[i]      = NAN;
      ec_ecin_lw[i]      = NAN;
      ec_ecout_energy[i] = NAN;
      ec_ecout_sec[i]    = -1;
      ec_ecout_time[i]   = NAN;
      ec_ecout_path[i]   = NAN;
      ec_ecout_x[i]      = NAN;
      ec_ecout_y[i]      = NAN;
      ec_ecout_z[i]      = NAN;
      ec_ecout_lu[i]     = NAN;
      ec_ecout_lv[i]     = NAN;
      ec_ecout_lw[i]     = NAN;
    }

    float pcal   = 0.0;
    float einner = 0.0;
    float eouter = 0.0;
    float etot   = 0.0;

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int   pindex   = rec_Calorimeter->getShort("pindex", k);
        int   detector = rec_Calorimeter->getByte("detector", k);
        int   layer    = rec_Calorimeter->getByte("layer", k);
        float energy   = rec_Calorimeter->getFloat("energy", k);

        if (pindex == i && detector == ECAL) {
          etot += energy;
          if (layer == PCAL) {
            pcal += energy;
            ec_pcal_sec[i]  = rec_Calorimeter->getByte("sector", k);
            ec_pcal_time[i] = rec_Calorimeter->getFloat("time", k);
            ec_pcal_path[i] = rec_Calorimeter->getFloat("path", k);
            ec_pcal_x[i]    = rec_Calorimeter->getFloat("x", k);
            ec_pcal_y[i]    = rec_Calorimeter->getFloat("y", k);
            ec_pcal_z[i]    = rec_Calorimeter->getFloat("z", k);
            ec_pcal_lu[i]   = rec_Calorimeter->getFloat("lu", k);
            ec_pcal_lv[i]   = rec_Calorimeter->getFloat("lv", k);
            ec_pcal_lw[i]   = rec_Calorimeter->getFloat("lw", k);
          } else if (layer == EC_INNER) {
            einner += energy;
            ec_ecin_sec[i]  = rec_Calorimeter->getByte("sector", k);
            ec_ecin_time[i] = rec_Calorimeter->getFloat("time", k);
            ec_ecin_path[i] = rec_Calorimeter->getFloat("path", k);
            ec_ecin_x[i]    = rec_Calorimeter->getFloat("x", k);
            ec_ecin_y[i]    = rec_Calorimeter->getFloat("y", k);
            ec_ecin_z[i]    = rec_Calorimeter->getFloat("z", k);
            ec_ecin_lu[i]   = rec_Calorimeter->getFloat("lu", k);
            ec_ecin_lv[i]   = rec_Calorimeter->getFloat("lv", k);
            ec_ecin_lw[i]   = rec_Calorimeter->getFloat("lw", k);
          } else if (layer == EC_OUTER) {
            eouter += energy;
            ec_ecout_sec[i]  = rec_Calorimeter->getByte("sector", k);
            ec_ecout_time[i] = rec_Calorimeter->getFloat("time", k);
            ec_ecout_path[i] = rec_Calorimeter->getFloat("path", k);
            ec_ecout_x[i]    = rec_Calorimeter->getFloat("x", k);
            ec_ecout_y[i]    = rec_Calorimeter->getFloat("y", k);
            ec_ecout_z[i]    = rec_Calorimeter->getFloat("z", k);
            ec_ecout_lu[i]   = rec_Calorimeter->getFloat("lu", k);
            ec_ecout_lv[i]   = rec_Calorimeter->getFloat("lv", k);
            ec_ecout_lw[i]   = rec_Calorimeter->getFloat("lw", k);
          }
        }
      }
      if (ec_pcal_energy[i] != ec_pcal_energy[i])
        ec_pcal_energy[i] = ((pcal != 0.0) ? pcal : NAN);
      if (ec_ecin_energy[i] != ec_ecin_energy[i])
        ec_ecin_energy[i] = ((einner != 0.0) ? einner : NAN);
      if (ec_ecout_energy[i] != ec_ecout_energy[i])
        ec_ecout_energy[i] = ((eouter != 0.0) ? eouter : NAN);
      if (ec_tot_energy[i] != ec_tot_energy[i])
        ec_tot_energy[i] = ((etot != 0.0) ? etot : NAN);
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Cherenkov->getRows();

    cc_nphe_tot.resize(len_pid);

    cc_ltcc_sec.resize(len_pid);
    cc_ltcc_nphe.resize(len_pid);
    cc_ltcc_time.resize(len_pid);
    cc_ltcc_path.resize(len_pid);
    cc_ltcc_theta.resize(len_pid);
    cc_ltcc_phi.resize(len_pid);

    cc_htcc_sec.resize(len_pid);
    cc_htcc_nphe.resize(len_pid);
    cc_htcc_time.resize(len_pid);
    cc_htcc_path.resize(len_pid);
    cc_htcc_theta.resize(len_pid);
    cc_htcc_phi.resize(len_pid);

    cc_rich_sec.resize(len_pid);
    cc_rich_nphe.resize(len_pid);
    cc_rich_time.resize(len_pid);
    cc_rich_path.resize(len_pid);
    cc_rich_theta.resize(len_pid);
    cc_rich_phi.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      cc_nphe_tot[i]   = NAN;
      cc_ltcc_sec[i]   = -1;
      cc_ltcc_nphe[i]  = NAN;
      cc_ltcc_time[i]  = NAN;
      cc_ltcc_path[i]  = NAN;
      cc_ltcc_theta[i] = NAN;
      cc_ltcc_phi[i]   = NAN;
      cc_htcc_sec[i]   = -1;
      cc_htcc_nphe[i]  = NAN;
      cc_htcc_time[i]  = NAN;
      cc_htcc_path[i]  = NAN;
      cc_htcc_theta[i] = NAN;
      cc_htcc_phi[i]   = NAN;
      cc_rich_sec[i]   = -1;
      cc_rich_nphe[i]  = NAN;
      cc_rich_time[i]  = NAN;
      cc_rich_path[i]  = NAN;
      cc_rich_theta[i] = NAN;
      cc_rich_phi[i]   = NAN;
    }

    float nphe_tot = 0.0;

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Cherenkov->getShort("pindex", k);
        int detector = rec_Cherenkov->getShort("detector", k);

        if (pindex == i && (detector == HTCC || detector == LTCC || detector == RICH))
          nphe_tot += rec_Cherenkov->getFloat("nphe", k);

        if (pindex == i && detector == HTCC) {
          cc_htcc_nphe[i]  = rec_Cherenkov->getFloat("nphe", k);
          cc_htcc_sec[i]   = rec_Cherenkov->getByte("sector", k);
          cc_htcc_time[i]  = rec_Cherenkov->getFloat("time", k);
          cc_htcc_path[i]  = rec_Cherenkov->getFloat("path", k);
          cc_htcc_theta[i] = rec_Cherenkov->getFloat("theta", k);
          cc_htcc_phi[i]   = rec_Cherenkov->getFloat("phi", k);
        } else if (pindex == i && detector == LTCC) {
          cc_ltcc_nphe[i]  = rec_Cherenkov->getFloat("nphe", k);
          cc_ltcc_sec[i]   = rec_Cherenkov->getByte("sector", k);
          cc_ltcc_time[i]  = rec_Cherenkov->getFloat("time", k);
          cc_ltcc_path[i]  = rec_Cherenkov->getFloat("path", k);
          cc_ltcc_theta[i] = rec_Cherenkov->getFloat("theta", k);
          cc_ltcc_phi[i]   = rec_Cherenkov->getFloat("phi", k);
        } else if (pindex == i && detector == RICH) {
          cc_rich_nphe[i]  = rec_Cherenkov->getFloat("nphe", k);
          cc_rich_sec[i]   = rec_Cherenkov->getByte("sector", k);
          cc_rich_time[i]  = rec_Cherenkov->getFloat("time", k);
          cc_rich_path[i]  = rec_Cherenkov->getFloat("path", k);
          cc_rich_theta[i] = rec_Cherenkov->getFloat("theta", k);
          cc_rich_phi[i]   = rec_Cherenkov->getFloat("phi", k);
        }
      }
      if (cc_nphe_tot[i] != cc_nphe_tot[i])
        cc_nphe_tot[i] = ((nphe_tot != 0.0) ? nphe_tot : NAN);
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Scintillator->getRows();

    sc_ftof_1a_sec.resize(len_pid);
    sc_ftof_1a_time.resize(len_pid);
    sc_ftof_1a_path.resize(len_pid);
    sc_ftof_1a_energy.resize(len_pid);
    sc_ftof_1a_component.resize(len_pid);
    sc_ftof_1a_x.resize(len_pid);
    sc_ftof_1a_y.resize(len_pid);
    sc_ftof_1a_z.resize(len_pid);
    sc_ftof_1a_hx.resize(len_pid);
    sc_ftof_1a_hy.resize(len_pid);
    sc_ftof_1a_hz.resize(len_pid);

    sc_ftof_1b_sec.resize(len_pid);
    sc_ftof_1b_time.resize(len_pid);
    sc_ftof_1b_path.resize(len_pid);
    sc_ftof_1b_energy.resize(len_pid);
    sc_ftof_1b_component.resize(len_pid);
    sc_ftof_1b_x.resize(len_pid);
    sc_ftof_1b_y.resize(len_pid);
    sc_ftof_1b_z.resize(len_pid);
    sc_ftof_1b_hx.resize(len_pid);
    sc_ftof_1b_hy.resize(len_pid);
    sc_ftof_1b_hz.resize(len_pid);

    sc_ftof_2_sec.resize(len_pid);
    sc_ftof_2_time.resize(len_pid);
    sc_ftof_2_path.resize(len_pid);
    sc_ftof_2_energy.resize(len_pid);
    sc_ftof_2_component.resize(len_pid);
    sc_ftof_2_x.resize(len_pid);
    sc_ftof_2_y.resize(len_pid);
    sc_ftof_2_z.resize(len_pid);
    sc_ftof_2_hx.resize(len_pid);
    sc_ftof_2_hy.resize(len_pid);
    sc_ftof_2_hz.resize(len_pid);

    sc_ctof_time.resize(len_pid);
    sc_ctof_path.resize(len_pid);
    sc_ctof_energy.resize(len_pid);
    sc_ctof_component.resize(len_pid);
    sc_ctof_x.resize(len_pid);
    sc_ctof_y.resize(len_pid);
    sc_ctof_z.resize(len_pid);
    sc_ctof_hx.resize(len_pid);
    sc_ctof_hy.resize(len_pid);
    sc_ctof_hz.resize(len_pid);

    sc_cnd_time.resize(len_pid);
    sc_cnd_path.resize(len_pid);
    sc_cnd_energy.resize(len_pid);
    sc_cnd_component.resize(len_pid);
    sc_cnd_x.resize(len_pid);
    sc_cnd_y.resize(len_pid);
    sc_cnd_z.resize(len_pid);
    sc_cnd_hx.resize(len_pid);
    sc_cnd_hy.resize(len_pid);
    sc_cnd_hz.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      sc_ftof_1a_sec[i]       = -1;
      sc_ftof_1a_time[i]      = NAN;
      sc_ftof_1a_path[i]      = NAN;
      sc_ftof_1a_energy[i]    = NAN;
      sc_ftof_1a_component[i] = -1;
      sc_ftof_1a_x[i]         = NAN;
      sc_ftof_1a_y[i]         = NAN;
      sc_ftof_1a_z[i]         = NAN;
      sc_ftof_1a_hx[i]        = NAN;
      sc_ftof_1a_hy[i]        = NAN;
      sc_ftof_1a_hz[i]        = NAN;

      sc_ftof_1b_sec[i]       = -1;
      sc_ftof_1b_time[i]      = NAN;
      sc_ftof_1b_path[i]      = NAN;
      sc_ftof_1b_energy[i]    = NAN;
      sc_ftof_1b_component[i] = -1;
      sc_ftof_1b_x[i]         = NAN;
      sc_ftof_1b_y[i]         = NAN;
      sc_ftof_1b_z[i]         = NAN;
      sc_ftof_1b_hx[i]        = NAN;
      sc_ftof_1b_hy[i]        = NAN;
      sc_ftof_1b_hz[i]        = NAN;

      sc_ftof_2_sec[i]       = -1;
      sc_ftof_2_time[i]      = NAN;
      sc_ftof_2_path[i]      = NAN;
      sc_ftof_2_energy[i]    = NAN;
      sc_ftof_2_component[i] = -1;
      sc_ftof_2_x[i]         = NAN;
      sc_ftof_2_y[i]         = NAN;
      sc_ftof_2_z[i]         = NAN;
      sc_ftof_2_hx[i]        = NAN;
      sc_ftof_2_hy[i]        = NAN;
      sc_ftof_2_hz[i]        = NAN;

      sc_ctof_time[i]      = NAN;
      sc_ctof_path[i]      = NAN;
      sc_ctof_energy[i]    = NAN;
      sc_ctof_component[i] = -1;
      sc_ctof_x[i]         = NAN;
      sc_ctof_y[i]         = NAN;
      sc_ctof_z[i]         = NAN;
      sc_ctof_hx[i]        = NAN;
      sc_ctof_hy[i]        = NAN;
      sc_ctof_hz[i]        = NAN;

      sc_cnd_time[i]      = NAN;
      sc_cnd_path[i]      = NAN;
      sc_cnd_energy[i]    = NAN;
      sc_cnd_component[i] = -1;
      sc_cnd_x[i]         = NAN;
      sc_cnd_y[i]         = NAN;
      sc_cnd_z[i]         = NAN;
      sc_cnd_hx[i]        = NAN;
      sc_cnd_hy[i]        = NAN;
      sc_cnd_hz[i]        = NAN;
    }

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Scintillator->getShort("pindex", k);
        int detector = rec_Scintillator->getByte("detector", k);
        int layer    = rec_Scintillator->getByte("layer", k);
        if (pindex == i && detector == FTOF && layer == FTOF_1A) {
          sc_ftof_1a_sec[i]       = rec_Scintillator->getByte("sector", k);
          sc_ftof_1a_time[i]      = rec_Scintillator->getFloat("time", k);
          sc_ftof_1a_path[i]      = rec_Scintillator->getFloat("path", k);
          sc_ftof_1a_energy[i]    = rec_Scintillator->getFloat("energy", k);
          sc_ftof_1a_component[i] = rec_Scintillator->getShort("component", k);
          sc_ftof_1a_x[i]         = rec_Scintillator->getFloat("x", k);
          sc_ftof_1a_y[i]         = rec_Scintillator->getFloat("y", k);
          sc_ftof_1a_z[i]         = rec_Scintillator->getFloat("z", k);
          sc_ftof_1a_hx[i]        = rec_Scintillator->getFloat("hx", k);
          sc_ftof_1a_hy[i]        = rec_Scintillator->getFloat("hy", k);
          sc_ftof_1a_hz[i]        = rec_Scintillator->getFloat("hz", k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_1B) {
          sc_ftof_1b_sec[i]       = rec_Scintillator->getByte("sector", k);
          sc_ftof_1b_time[i]      = rec_Scintillator->getFloat("time", k);
          sc_ftof_1b_path[i]      = rec_Scintillator->getFloat("path", k);
          sc_ftof_1b_energy[i]    = rec_Scintillator->getFloat("energy", k);
          sc_ftof_1b_component[i] = rec_Scintillator->getShort("component", k);
          sc_ftof_1b_x[i]         = rec_Scintillator->getFloat("x", k);
          sc_ftof_1b_y[i]         = rec_Scintillator->getFloat("y", k);
          sc_ftof_1b_z[i]         = rec_Scintillator->getFloat("z", k);
          sc_ftof_1b_hx[i]        = rec_Scintillator->getFloat("hx", k);
          sc_ftof_1b_hy[i]        = rec_Scintillator->getFloat("hy", k);
          sc_ftof_1b_hz[i]        = rec_Scintillator->getFloat("hz", k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_2) {
          sc_ftof_2_sec[i]       = rec_Scintillator->getByte("sector", k);
          sc_ftof_2_time[i]      = rec_Scintillator->getFloat("time", k);
          sc_ftof_2_path[i]      = rec_Scintillator->getFloat("path", k);
          sc_ftof_2_energy[i]    = rec_Scintillator->getFloat("energy", k);
          sc_ftof_2_component[i] = rec_Scintillator->getShort("component", k);
          sc_ftof_2_x[i]         = rec_Scintillator->getFloat("x", k);
          sc_ftof_2_y[i]         = rec_Scintillator->getFloat("y", k);
          sc_ftof_2_z[i]         = rec_Scintillator->getFloat("z", k);
          sc_ftof_2_hx[i]        = rec_Scintillator->getFloat("hx", k);
          sc_ftof_2_hy[i]        = rec_Scintillator->getFloat("hy", k);
          sc_ftof_2_hz[i]        = rec_Scintillator->getFloat("hz", k);
        } else if (pindex == i && detector == CTOF) {
          sc_ctof_time[i]      = rec_Scintillator->getFloat("time", k);
          sc_ctof_path[i]      = rec_Scintillator->getFloat("path", k);
          sc_ctof_energy[i]    = rec_Scintillator->getFloat("energy", k);
          sc_ctof_component[i] = rec_Scintillator->getShort("component", k);
          sc_ctof_x[i]         = rec_Scintillator->getFloat("x", k);
          sc_ctof_y[i]         = rec_Scintillator->getFloat("y", k);
          sc_ctof_z[i]         = rec_Scintillator->getFloat("z", k);
          sc_ctof_hx[i]        = rec_Scintillator->getFloat("hx", k);
          sc_ctof_hy[i]        = rec_Scintillator->getFloat("hy", k);
          sc_ctof_hz[i]        = rec_Scintillator->getFloat("hz", k);
        } else if (pindex == i && detector == CND) {
          sc_cnd_time[i]      = rec_Scintillator->getFloat("time", k);
          sc_cnd_path[i]      = rec_Scintillator->getFloat("path", k);
          sc_cnd_energy[i]    = rec_Scintillator->getFloat("energy", k);
          sc_cnd_component[i] = rec_Scintillator->getShort("component", k);
          sc_cnd_x[i]         = rec_Scintillator->getFloat("x", k);
          sc_cnd_y[i]         = rec_Scintillator->getFloat("y", k);
          sc_cnd_z[i]         = rec_Scintillator->getFloat("z", k);
          sc_cnd_hx[i]        = rec_Scintillator->getFloat("hx", k);
          sc_cnd_hy[i]        = rec_Scintillator->getFloat("hy", k);
          sc_cnd_hz[i]        = rec_Scintillator->getFloat("hz", k);
        }
      }
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Track->getRows();

    dc_sec.resize(len_pid);
    dc_px.resize(len_pid);
    dc_py.resize(len_pid);
    dc_pz.resize(len_pid);
    dc_vx.resize(len_pid);
    dc_vy.resize(len_pid);
    dc_vz.resize(len_pid);

    cvt_px.resize(len_pid);
    cvt_py.resize(len_pid);
    cvt_pz.resize(len_pid);
    cvt_vx.resize(len_pid);
    cvt_vy.resize(len_pid);
    cvt_vz.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      dc_sec[i] = -1;
      dc_px[i]  = NAN;
      dc_py[i]  = NAN;
      dc_pz[i]  = NAN;
      dc_vx[i]  = NAN;
      dc_vy[i]  = NAN;
      dc_vz[i]  = NAN;

      cvt_px[i] = NAN;
      cvt_py[i] = NAN;
      cvt_pz[i] = NAN;
      cvt_vx[i] = NAN;
      cvt_vy[i] = NAN;
      cvt_vz[i] = NAN;
    }

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Track->getShort("pindex", k);
        int detector = rec_Track->getByte("detector", k);

        if (pindex == i && detector == CVT) {
          cvt_px[i] = rec_Track->getFloat("px_nomm", k);
          cvt_py[i] = rec_Track->getFloat("py_nomm", k);
          cvt_pz[i] = rec_Track->getFloat("pz_nomm", k);
          cvt_vx[i] = rec_Track->getFloat("vx_nomm", k);
          cvt_vy[i] = rec_Track->getFloat("vy_nomm", k);
          cvt_vz[i] = rec_Track->getFloat("vz_nomm", k);

        } else if (pindex == i && detector == DC) {
          dc_sec[i] = rec_Track->getByte("sector", k);
          dc_px[i]  = rec_Track->getFloat("px_nomm", k);
          dc_py[i]  = rec_Track->getFloat("py_nomm", k);
          dc_pz[i]  = rec_Track->getFloat("pz_nomm", k);
          dc_vx[i]  = rec_Track->getFloat("vx_nomm", k);
          dc_vy[i]  = rec_Track->getFloat("vy_nomm", k);
          dc_vz[i]  = rec_Track->getFloat("vz_nomm", k);
        }
      }
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_ForwardTagger->getRows();

    ft_cal_energy.resize(len_pid);
    ft_cal_time.resize(len_pid);
    ft_cal_path.resize(len_pid);
    ft_cal_x.resize(len_pid);
    ft_cal_y.resize(len_pid);
    ft_cal_z.resize(len_pid);
    ft_cal_dx.resize(len_pid);
    ft_cal_dy.resize(len_pid);
    ft_cal_radius.resize(len_pid);

    ft_hodo_energy.resize(len_pid);
    ft_hodo_time.resize(len_pid);
    ft_hodo_path.resize(len_pid);
    ft_hodo_x.resize(len_pid);
    ft_hodo_y.resize(len_pid);
    ft_hodo_z.resize(len_pid);
    ft_hodo_dx.resize(len_pid);
    ft_hodo_dy.resize(len_pid);
    ft_hodo_radius.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      ft_cal_energy[i] = NAN;
      ft_cal_time[i]   = NAN;
      ft_cal_path[i]   = NAN;
      ft_cal_x[i]      = NAN;
      ft_cal_y[i]      = NAN;
      ft_cal_z[i]      = NAN;
      ft_cal_dx[i]     = NAN;
      ft_cal_dy[i]     = NAN;
      ft_cal_radius[i] = NAN;

      ft_hodo_energy[i] = NAN;
      ft_hodo_time[i]   = NAN;
      ft_hodo_path[i]   = NAN;
      ft_hodo_x[i]      = NAN;
      ft_hodo_y[i]      = NAN;
      ft_hodo_z[i]      = NAN;
      ft_hodo_dx[i]     = NAN;
      ft_hodo_dy[i]     = NAN;
      ft_hodo_radius[i] = NAN;
    }

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_ForwardTagger->getShort("pindex", k);
        int detector = rec_ForwardTagger->getByte("detector", k);

        if (pindex == i && detector == FTCAL) {
          ft_cal_energy[i] = rec_ForwardTagger->getFloat("energy", k);
          ft_cal_time[i]   = rec_ForwardTagger->getFloat("time", k);
          ft_cal_path[i]   = rec_ForwardTagger->getFloat("path", k);
          ft_cal_x[i]      = rec_ForwardTagger->getFloat("x", k);
          ft_cal_y[i]      = rec_ForwardTagger->getFloat("y", k);
          ft_cal_z[i]      = rec_ForwardTagger->getFloat("z", k);
          ft_cal_dx[i]     = rec_ForwardTagger->getFloat("dx", k);
          ft_cal_dy[i]     = rec_ForwardTagger->getFloat("dy", k);
          ft_cal_radius[i] = rec_ForwardTagger->getFloat("radius", k);
        } else if (pindex == i && detector == FTHODO) {
          ft_hodo_energy[i] = rec_ForwardTagger->getFloat("energy", k);
          ft_hodo_time[i]   = rec_ForwardTagger->getFloat("time", k);
          ft_hodo_path[i]   = rec_ForwardTagger->getFloat("path", k);
          ft_hodo_x[i]      = rec_ForwardTagger->getFloat("x", k);
          ft_hodo_y[i]      = rec_ForwardTagger->getFloat("y", k);
          ft_hodo_z[i]      = rec_ForwardTagger->getFloat("z", k);
          ft_hodo_dx[i]     = rec_ForwardTagger->getFloat("dx", k);
          ft_hodo_dy[i]     = rec_ForwardTagger->getFloat("dy", k);
          ft_hodo_radius[i] = rec_ForwardTagger->getFloat("radius", k);
        }
      }
    }

    if (cov) {
      len_pid    = rec_Particle->getRows();
      len_pindex = rec_CovMat->getRows();

      CovMat_11.resize(len_pid);
      CovMat_12.resize(len_pid);
      CovMat_13.resize(len_pid);
      CovMat_14.resize(len_pid);
      CovMat_15.resize(len_pid);
      CovMat_22.resize(len_pid);
      CovMat_23.resize(len_pid);
      CovMat_24.resize(len_pid);
      CovMat_25.resize(len_pid);
      CovMat_33.resize(len_pid);
      CovMat_34.resize(len_pid);
      CovMat_35.resize(len_pid);
      CovMat_44.resize(len_pid);
      CovMat_45.resize(len_pid);
      CovMat_55.resize(len_pid);

      for (int i = 0; i < len_pid; i++) {
        CovMat_11[i] = NAN;
        CovMat_12[i] = NAN;
        CovMat_13[i] = NAN;
        CovMat_14[i] = NAN;
        CovMat_15[i] = NAN;
        CovMat_22[i] = NAN;
        CovMat_23[i] = NAN;
        CovMat_24[i] = NAN;
        CovMat_25[i] = NAN;
        CovMat_33[i] = NAN;
        CovMat_34[i] = NAN;
        CovMat_35[i] = NAN;
        CovMat_44[i] = NAN;
        CovMat_45[i] = NAN;
        CovMat_55[i] = NAN;
      }

      for (int i = 0; i < len_pid; i++) {
        for (int k = 0; k < len_pindex; ++k) {
          int pindex = rec_CovMat->getShort("pindex", k);
          if (pindex == i) {
            CovMat_11[i] = rec_CovMat->getFloat("C11", k);
            CovMat_12[i] = rec_CovMat->getFloat("C12", k);
            CovMat_13[i] = rec_CovMat->getFloat("C13", k);
            CovMat_14[i] = rec_CovMat->getFloat("C14", k);
            CovMat_15[i] = rec_CovMat->getFloat("C15", k);
            CovMat_22[i] = rec_CovMat->getFloat("C22", k);
            CovMat_23[i] = rec_CovMat->getFloat("C23", k);
            CovMat_24[i] = rec_CovMat->getFloat("C24", k);
            CovMat_25[i] = rec_CovMat->getFloat("C25", k);
            CovMat_33[i] = rec_CovMat->getFloat("C33", k);
            CovMat_34[i] = rec_CovMat->getFloat("C34", k);
            CovMat_35[i] = rec_CovMat->getFloat("C35", k);
            CovMat_44[i] = rec_CovMat->getFloat("C44", k);
            CovMat_45[i] = rec_CovMat->getFloat("C45", k);
            CovMat_55[i] = rec_CovMat->getFloat("C55", k);
          }
        }
      }
    }

    clas12->Fill();
  }

  OutputFile->cd();
  clas12->Write();
  OutputFile->Close();

  if (!is_batch) {
    std::chrono::duration<double> elapsed_full =
        (std::chrono::high_resolution_clock::now() - start_full);
    std::cout << "Elapsed time: " << elapsed_full.count() << " s" << std::endl;
    std::cout << "Events/Sec: " << entry / elapsed_full.count() << " Hz" << std::endl;
  }

  return 0;
}
