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
#include "TROOT.h"
#include "TTree.h"
// Hipo libs
#include "hipo4/reader.h"

// Headers in same folder
#include "banks.h"
#include "clipp.h"
#include "constants.h"

void init(TTree* clas12, bool is_mc, bool cov, bool traj) {

  clas12->Branch("run", &run);
  clas12->Branch("event", &event);
  clas12->Branch("unixtime", &unixtime);
  clas12->Branch("trigger", &trigger);
  clas12->Branch("timestamp", &timestamp);
  clas12->Branch("type", &type);
  clas12->Branch("mode", &mode);
  clas12->Branch("torus", &torus);
  clas12->Branch("solenoid", &solenoid);

  clas12->Branch("category", &category);
  clas12->Branch("topology", &topology);
  clas12->Branch("beamCharge", &beamCharge);
  clas12->Branch("liveTime", &liveTime);
  clas12->Branch("startTime", &startTime);
  clas12->Branch("RFTime", &RFTime);
  clas12->Branch("helicity", &helicity);
  clas12->Branch("helicityRaw", &helicityRaw);
  clas12->Branch("procTime", &procTime);

  clas12->Branch("hel_run", &hel_run);
  clas12->Branch("hel_event", &hel_event);
  clas12->Branch("hel_timestamp", &hel_timestamp);
  clas12->Branch("hel_helicity", &hel_helicity);
  clas12->Branch("hel_helicityRaw", &hel_helicityRaw);
  clas12->Branch("hel_pair", &hel_pair);
  clas12->Branch("hel_pattern", &hel_pattern);
  clas12->Branch("hel_status", &hel_status);

  clas12->Branch("pid", &pid);
  clas12->Branch("p", &p);
  clas12->Branch("p2", &p2);
  clas12->Branch("px", &px);
  clas12->Branch("py", &py);
  clas12->Branch("pz", &pz);
  clas12->Branch("vx", &vx);
  clas12->Branch("vy", &vy);
  clas12->Branch("vz", &vz);
  clas12->Branch("vt", &vt);
  clas12->Branch("charge", &charge);
  clas12->Branch("beta", &beta);
  clas12->Branch("chi2pid", &chi2pid);
  clas12->Branch("status", &status);

  if (is_mc) {
    clas12->Branch("mc_npart", &mc_npart);
    clas12->Branch("mc_ebeam", &mc_ebeam);
    clas12->Branch("mc_weight", &mc_weight);
    clas12->Branch("mc_helicity", &mc_helicity);

    clas12->Branch("mc_pid", &mc_pid_vec);
    clas12->Branch("mc_px", &mc_px_vec);
    clas12->Branch("mc_py", &mc_py_vec);
    clas12->Branch("mc_pz", &mc_pz_vec);
    clas12->Branch("mc_vx", &mc_vx_vec);
    clas12->Branch("mc_vy", &mc_vy_vec);
    clas12->Branch("mc_vz", &mc_vz_vec);
    clas12->Branch("mc_vt", &mc_vt_vec);
  }

  clas12->Branch("dc_sec", &dc_sec);
  clas12->Branch("dc_r1_x", &dc_r1_x);
  clas12->Branch("dc_r1_y", &dc_r1_y);
  clas12->Branch("dc_r1_z", &dc_r1_z);
  clas12->Branch("dc_r2_x", &dc_r2_x);
  clas12->Branch("dc_r2_y", &dc_r2_y);
  clas12->Branch("dc_r2_z", &dc_r2_z);
  clas12->Branch("dc_r3_x", &dc_r3_x);
  clas12->Branch("dc_r3_y", &dc_r3_y);
  clas12->Branch("dc_r3_z", &dc_r3_z);
  clas12->Branch("cvt_x", &cvt_x);
  clas12->Branch("cvt_y", &cvt_y);
  clas12->Branch("cvt_z", &cvt_z);

  clas12->Branch("fmt_x", &fmt_x);
  clas12->Branch("fmt_y", &fmt_y);
  clas12->Branch("fmt_z", &fmt_z);

  clas12->Branch("ec_tot_energy", &ec_tot_energy);
  clas12->Branch("ec_pcal_energy", &ec_pcal_energy);
  clas12->Branch("ec_pcal_sec", &ec_pcal_sec);
  clas12->Branch("ec_pcal_time", &ec_pcal_time);
  clas12->Branch("ec_pcal_path", &ec_pcal_path);
  clas12->Branch("ec_pcal_x", &ec_pcal_x);
  clas12->Branch("ec_pcal_y", &ec_pcal_y);
  clas12->Branch("ec_pcal_z", &ec_pcal_z);
  clas12->Branch("ec_pcal_hx", &ec_pcal_hx);
  clas12->Branch("ec_pcal_hy", &ec_pcal_hy);
  clas12->Branch("ec_pcal_hz", &ec_pcal_hz);
  clas12->Branch("ec_pcal_lu", &ec_pcal_lu);
  clas12->Branch("ec_pcal_lv", &ec_pcal_lv);
  clas12->Branch("ec_pcal_lw", &ec_pcal_lw);
  clas12->Branch("ec_pcal_du", &ec_pcal_du);
  clas12->Branch("ec_pcal_dv", &ec_pcal_dv);
  clas12->Branch("ec_pcal_dw", &ec_pcal_dw);
  clas12->Branch("ec_pcal_m2u", &ec_pcal_m2u);
  clas12->Branch("ec_pcal_m2v", &ec_pcal_m2v);
  clas12->Branch("ec_pcal_m2w", &ec_pcal_m2w);
  clas12->Branch("ec_pcal_m3u", &ec_pcal_m3u);
  clas12->Branch("ec_pcal_m3v", &ec_pcal_m3v);
  clas12->Branch("ec_pcal_m3w", &ec_pcal_m3w);

  clas12->Branch("ec_ecin_energy", &ec_ecin_energy);
  clas12->Branch("ec_ecin_sec", &ec_ecin_sec);
  clas12->Branch("ec_ecin_time", &ec_ecin_time);
  clas12->Branch("ec_ecin_path", &ec_ecin_path);
  clas12->Branch("ec_ecin_x", &ec_ecin_x);
  clas12->Branch("ec_ecin_y", &ec_ecin_y);
  clas12->Branch("ec_ecin_z", &ec_ecin_z);
  clas12->Branch("ec_ecin_hx", &ec_pcal_hx);
  clas12->Branch("ec_ecin_hy", &ec_pcal_hy);
  clas12->Branch("ec_ecin_hz", &ec_pcal_hz);
  clas12->Branch("ec_ecin_lu", &ec_pcal_lu);
  clas12->Branch("ec_ecin_lv", &ec_pcal_lv);
  clas12->Branch("ec_ecin_lw", &ec_pcal_lw);
  clas12->Branch("ec_ecin_du", &ec_pcal_du);
  clas12->Branch("ec_ecin_dv", &ec_pcal_dv);
  clas12->Branch("ec_ecin_dw", &ec_pcal_dw);
  clas12->Branch("ec_ecin_m2u", &ec_pcal_m2u);
  clas12->Branch("ec_ecin_m2v", &ec_pcal_m2v);
  clas12->Branch("ec_ecin_m2w", &ec_pcal_m2w);
  clas12->Branch("ec_ecin_m3u", &ec_pcal_m3u);
  clas12->Branch("ec_ecin_m3v", &ec_pcal_m3v);
  clas12->Branch("ec_ecin_m3w", &ec_pcal_m3w);

  clas12->Branch("ec_ecout_energy", &ec_ecout_energy);
  clas12->Branch("ec_ecout_sec", &ec_ecout_sec);
  clas12->Branch("ec_ecout_time", &ec_ecout_time);
  clas12->Branch("ec_ecout_path", &ec_ecout_path);
  clas12->Branch("ec_ecout_x", &ec_ecout_x);
  clas12->Branch("ec_ecout_y", &ec_ecout_y);
  clas12->Branch("ec_ecout_z", &ec_ecout_z);
  clas12->Branch("ec_ecout_hx", &ec_pcal_hx);
  clas12->Branch("ec_ecout_hy", &ec_pcal_hy);
  clas12->Branch("ec_ecout_hz", &ec_pcal_hz);
  clas12->Branch("ec_ecout_lu", &ec_pcal_lu);
  clas12->Branch("ec_ecout_lv", &ec_pcal_lv);
  clas12->Branch("ec_ecout_lw", &ec_pcal_lw);
  clas12->Branch("ec_ecout_du", &ec_pcal_du);
  clas12->Branch("ec_ecout_dv", &ec_pcal_dv);
  clas12->Branch("ec_ecout_dw", &ec_pcal_dw);
  clas12->Branch("ec_ecout_m2u", &ec_pcal_m2u);
  clas12->Branch("ec_ecout_m2v", &ec_pcal_m2v);
  clas12->Branch("ec_ecout_m2w", &ec_pcal_m2w);
  clas12->Branch("ec_ecout_m3u", &ec_pcal_m3u);
  clas12->Branch("ec_ecout_m3v", &ec_pcal_m3v);
  clas12->Branch("ec_ecout_m3w", &ec_pcal_m3w);

  clas12->Branch("cc_nphe_tot", &cc_nphe_tot);
  clas12->Branch("cc_ltcc_sec", &cc_ltcc_sec);
  clas12->Branch("cc_ltcc_nphe", &cc_ltcc_nphe);
  clas12->Branch("cc_ltcc_time", &cc_ltcc_time);
  clas12->Branch("cc_ltcc_path", &cc_ltcc_path);
  clas12->Branch("cc_ltcc_theta", &cc_ltcc_theta);
  clas12->Branch("cc_ltcc_phi", &cc_ltcc_phi);
  clas12->Branch("cc_ltcc_x", &cc_ltcc_x);
  clas12->Branch("cc_ltcc_y", &cc_ltcc_y);
  clas12->Branch("cc_ltcc_z", &cc_ltcc_z);

  clas12->Branch("cc_htcc_sec", &cc_htcc_sec);
  clas12->Branch("cc_htcc_nphe", &cc_htcc_nphe);
  clas12->Branch("cc_htcc_time", &cc_htcc_time);
  clas12->Branch("cc_htcc_path", &cc_htcc_path);
  clas12->Branch("cc_htcc_theta", &cc_htcc_theta);
  clas12->Branch("cc_htcc_phi", &cc_htcc_phi);
  clas12->Branch("cc_htcc_x", &cc_htcc_x);
  clas12->Branch("cc_htcc_y", &cc_htcc_y);
  clas12->Branch("cc_htcc_z", &cc_htcc_z);

  clas12->Branch("cc_rich_sec", &cc_rich_sec);
  clas12->Branch("cc_rich_nphe", &cc_rich_nphe);
  clas12->Branch("cc_rich_time", &cc_rich_time);
  clas12->Branch("cc_rich_path", &cc_rich_path);
  clas12->Branch("cc_rich_theta", &cc_rich_theta);
  clas12->Branch("cc_rich_phi", &cc_rich_phi);
  clas12->Branch("cc_rich_x", &cc_rich_x);
  clas12->Branch("cc_rich_y", &cc_rich_y);
  clas12->Branch("cc_rich_z", &cc_rich_z);

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
  if (traj) {
    clas12->Branch("traj_pindex", &traj_pindex_vec);
    clas12->Branch("traj_index", &traj_index_vec);
    clas12->Branch("traj_detId", &traj_detId_vec);
    clas12->Branch("traj_q", &traj_q_vec);
    clas12->Branch("traj_x", &traj_x_vec);
    clas12->Branch("traj_y", &traj_y_vec);
    clas12->Branch("traj_z", &traj_z_vec);
    clas12->Branch("traj_cx", &traj_cx_vec);
    clas12->Branch("traj_cy", &traj_cy_vec);
    clas12->Branch("traj_cz", &traj_cz_vec);
    clas12->Branch("traj_pathlength", &traj_pathlength_vec);
  }
}

int main(int argc, char** argv) {
  ROOT::EnableThreadSafety();
  ROOT::EnableImplicitMT(2);
  std::string InFileName;
  std::string OutFileName;
  bool        is_mc      = false;
  bool        is_batch   = false;
  bool        is_test    = false;
  bool        print_help = false;
  bool        good_rec   = false;
  bool        elec_first = false;
  bool        cov        = false;
  bool        traj       = false;
  float       max_size   = 1500;

  auto cli = (clipp::option("-h", "--help").set(print_help) % "print help",
              clipp::option("-mc", "--MC").set(is_mc) % "Convert dst and mc banks",
              clipp::option("-b", "--batch").set(is_batch) % "Don't show progress and statistics",
              clipp::option("-r", "--rec").set(good_rec) %
                  "Only save events where number of partilces in the event > 0",
              clipp::option("-e", "--elec").set(elec_first) %
                  "Only save events with good electron as first particle",
              clipp::option("-c", "--cov").set(cov) % "Save Covariant Matrix for kinematic fitting",
              clipp::option("-t", "--traj").set(traj) % "Save traj information",
              clipp::option("-test", "--test").set(is_test) % "Testing",
              clipp::option("-m", "--max_file_size") &
                  clipp::value("max_size", max_size) % "Max file size in GB (150GB default)",
              clipp::value("inputFile.hipo", InFileName),
              clipp::opt_value("outputFile.root", OutFileName));

  clipp::parse(argc, argv, cli);
  if (print_help || InFileName.empty()) {
    std::cout << clipp::make_man_page(cli, argv[0]);
    exit(0);
  }

  TFile* OutputFile;
  TTree* clas12;

  if (OutFileName.empty())
    OutFileName = InFileName + ".root";

  OutputFile = new TFile(OutFileName.c_str(), "RECREATE");
  OutputFile->SetCompressionSettings(404); // kUseAnalysis
  clas12                  = new TTree("clas12", "clas12");
  long long max_tree_size = 1000000000LL * max_size;
  clas12->SetMaxTreeSize(max_tree_size);

  auto   reader          = std::make_shared<hipo::reader>(InFileName);
  size_t tot_hipo_events = reader->numEvents();

  auto dict = std::make_shared<hipo::dictionary>();
  reader->readDictionary(*dict);
  auto hipo_event = std::make_shared<hipo::event>();

  // Event config
  auto run_Config = std::make_shared<hipo::bank>(dict->getSchema("RUN::config"));
  auto rec_Event  = std::make_shared<hipo::bank>(dict->getSchema("REC::Event"));
  auto hel_Flip   = std::make_shared<hipo::bank>(dict->getSchema("HEL::flip"));

  // Physics
  auto rec_Particle      = std::make_shared<hipo::bank>(dict->getSchema("REC::Particle"));
  auto rec_Calorimeter   = std::make_shared<hipo::bank>(dict->getSchema("REC::Calorimeter"));
  auto rec_Scintillator  = std::make_shared<hipo::bank>(dict->getSchema("REC::Scintillator"));
  auto rec_Cherenkov     = std::make_shared<hipo::bank>(dict->getSchema("REC::Cherenkov"));
  auto rec_Track         = std::make_shared<hipo::bank>(dict->getSchema("REC::Track"));
  auto rec_ForwardTagger = std::make_shared<hipo::bank>(dict->getSchema("REC::ForwardTagger"));
  auto rec_Traj          = std::make_shared<hipo::bank>(dict->getSchema("REC::Traj"));
  auto rec_CovMat        = std::make_shared<hipo::bank>(dict->getSchema("REC::CovMat"));

  // Monte Carlo only banks
  auto mc_Header   = std::make_shared<hipo::bank>(dict->getSchema("MC::Header"));
  auto mc_Event    = std::make_shared<hipo::bank>(dict->getSchema("MC::Event"));
  auto mc_Particle = std::make_shared<hipo::bank>(dict->getSchema("MC::Particle"));
  auto mc_Lund     = std::make_shared<hipo::bank>(dict->getSchema("MC::Lund"));

  init(clas12, is_mc, cov, traj);

  int  entry                = 0;
  int  l                    = 0;
  int  len_pid              = 0;
  int  len_pindex           = 0;
  int  tot_events_processed = 0;
  auto start_full           = std::chrono::high_resolution_clock::now();
  while (reader->next()) {
    if (is_test && entry > 50000)
      break;
    reader->read(*hipo_event);
    hipo_event->getStructure(*rec_Event);
    hipo_event->getStructure(*run_Config);
    hipo_event->getStructure(*hel_Flip);
    hipo_event->getStructure(*rec_Particle);
    hipo_event->getStructure(*rec_ForwardTagger);
    hipo_event->getStructure(*rec_Track);
    hipo_event->getStructure(*rec_Cherenkov);
    hipo_event->getStructure(*rec_Scintillator);
    hipo_event->getStructure(*rec_Calorimeter);
    hipo_event->getStructure(*rec_Traj);
    if (cov)
      hipo_event->getStructure(*rec_CovMat);
    if (is_mc) {
      hipo_event->getStructure(*mc_Header);
      hipo_event->getStructure(*mc_Particle);
      hipo_event->getStructure(*mc_Event);
      hipo_event->getStructure(*mc_Lund);
    }

    if (!is_batch && (++entry % 10000) == 0)
      std::cout << "\t" << floor(100 * entry / tot_hipo_events) << "%\r\r" << std::flush;
    tot_events_processed++;

    l = rec_Event->getRows();
    if (l != -1) {
      category    = rec_Event->getLong(0, 0);
      topology    = rec_Event->getLong(1, 0);
      beamCharge  = rec_Event->getFloat(2, 0);
      liveTime    = rec_Event->getDouble(3, 0);
      startTime   = ((rec_Event->getFloat(4, 0) != -1000) ? rec_Event->getFloat(4, 0) : NAN);
      RFTime      = rec_Event->getFloat(5, 0);
      helicity    = rec_Event->getByte(6, 0);
      helicityRaw = rec_Event->getByte(7, 0);
      procTime    = rec_Event->getFloat(8, 0);
    }

    l = run_Config->getRows();
    if (l != -1) {
      run       = run_Config->getInt(0, 0);
      event     = run_Config->getInt(1, 0);
      unixtime  = run_Config->getInt(2, 0);
      trigger   = run_Config->getLong(3, 0);
      timestamp = run_Config->getLong(4, 0);
      type      = run_Config->getInt(5, 0);
      mode      = run_Config->getInt(6, 0);
      torus     = run_Config->getFloat(7, 0);
      solenoid  = run_Config->getFloat(8, 0);
    }

    l = hel_Flip->getRows();
    if (l != -1) {
      int hel = hel_Flip->getInt(3, 0);
      if (hel == 1 || hel == -1) {
        hel_run         = hel_Flip->getInt(0, 0);
        hel_event       = hel_Flip->getInt(1, 0);
        hel_timestamp   = hel_Flip->getLong(2, 0);
        hel_helicity    = hel_Flip->getInt(3, 0);
        hel_helicityRaw = hel_Flip->getInt(4, 0);
        hel_pair        = hel_Flip->getInt(5, 0);
        hel_pattern     = hel_Flip->getInt(6, 0);
        hel_status      = hel_Flip->getInt(7, 0);
      }
    }

    if (is_mc) {
      l = mc_Header->getRows();
      if (l != -1) {
        mc_run      = mc_Header->getInt(0, 0);
        mc_event    = mc_Header->getInt(1, 0);
        mc_type     = mc_Header->getInt(2, 0);
        mc_helicity = mc_Header->getFloat(3, 0);
      }

      l = mc_Event->getRows();
      if (l != -1) {
        mc_npart  = mc_Event->getInt(0, 0);
        mc_ebeam  = mc_Event->getFloat(6, 0);
        mc_weight = mc_Event->getFloat(9, 0);
      }

      l = mc_Particle->getRows();
      if (l != -1) {
        mc_pid_vec.resize(l);
        mc_px_vec.resize(l);
        mc_py_vec.resize(l);
        mc_pz_vec.resize(l);
        mc_vx_vec.resize(l);
        mc_vy_vec.resize(l);
        mc_vz_vec.resize(l);
        mc_vt_vec.resize(l);
        for (int i = 0; i < l; i++) {
          mc_pid_vec[i] = mc_Particle->getInt(0, i);
          mc_px_vec[i]  = mc_Particle->getFloat(1, i);
          mc_py_vec[i]  = mc_Particle->getFloat(2, i);
          mc_pz_vec[i]  = mc_Particle->getFloat(3, i);
          mc_vx_vec[i]  = mc_Particle->getFloat(4, i);
          mc_vy_vec[i]  = mc_Particle->getFloat(5, i);
          mc_vz_vec[i]  = mc_Particle->getFloat(6, i);
          mc_vt_vec[i]  = mc_Particle->getFloat(7, i);
        }
      }
    }

    if (good_rec && rec_Particle->getRows() == -1)
      continue;
    if (elec_first && (rec_Particle->getInt(0, 0) != 11 || rec_Particle->getInt(0, 0) != 0))
      continue;

    l = rec_Particle->getRows();
    if (l == -1)
      continue;

    pid.resize(l);
    p.resize(l);
    p2.resize(l);
    px.resize(l);
    py.resize(l);
    pz.resize(l);
    vx.resize(l);
    vy.resize(l);
    vz.resize(l);
    vt.resize(l);
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
      vt[i]     = rec_Particle->getFloat("vt", i);
      charge[i] = rec_Particle->getInt("charge", i);
      beta[i] =
          ((rec_Particle->getFloat("beta", i) != -9999) ? rec_Particle->getFloat("beta", i) : NAN);
      chi2pid[i] = rec_Particle->getFloat("chi2pid", i);
      status[i]  = rec_Particle->getInt("status", i);
    }

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
    ec_pcal_hx.resize(len_pid);
    ec_pcal_hy.resize(len_pid);
    ec_pcal_hz.resize(len_pid);
    ec_pcal_lu.resize(len_pid);
    ec_pcal_lv.resize(len_pid);
    ec_pcal_lw.resize(len_pid);
    ec_pcal_du.resize(len_pid);
    ec_pcal_dv.resize(len_pid);
    ec_pcal_dw.resize(len_pid);
    ec_pcal_m2u.resize(len_pid);
    ec_pcal_m2v.resize(len_pid);
    ec_pcal_m2w.resize(len_pid);
    ec_pcal_m3u.resize(len_pid);
    ec_pcal_m3v.resize(len_pid);
    ec_pcal_m3w.resize(len_pid);

    ec_ecin_energy.resize(len_pid);
    ec_ecin_sec.resize(len_pid);
    ec_ecin_time.resize(len_pid);
    ec_ecin_path.resize(len_pid);
    ec_ecin_x.resize(len_pid);
    ec_ecin_y.resize(len_pid);
    ec_ecin_z.resize(len_pid);
    ec_ecin_hx.resize(len_pid);
    ec_ecin_hy.resize(len_pid);
    ec_ecin_hz.resize(len_pid);
    ec_ecin_lu.resize(len_pid);
    ec_ecin_lv.resize(len_pid);
    ec_ecin_lw.resize(len_pid);
    ec_ecin_du.resize(len_pid);
    ec_ecin_dv.resize(len_pid);
    ec_ecin_dw.resize(len_pid);
    ec_ecin_m2u.resize(len_pid);
    ec_ecin_m2v.resize(len_pid);
    ec_ecin_m2w.resize(len_pid);
    ec_ecin_m3u.resize(len_pid);
    ec_ecin_m3v.resize(len_pid);
    ec_ecin_m3w.resize(len_pid);

    ec_ecout_energy.resize(len_pid);
    ec_ecout_sec.resize(len_pid);
    ec_ecout_time.resize(len_pid);
    ec_ecout_path.resize(len_pid);
    ec_ecout_x.resize(len_pid);
    ec_ecout_y.resize(len_pid);
    ec_ecout_z.resize(len_pid);
    ec_ecout_hx.resize(len_pid);
    ec_ecout_hy.resize(len_pid);
    ec_ecout_hz.resize(len_pid);
    ec_ecout_lu.resize(len_pid);
    ec_ecout_lv.resize(len_pid);
    ec_ecout_lw.resize(len_pid);
    ec_ecout_du.resize(len_pid);
    ec_ecout_dv.resize(len_pid);
    ec_ecout_dw.resize(len_pid);
    ec_ecout_m2u.resize(len_pid);
    ec_ecout_m2v.resize(len_pid);
    ec_ecout_m2w.resize(len_pid);
    ec_ecout_m3u.resize(len_pid);
    ec_ecout_m3v.resize(len_pid);
    ec_ecout_m3w.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      ec_tot_energy[i]  = NAN;
      ec_pcal_energy[i] = NAN;
      ec_pcal_sec[i]    = -1;
      ec_pcal_time[i]   = NAN;
      ec_pcal_path[i]   = NAN;
      ec_pcal_x[i]      = NAN;
      ec_pcal_y[i]      = NAN;
      ec_pcal_z[i]      = NAN;
      ec_pcal_hx[i]     = NAN;
      ec_pcal_hy[i]     = NAN;
      ec_pcal_hz[i]     = NAN;
      ec_pcal_lu[i]     = NAN;
      ec_pcal_lv[i]     = NAN;
      ec_pcal_lw[i]     = NAN;
      ec_pcal_du[i]     = NAN;
      ec_pcal_dv[i]     = NAN;
      ec_pcal_dw[i]     = NAN;
      ec_pcal_m2u[i]    = NAN;
      ec_pcal_m2v[i]    = NAN;
      ec_pcal_m2w[i]    = NAN;
      ec_pcal_m3u[i]    = NAN;
      ec_pcal_m3v[i]    = NAN;
      ec_pcal_m3w[i]    = NAN;

      ec_ecin_energy[i] = NAN;
      ec_ecin_sec[i]    = -1;
      ec_ecin_time[i]   = NAN;
      ec_ecin_path[i]   = NAN;
      ec_ecin_x[i]      = NAN;
      ec_ecin_y[i]      = NAN;
      ec_ecin_z[i]      = NAN;
      ec_ecin_hx[i]     = NAN;
      ec_ecin_hy[i]     = NAN;
      ec_ecin_hz[i]     = NAN;
      ec_ecin_lu[i]     = NAN;
      ec_ecin_lv[i]     = NAN;
      ec_ecin_lw[i]     = NAN;
      ec_ecin_du[i]     = NAN;
      ec_ecin_dv[i]     = NAN;
      ec_ecin_dw[i]     = NAN;
      ec_ecin_m2u[i]    = NAN;
      ec_ecin_m2v[i]    = NAN;
      ec_ecin_m2w[i]    = NAN;
      ec_ecin_m3u[i]    = NAN;
      ec_ecin_m3v[i]    = NAN;
      ec_ecin_m3w[i]    = NAN;

      ec_ecout_energy[i] = NAN;
      ec_ecout_sec[i]    = -1;
      ec_ecout_time[i]   = NAN;
      ec_ecout_path[i]   = NAN;
      ec_ecout_x[i]      = NAN;
      ec_ecout_y[i]      = NAN;
      ec_ecout_z[i]      = NAN;
      ec_ecout_hx[i]     = NAN;
      ec_ecout_hy[i]     = NAN;
      ec_ecout_hz[i]     = NAN;
      ec_ecout_lu[i]     = NAN;
      ec_ecout_lv[i]     = NAN;
      ec_ecout_lw[i]     = NAN;
      ec_ecout_du[i]     = NAN;
      ec_ecout_dv[i]     = NAN;
      ec_ecout_dw[i]     = NAN;
      ec_ecout_m2u[i]    = NAN;
      ec_ecout_m2v[i]    = NAN;
      ec_ecout_m2w[i]    = NAN;
      ec_ecout_m3u[i]    = NAN;
      ec_ecout_m3v[i]    = NAN;
      ec_ecout_m3w[i]    = NAN;
    }

    float pcal   = 0.0;
    float einner = 0.0;
    float eouter = 0.0;
    float etot   = 0.0;

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int   pindex   = rec_Calorimeter->getInt(1, k);
        int   detector = rec_Calorimeter->getInt(2, k);
        int   layer    = rec_Calorimeter->getInt(4, k);
        float energy   = rec_Calorimeter->getFloat(5, k);

        if (pindex == i && detector == ECAL) {
          etot += energy;
          if (layer == PCAL) {
            pcal += energy;
            ec_pcal_sec[i]  = rec_Calorimeter->getInt(3, k);
            ec_pcal_time[i] = rec_Calorimeter->getFloat(6, k);
            ec_pcal_path[i] = rec_Calorimeter->getFloat(7, k);
            ec_pcal_x[i]    = rec_Calorimeter->getFloat(9, k);
            ec_pcal_y[i]    = rec_Calorimeter->getFloat(10, k);
            ec_pcal_z[i]    = rec_Calorimeter->getFloat(11, k);
            ec_pcal_hx[i]   = rec_Calorimeter->getFloat(12, k);
            ec_pcal_hy[i]   = rec_Calorimeter->getFloat(13, k);
            ec_pcal_hz[i]   = rec_Calorimeter->getFloat(14, k);
            ec_pcal_lu[i]   = rec_Calorimeter->getFloat(15, k);
            ec_pcal_lv[i]   = rec_Calorimeter->getFloat(16, k);
            ec_pcal_lw[i]   = rec_Calorimeter->getFloat(17, k);
            ec_pcal_du[i]   = rec_Calorimeter->getFloat(18, k);
            ec_pcal_dv[i]   = rec_Calorimeter->getFloat(19, k);
            ec_pcal_dw[i]   = rec_Calorimeter->getFloat(20, k);
            ec_pcal_m2u[i]  = rec_Calorimeter->getFloat(21, k);
            ec_pcal_m2v[i]  = rec_Calorimeter->getFloat(22, k);
            ec_pcal_m2w[i]  = rec_Calorimeter->getFloat(23, k);
            ec_pcal_m3u[i]  = rec_Calorimeter->getFloat(24, k);
            ec_pcal_m3v[i]  = rec_Calorimeter->getFloat(25, k);
            ec_pcal_m3w[i]  = rec_Calorimeter->getFloat(26, k);
          } else if (layer == EC_INNER) {
            einner += energy;
            ec_ecin_sec[i]  = rec_Calorimeter->getInt(3, k);
            ec_ecin_time[i] = rec_Calorimeter->getFloat(6, k);
            ec_ecin_path[i] = rec_Calorimeter->getFloat(7, k);
            ec_ecin_x[i]    = rec_Calorimeter->getFloat(9, k);
            ec_ecin_y[i]    = rec_Calorimeter->getFloat(10, k);
            ec_ecin_z[i]    = rec_Calorimeter->getFloat(11, k);
            ec_ecin_hx[i]   = rec_Calorimeter->getFloat(12, k);
            ec_ecin_hy[i]   = rec_Calorimeter->getFloat(13, k);
            ec_ecin_hz[i]   = rec_Calorimeter->getFloat(14, k);
            ec_ecin_lu[i]   = rec_Calorimeter->getFloat(15, k);
            ec_ecin_lv[i]   = rec_Calorimeter->getFloat(16, k);
            ec_ecin_lw[i]   = rec_Calorimeter->getFloat(17, k);
            ec_ecin_du[i]   = rec_Calorimeter->getFloat(18, k);
            ec_ecin_dv[i]   = rec_Calorimeter->getFloat(19, k);
            ec_ecin_dw[i]   = rec_Calorimeter->getFloat(20, k);
            ec_ecin_m2u[i]  = rec_Calorimeter->getFloat(21, k);
            ec_ecin_m2v[i]  = rec_Calorimeter->getFloat(22, k);
            ec_ecin_m2w[i]  = rec_Calorimeter->getFloat(23, k);
            ec_ecin_m3u[i]  = rec_Calorimeter->getFloat(24, k);
            ec_ecin_m3v[i]  = rec_Calorimeter->getFloat(25, k);
            ec_ecin_m3w[i]  = rec_Calorimeter->getFloat(26, k);
          } else if (layer == EC_OUTER) {
            eouter += energy;
            ec_ecout_sec[i]  = rec_Calorimeter->getInt(3, k);
            ec_ecout_time[i] = rec_Calorimeter->getFloat(6, k);
            ec_ecout_path[i] = rec_Calorimeter->getFloat(7, k);
            ec_ecout_x[i]    = rec_Calorimeter->getFloat(9, k);
            ec_ecout_y[i]    = rec_Calorimeter->getFloat(10, k);
            ec_ecout_z[i]    = rec_Calorimeter->getFloat(11, k);
            ec_ecout_hx[i]   = rec_Calorimeter->getFloat(12, k);
            ec_ecout_hy[i]   = rec_Calorimeter->getFloat(13, k);
            ec_ecout_hz[i]   = rec_Calorimeter->getFloat(14, k);
            ec_ecout_lu[i]   = rec_Calorimeter->getFloat(15, k);
            ec_ecout_lv[i]   = rec_Calorimeter->getFloat(16, k);
            ec_ecout_lw[i]   = rec_Calorimeter->getFloat(17, k);
            ec_ecout_du[i]   = rec_Calorimeter->getFloat(18, k);
            ec_ecout_dv[i]   = rec_Calorimeter->getFloat(19, k);
            ec_ecout_dw[i]   = rec_Calorimeter->getFloat(20, k);
            ec_ecout_m2u[i]  = rec_Calorimeter->getFloat(21, k);
            ec_ecout_m2v[i]  = rec_Calorimeter->getFloat(22, k);
            ec_ecout_m2w[i]  = rec_Calorimeter->getFloat(23, k);
            ec_ecout_m3u[i]  = rec_Calorimeter->getFloat(24, k);
            ec_ecout_m3v[i]  = rec_Calorimeter->getFloat(25, k);
            ec_ecout_m3w[i]  = rec_Calorimeter->getFloat(26, k);
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
    cc_ltcc_x.resize(len_pid);
    cc_ltcc_y.resize(len_pid);
    cc_ltcc_z.resize(len_pid);

    cc_htcc_sec.resize(len_pid);
    cc_htcc_nphe.resize(len_pid);
    cc_htcc_time.resize(len_pid);
    cc_htcc_path.resize(len_pid);
    cc_htcc_theta.resize(len_pid);
    cc_htcc_phi.resize(len_pid);
    cc_htcc_x.resize(len_pid);
    cc_htcc_y.resize(len_pid);
    cc_htcc_z.resize(len_pid);

    cc_rich_sec.resize(len_pid);
    cc_rich_nphe.resize(len_pid);
    cc_rich_time.resize(len_pid);
    cc_rich_path.resize(len_pid);
    cc_rich_theta.resize(len_pid);
    cc_rich_phi.resize(len_pid);
    cc_rich_x.resize(len_pid);
    cc_rich_y.resize(len_pid);
    cc_rich_z.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      cc_nphe_tot[i]   = NAN;
      cc_ltcc_sec[i]   = -1;
      cc_ltcc_nphe[i]  = NAN;
      cc_ltcc_time[i]  = NAN;
      cc_ltcc_path[i]  = NAN;
      cc_ltcc_theta[i] = NAN;
      cc_ltcc_phi[i]   = NAN;
      cc_ltcc_x[i]     = NAN;
      cc_ltcc_y[i]     = NAN;
      cc_ltcc_z[i]     = NAN;
      cc_htcc_sec[i]   = -1;
      cc_htcc_nphe[i]  = NAN;
      cc_htcc_time[i]  = NAN;
      cc_htcc_path[i]  = NAN;
      cc_htcc_theta[i] = NAN;
      cc_htcc_phi[i]   = NAN;
      cc_htcc_x[i]     = NAN;
      cc_htcc_y[i]     = NAN;
      cc_htcc_z[i]     = NAN;
      cc_rich_sec[i]   = -1;
      cc_rich_nphe[i]  = NAN;
      cc_rich_time[i]  = NAN;
      cc_rich_path[i]  = NAN;
      cc_rich_theta[i] = NAN;
      cc_rich_phi[i]   = NAN;
      cc_rich_x[i]     = NAN;
      cc_rich_y[i]     = NAN;
      cc_rich_z[i]     = NAN;
    }

    float nphe_tot = 0.0;

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Cherenkov->getInt(1, k);
        int detector = rec_Cherenkov->getInt(2, k);

        if (pindex == i && (detector == HTCC || detector == LTCC || detector == RICH))
          nphe_tot += rec_Cherenkov->getFloat(4, k);

        if (pindex == i && detector == HTCC) {
          cc_htcc_sec[i]   = rec_Cherenkov->getInt(3, k);
          cc_htcc_nphe[i]  = rec_Cherenkov->getFloat(4, k);
          cc_htcc_time[i]  = rec_Cherenkov->getFloat(5, k);
          cc_htcc_path[i]  = rec_Cherenkov->getFloat(6, k);
          cc_htcc_theta[i] = rec_Cherenkov->getFloat(11, k);
          cc_htcc_phi[i]   = rec_Cherenkov->getFloat(12, k);
          cc_htcc_x[i]     = rec_Cherenkov->getFloat(7, k);
          cc_htcc_y[i]     = rec_Cherenkov->getFloat(8, k);
          cc_htcc_z[i]     = rec_Cherenkov->getFloat(9, k);
        } else if (pindex == i && detector == LTCC) {
          cc_ltcc_sec[i]   = rec_Cherenkov->getInt(3, k);
          cc_ltcc_nphe[i]  = rec_Cherenkov->getFloat(4, k);
          cc_ltcc_time[i]  = rec_Cherenkov->getFloat(5, k);
          cc_ltcc_path[i]  = rec_Cherenkov->getFloat(6, k);
          cc_ltcc_theta[i] = rec_Cherenkov->getFloat(11, k);
          cc_ltcc_phi[i]   = rec_Cherenkov->getFloat(12, k);
          cc_ltcc_x[i]     = rec_Cherenkov->getFloat(7, k);
          cc_ltcc_y[i]     = rec_Cherenkov->getFloat(8, k);
          cc_ltcc_z[i]     = rec_Cherenkov->getFloat(9, k);
        } else if (pindex == i && detector == RICH) {
          cc_rich_sec[i]   = rec_Cherenkov->getInt(3, k);
          cc_rich_nphe[i]  = rec_Cherenkov->getFloat(4, k);
          cc_rich_time[i]  = rec_Cherenkov->getFloat(5, k);
          cc_rich_path[i]  = rec_Cherenkov->getFloat(6, k);
          cc_rich_theta[i] = rec_Cherenkov->getFloat(11, k);
          cc_rich_phi[i]   = rec_Cherenkov->getFloat(12, k);
          cc_rich_x[i]     = rec_Cherenkov->getFloat(7, k);
          cc_rich_y[i]     = rec_Cherenkov->getFloat(8, k);
          cc_rich_z[i]     = rec_Cherenkov->getFloat(9, k);
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
        int pindex   = rec_Scintillator->getInt(1, k);
        int detector = rec_Scintillator->getInt(2, k);
        int layer    = rec_Scintillator->getInt(4, k);
        if (pindex == i && detector == FTOF && layer == FTOF_1A) {
          sc_ftof_1a_sec[i]       = rec_Scintillator->getInt(3, k);
          sc_ftof_1a_component[i] = rec_Scintillator->getInt(5, k);
          sc_ftof_1a_energy[i]    = rec_Scintillator->getFloat(6, k);
          sc_ftof_1a_time[i]      = rec_Scintillator->getFloat(7, k);
          sc_ftof_1a_path[i]      = rec_Scintillator->getFloat(8, k);
          sc_ftof_1a_x[i]         = rec_Scintillator->getFloat(10, k);
          sc_ftof_1a_y[i]         = rec_Scintillator->getFloat(11, k);
          sc_ftof_1a_z[i]         = rec_Scintillator->getFloat(12, k);
          sc_ftof_1a_hx[i]        = rec_Scintillator->getFloat(13, k);
          sc_ftof_1a_hy[i]        = rec_Scintillator->getFloat(14, k);
          sc_ftof_1a_hz[i]        = rec_Scintillator->getFloat(15, k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_1B) {
          sc_ftof_1b_sec[i]       = rec_Scintillator->getInt(3, k);
          sc_ftof_1b_component[i] = rec_Scintillator->getInt(5, k);
          sc_ftof_1b_energy[i]    = rec_Scintillator->getFloat(6, k);
          sc_ftof_1b_time[i]      = rec_Scintillator->getFloat(7, k);
          sc_ftof_1b_path[i]      = rec_Scintillator->getFloat(8, k);
          sc_ftof_1b_x[i]         = rec_Scintillator->getFloat(10, k);
          sc_ftof_1b_y[i]         = rec_Scintillator->getFloat(11, k);
          sc_ftof_1b_z[i]         = rec_Scintillator->getFloat(12, k);
          sc_ftof_1b_hx[i]        = rec_Scintillator->getFloat(13, k);
          sc_ftof_1b_hy[i]        = rec_Scintillator->getFloat(14, k);
          sc_ftof_1b_hz[i]        = rec_Scintillator->getFloat(15, k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_2) {
          sc_ftof_2_sec[i]       = rec_Scintillator->getInt(3, k);
          sc_ftof_2_component[i] = rec_Scintillator->getInt(5, k);
          sc_ftof_2_energy[i]    = rec_Scintillator->getFloat(6, k);
          sc_ftof_2_time[i]      = rec_Scintillator->getFloat(7, k);
          sc_ftof_2_path[i]      = rec_Scintillator->getFloat(8, k);
          sc_ftof_2_x[i]         = rec_Scintillator->getFloat(10, k);
          sc_ftof_2_y[i]         = rec_Scintillator->getFloat(11, k);
          sc_ftof_2_z[i]         = rec_Scintillator->getFloat(12, k);
          sc_ftof_2_hx[i]        = rec_Scintillator->getFloat(13, k);
          sc_ftof_2_hy[i]        = rec_Scintillator->getFloat(14, k);
          sc_ftof_2_hz[i]        = rec_Scintillator->getFloat(15, k);
        } else if (pindex == i && detector == CTOF) {
          sc_ctof_component[i] = rec_Scintillator->getInt(5, k);
          sc_ctof_energy[i]    = rec_Scintillator->getFloat(6, k);
          sc_ctof_time[i]      = rec_Scintillator->getFloat(7, k);
          sc_ctof_path[i]      = rec_Scintillator->getFloat(8, k);
          sc_ctof_x[i]         = rec_Scintillator->getFloat(10, k);
          sc_ctof_y[i]         = rec_Scintillator->getFloat(11, k);
          sc_ctof_z[i]         = rec_Scintillator->getFloat(12, k);
          sc_ctof_hx[i]        = rec_Scintillator->getFloat(13, k);
          sc_ctof_hy[i]        = rec_Scintillator->getFloat(14, k);
          sc_ctof_hz[i]        = rec_Scintillator->getFloat(15, k);
        } else if (pindex == i && detector == CND) {
          sc_cnd_component[i] = rec_Scintillator->getInt(5, k);
          sc_cnd_energy[i]    = rec_Scintillator->getFloat(6, k);
          sc_cnd_time[i]      = rec_Scintillator->getFloat(7, k);
          sc_cnd_path[i]      = rec_Scintillator->getFloat(8, k);
          sc_cnd_x[i]         = rec_Scintillator->getFloat(10, k);
          sc_cnd_y[i]         = rec_Scintillator->getFloat(11, k);
          sc_cnd_z[i]         = rec_Scintillator->getFloat(12, k);
          sc_cnd_hx[i]        = rec_Scintillator->getFloat(13, k);
          sc_cnd_hy[i]        = rec_Scintillator->getFloat(14, k);
          sc_cnd_hz[i]        = rec_Scintillator->getFloat(15, k);
        }
      }
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Track->getRows();

    dc_sec.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      dc_sec[i] = -1;
    }

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Track->getInt(1, k);
        int detector = rec_Track->getInt(2, k);

        if (pindex == i && detector == DC) {
          dc_sec[i] = rec_Track->getInt(3, k);
        }
      }
    }

    len_pid    = rec_Particle->getRows();
    len_pindex = rec_Traj->getRows();

    dc_r1_x.resize(len_pid);
    dc_r1_y.resize(len_pid);
    dc_r1_z.resize(len_pid);
    dc_r2_x.resize(len_pid);
    dc_r2_y.resize(len_pid);
    dc_r2_z.resize(len_pid);
    dc_r3_x.resize(len_pid);
    dc_r3_y.resize(len_pid);
    dc_r3_z.resize(len_pid);

    cvt_x.resize(len_pid);
    cvt_y.resize(len_pid);
    cvt_z.resize(len_pid);

    fmt_x.resize(len_pid);
    fmt_y.resize(len_pid);
    fmt_z.resize(len_pid);

    for (int i = 0; i < len_pid; i++) {
      dc_r1_x[i] = NAN;
      dc_r1_y[i] = NAN;
      dc_r1_z[i] = NAN;
      dc_r2_x[i] = NAN;
      dc_r2_y[i] = NAN;
      dc_r2_z[i] = NAN;
      dc_r3_x[i] = NAN;
      dc_r3_y[i] = NAN;
      dc_r3_z[i] = NAN;

      cvt_x[i] = NAN;
      cvt_y[i] = NAN;
      cvt_z[i] = NAN;

      fmt_x[i] = NAN;
      fmt_y[i] = NAN;
      fmt_z[i] = NAN;
    }

    for (int i = 0; i < len_pid; i++) {
      for (int k = 0; k < len_pindex; k++) {
        int pindex   = rec_Traj->getShort("pindex", k);
        int detector = rec_Traj->getByte("detector", k);
        int layer    = rec_Traj->getByte("layer", k);

        if (pindex == i) {
          if (detector == FMT && layer == 1) {
            fmt_x[i] = rec_Traj->getFloat("x", k);
            fmt_y[i] = rec_Traj->getFloat("y", k);
            fmt_z[i] = rec_Traj->getFloat("z", k);
          }

          if (detector == CVT && layer == 1) {
            cvt_x[i] = rec_Traj->getFloat("x", k);
            cvt_y[i] = rec_Traj->getFloat("y", k);
            cvt_z[i] = rec_Traj->getFloat("z", k);
          }

          if (detector == DC) {
            // Layers 6 12 18 24 30 36 are saved
            // Choose every other one starting at 6, 18, 30
            // Assuiming they are in r1,r2,r3
            if (layer == 6) {
              dc_r1_x[i] = rec_Traj->getFloat("x", k);
              dc_r1_y[i] = rec_Traj->getFloat("y", k);
              dc_r1_z[i] = rec_Traj->getFloat("z", k);
            } else if (layer == 18) {
              dc_r2_x[i] = rec_Traj->getFloat("x", k);
              dc_r2_y[i] = rec_Traj->getFloat("y", k);
              dc_r2_z[i] = rec_Traj->getFloat("z", k);
            } else if (layer == 30) {
              dc_r3_x[i] = rec_Traj->getFloat("x", k);
              dc_r3_y[i] = rec_Traj->getFloat("y", k);
              dc_r3_z[i] = rec_Traj->getFloat("z", k);
            }
          }
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
        int pindex   = rec_ForwardTagger->getInt(1, k);
        int detector = rec_ForwardTagger->getInt(2, k);

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
          int pindex = rec_CovMat->getInt(1, k);
          if (pindex == i) {
            CovMat_11[i] = rec_CovMat->getFloat(2, k);
            CovMat_12[i] = rec_CovMat->getFloat(3, k);
            CovMat_13[i] = rec_CovMat->getFloat(4, k);
            CovMat_14[i] = rec_CovMat->getFloat(5, k);
            CovMat_15[i] = rec_CovMat->getFloat(6, k);
            CovMat_22[i] = rec_CovMat->getFloat(7, k);
            CovMat_23[i] = rec_CovMat->getFloat(8, k);
            CovMat_24[i] = rec_CovMat->getFloat(9, k);
            CovMat_25[i] = rec_CovMat->getFloat(10, k);
            CovMat_33[i] = rec_CovMat->getFloat(11, k);
            CovMat_34[i] = rec_CovMat->getFloat(12, k);
            CovMat_35[i] = rec_CovMat->getFloat(13, k);
            CovMat_44[i] = rec_CovMat->getFloat(14, k);
            CovMat_45[i] = rec_CovMat->getFloat(15, k);
            CovMat_55[i] = rec_CovMat->getFloat(16, k);
          }
        }
      }
    }

    if (traj) {
      len_pid = rec_Traj->getRows();
      traj_pindex_vec.resize(len_pid);
      traj_index_vec.resize(len_pid);
      traj_detId_vec.resize(len_pid);
      traj_q_vec.resize(len_pid);
      traj_x_vec.resize(len_pid);
      traj_y_vec.resize(len_pid);
      traj_z_vec.resize(len_pid);
      traj_cx_vec.resize(len_pid);
      traj_cy_vec.resize(len_pid);
      traj_cz_vec.resize(len_pid);
      traj_pathlength_vec.resize(len_pid);

      for (int i = 0; i < len_pid; i++) {
        traj_pindex_vec[i]     = rec_Traj->getInt(1, i);
        traj_index_vec[i]      = rec_Traj->getInt(2, i);
        traj_detId_vec[i]      = rec_Traj->getInt(3, i);
        traj_q_vec[i]          = rec_Traj->getFloat(4, i);
        traj_x_vec[i]          = rec_Traj->getFloat(5, i);
        traj_y_vec[i]          = rec_Traj->getFloat(6, i);
        traj_z_vec[i]          = rec_Traj->getFloat(7, i);
        traj_cx_vec[i]         = rec_Traj->getFloat(8, i);
        traj_cy_vec[i]         = rec_Traj->getFloat(9, i);
        traj_cz_vec[i]         = rec_Traj->getFloat(10, i);
        traj_pathlength_vec[i] = rec_Traj->getFloat(11, i);
      }
    }
    clas12->Fill();
  }

  OutputFile = clas12->GetCurrentFile();
  OutputFile->cd();
  clas12->Write();
  OutputFile->Close();

  std::chrono::duration<double> elapsed_full =
      (std::chrono::high_resolution_clock::now() - start_full);
  std::cout << "Elapsed time: " << elapsed_full.count() << " s" << std::endl;
  if (!is_batch) {
    std::cout << "Events/Sec: " << tot_hipo_events / elapsed_full.count() << " Hz" << std::endl;
    std::cout << "Events converted: " << tot_events_processed << "\t ("
              << 100.0 * tot_events_processed / tot_hipo_events << "%)" << std::endl;
  }
  std::cout << "Total events in file: " << tot_hipo_events << std::endl;
  std::cout << "Events converted/Sec: " << tot_events_processed / elapsed_full.count() << " Hz"
            << std::endl;

  return 0;
}
