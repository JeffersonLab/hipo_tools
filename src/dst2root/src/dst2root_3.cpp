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
#include "TFileCacheWrite.h"
#include "TTree.h"
// Hipo libs
#include "hipo3/reader.h"

// Headers in same folder
#include "banks.h"
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
  bool        VertDoca    = false;
  bool        traj        = false;

  auto cli = (clipp::option("-h", "--help").set(print_help) % "print help",
              clipp::option("-mc", "--MC").set(is_mc) % "Convert dst and mc banks",
              clipp::option("-b", "--batch").set(is_batch) % "Don't show progress and statistics",
              clipp::option("-r", "--rec").set(good_rec) %
                  "Only save events where number of partilces in the event > 0",
              clipp::option("-e", "--elec").set(elec_first) %
                  "Only save events with good electron as first particle",
              clipp::option("-c", "--cov").set(cov) % "Save Covariant Matrix for kinematic fitting",
              clipp::option("-v", "--VertDoca").set(VertDoca) % "Save VertDoca information",
              clipp::option("-t", "--traj").set(traj) % "Save traj information",
              clipp::value("inputFile.hipo", InFileName),
              clipp::opt_value("outputFile.root", OutFileName));

  clipp::parse(argc, argv, cli);
  if (print_help || InFileName == "") {
    std::cout << clipp::make_man_page(cli, argv[0]);
    exit(0);
  }

  if (OutFileName == "")
    OutFileName = InFileName + ".root";

  TFile*           OutputFile = new TFile(OutFileName.c_str(), "RECREATE");
  TFileCacheWrite* fileCache  = new TFileCacheWrite(OutputFile, 10000000);
  OutputFile->SetCompressionSettings(404); // kUseAnalysis
  TTree* clas12 = new TTree("clas12", "clas12");

  hipo::reader* reader          = new hipo::reader(InFileName.c_str());
  long          tot_hipo_events = reader->numEvents();

  hipo::node<int32_t>* NRUN_node    = reader->getBranch<int32_t>(330, 1);
  hipo::node<int32_t>* NEVENT_node  = reader->getBranch<int32_t>(330, 2);
  hipo::node<float>*   EVNTime_node = reader->getBranch<float>(330, 3);
  hipo::node<int8_t>*  TYPE_node    = reader->getBranch<int8_t>(330, 4);
  hipo::node<int64_t>* TRG_node     = reader->getBranch<int64_t>(330, 7);
  hipo::node<float>*   BCG_node     = reader->getBranch<float>(330, 8);
  hipo::node<float>*   STTime_node  = reader->getBranch<float>(330, 10);
  hipo::node<float>*   RFTime_node  = reader->getBranch<float>(330, 11);
  hipo::node<int8_t>*  Helic_node   = reader->getBranch<int8_t>(330, 12);

  hipo::node<int32_t>* pid_node     = reader->getBranch<int32_t>(331, 1);
  hipo::node<float>*   px_node      = reader->getBranch<float>(331, 2);
  hipo::node<float>*   py_node      = reader->getBranch<float>(331, 3);
  hipo::node<float>*   pz_node      = reader->getBranch<float>(331, 4);
  hipo::node<float>*   vx_node      = reader->getBranch<float>(331, 5);
  hipo::node<float>*   vy_node      = reader->getBranch<float>(331, 6);
  hipo::node<float>*   vz_node      = reader->getBranch<float>(331, 7);
  hipo::node<int8_t>*  charge_node  = reader->getBranch<int8_t>(331, 8);
  hipo::node<float>*   beta_node    = reader->getBranch<float>(331, 9);
  hipo::node<float>*   chi2pid_node = reader->getBranch<float>(331, 10);
  hipo::node<int16_t>* status_node  = reader->getBranch<int16_t>(331, 11);

  hipo::node<int16_t>* cal_pindex_node   = reader->getBranch<int16_t>(332, 2);
  hipo::node<int8_t>*  cal_detector_node = reader->getBranch<int8_t>(332, 3);
  hipo::node<int8_t>*  cal_sector_node   = reader->getBranch<int8_t>(332, 4);
  hipo::node<int8_t>*  cal_layer_node    = reader->getBranch<int8_t>(332, 5);
  hipo::node<float>*   cal_energy_node   = reader->getBranch<float>(332, 6);
  hipo::node<float>*   cal_time_node     = reader->getBranch<float>(332, 7);
  hipo::node<float>*   cal_path_node     = reader->getBranch<float>(332, 8);
  hipo::node<float>*   cal_x_node        = reader->getBranch<float>(332, 10);
  hipo::node<float>*   cal_y_node        = reader->getBranch<float>(332, 11);
  hipo::node<float>*   cal_z_node        = reader->getBranch<float>(332, 12);
  hipo::node<float>*   cal_hx_node       = reader->getBranch<float>(332, 13);
  hipo::node<float>*   cal_hy_node       = reader->getBranch<float>(332, 14);
  hipo::node<float>*   cal_hz_node       = reader->getBranch<float>(332, 15);
  hipo::node<float>*   cal_lu_node       = reader->getBranch<float>(332, 16);
  hipo::node<float>*   cal_lv_node       = reader->getBranch<float>(332, 17);
  hipo::node<float>*   cal_lw_node       = reader->getBranch<float>(332, 18);
  hipo::node<float>*   cal_du_node       = reader->getBranch<float>(332, 19);
  hipo::node<float>*   cal_dv_node       = reader->getBranch<float>(332, 20);
  hipo::node<float>*   cal_dw_node       = reader->getBranch<float>(332, 21);
  hipo::node<float>*   cal_m2u_node      = reader->getBranch<float>(332, 22);
  hipo::node<float>*   cal_m2v_node      = reader->getBranch<float>(332, 23);
  hipo::node<float>*   cal_m2w_node      = reader->getBranch<float>(332, 24);
  hipo::node<float>*   cal_m3u_node      = reader->getBranch<float>(332, 25);
  hipo::node<float>*   cal_m3v_node      = reader->getBranch<float>(332, 26);
  hipo::node<float>*   cal_m3w_node      = reader->getBranch<float>(332, 27);

  hipo::node<int16_t>* chern_pindex_node   = reader->getBranch<int16_t>(333, 2);
  hipo::node<int8_t>*  chern_detector_node = reader->getBranch<int8_t>(333, 3);
  hipo::node<int8_t>*  chern_sector_node   = reader->getBranch<int8_t>(333, 4);
  hipo::node<float>*   chern_nphe_node     = reader->getBranch<float>(333, 5);
  hipo::node<float>*   chern_time_node     = reader->getBranch<float>(333, 6);
  hipo::node<float>*   chern_path_node     = reader->getBranch<float>(333, 7);
  hipo::node<float>*   chern_chi2_node     = reader->getBranch<float>(333, 8);
  hipo::node<float>*   chern_x_node        = reader->getBranch<float>(333, 9);
  hipo::node<float>*   chern_y_node        = reader->getBranch<float>(333, 10);
  hipo::node<float>*   chern_z_node        = reader->getBranch<float>(333, 11);
  hipo::node<float>*   chern_theta_node    = reader->getBranch<float>(333, 12);
  hipo::node<float>*   chern_phi_node      = reader->getBranch<float>(333, 13);
  hipo::node<float>*   chern_dtheta_node   = reader->getBranch<float>(333, 14);
  hipo::node<float>*   chern_dphi_node     = reader->getBranch<float>(333, 15);

  hipo::node<int16_t>* fortag_pindex_node   = reader->getBranch<int16_t>(334, 2);
  hipo::node<int8_t>*  fortag_detector_node = reader->getBranch<int8_t>(334, 3);
  hipo::node<float>*   fortag_energy_node   = reader->getBranch<float>(334, 4);
  hipo::node<float>*   fortag_time_node     = reader->getBranch<float>(334, 5);
  hipo::node<float>*   fortag_path_node     = reader->getBranch<float>(334, 6);
  hipo::node<float>*   fortag_x_node        = reader->getBranch<float>(334, 8);
  hipo::node<float>*   fortag_y_node        = reader->getBranch<float>(334, 9);
  hipo::node<float>*   fortag_z_node        = reader->getBranch<float>(334, 10);
  hipo::node<float>*   fortag_dx_node       = reader->getBranch<float>(334, 11);
  hipo::node<float>*   fortag_dy_node       = reader->getBranch<float>(334, 12);
  hipo::node<float>*   fortag_radius_node   = reader->getBranch<float>(334, 13);
  hipo::node<int16_t>* fortag_size_node     = reader->getBranch<int16_t>(334, 14);

  hipo::node<int16_t>* scint_pindex_node    = reader->getBranch<int16_t>(335, 2);
  hipo::node<int8_t>*  scint_detector_node  = reader->getBranch<int8_t>(335, 3);
  hipo::node<int8_t>*  scint_sector_node    = reader->getBranch<int8_t>(335, 4);
  hipo::node<int8_t>*  scint_layer_node     = reader->getBranch<int8_t>(335, 5);
  hipo::node<int16_t>* scint_component_node = reader->getBranch<int16_t>(335, 6);
  hipo::node<float>*   scint_energy_node    = reader->getBranch<float>(335, 7);
  hipo::node<float>*   scint_time_node      = reader->getBranch<float>(335, 8);
  hipo::node<float>*   scint_path_node      = reader->getBranch<float>(335, 9);
  hipo::node<float>*   scint_x_node         = reader->getBranch<float>(335, 11);
  hipo::node<float>*   scint_y_node         = reader->getBranch<float>(335, 12);
  hipo::node<float>*   scint_z_node         = reader->getBranch<float>(335, 13);
  hipo::node<float>*   scint_hx_node        = reader->getBranch<float>(335, 14);
  hipo::node<float>*   scint_hy_node        = reader->getBranch<float>(335, 15);
  hipo::node<float>*   scint_hz_node        = reader->getBranch<float>(335, 16);

  hipo::node<int16_t>* track_pindex_node    = reader->getBranch<int16_t>(336, 2);
  hipo::node<int8_t>*  track_detector_node  = reader->getBranch<int8_t>(336, 3);
  hipo::node<int8_t>*  track_sector_node    = reader->getBranch<int8_t>(336, 4);
  hipo::node<int8_t>*  track_q_node         = reader->getBranch<int8_t>(336, 6);
  hipo::node<float>*   track_chi2_node      = reader->getBranch<float>(336, 7);
  hipo::node<int16_t>* track_NDF_node       = reader->getBranch<int16_t>(336, 8);
  hipo::node<float>*   track_px_nomm_node   = reader->getBranch<float>(336, 9);
  hipo::node<float>*   track_py_nomm_node   = reader->getBranch<float>(336, 10);
  hipo::node<float>*   track_pz_nomm_node   = reader->getBranch<float>(336, 11);
  hipo::node<float>*   track_vx_nomm_node   = reader->getBranch<float>(336, 12);
  hipo::node<float>*   track_vy_nomm_node   = reader->getBranch<float>(336, 13);
  hipo::node<float>*   track_vz_nomm_node   = reader->getBranch<float>(336, 14);
  hipo::node<float>*   track_chi2_nomm_node = reader->getBranch<float>(336, 15);
  hipo::node<float>*   track_NDF_nomm_node  = reader->getBranch<float>(336, 16);

  hipo::node<int16_t>* VertDoca_index1_node = reader->getBranch<int16_t>(339, 1);
  hipo::node<int16_t>* VertDoca_index2_node = reader->getBranch<int16_t>(339, 2);
  hipo::node<float>*   VertDoca_x_node      = reader->getBranch<float>(339, 3);
  hipo::node<float>*   VertDoca_y_node      = reader->getBranch<float>(339, 4);
  hipo::node<float>*   VertDoca_z_node      = reader->getBranch<float>(339, 5);
  hipo::node<float>*   VertDoca_x1_node     = reader->getBranch<float>(339, 6);
  hipo::node<float>*   VertDoca_y1_node     = reader->getBranch<float>(339, 7);
  hipo::node<float>*   VertDoca_z1_node     = reader->getBranch<float>(339, 8);
  hipo::node<float>*   VertDoca_cx1_node    = reader->getBranch<float>(339, 9);
  hipo::node<float>*   VertDoca_cy1_node    = reader->getBranch<float>(339, 10);
  hipo::node<float>*   VertDoca_cz1_node    = reader->getBranch<float>(339, 11);
  hipo::node<float>*   VertDoca_x2_node     = reader->getBranch<float>(339, 12);
  hipo::node<float>*   VertDoca_y2_node     = reader->getBranch<float>(339, 13);
  hipo::node<float>*   VertDoca_z2_node     = reader->getBranch<float>(339, 14);
  hipo::node<float>*   VertDoca_cx2_node    = reader->getBranch<float>(339, 15);
  hipo::node<float>*   VertDoca_cy2_node    = reader->getBranch<float>(339, 16);
  hipo::node<float>*   VertDoca_cz2_node    = reader->getBranch<float>(339, 17);
  hipo::node<float>*   VertDoca_r_node      = reader->getBranch<float>(339, 18);

  hipo::node<int16_t>* traj_pindex_node     = reader->getBranch<int16_t>(340, 1);
  hipo::node<int16_t>* traj_index_node      = reader->getBranch<int16_t>(340, 2);
  hipo::node<int16_t>* traj_detId_node      = reader->getBranch<int16_t>(340, 3);
  hipo::node<int8_t>*  traj_q_node          = reader->getBranch<int8_t>(340, 4);
  hipo::node<float>*   traj_x_node          = reader->getBranch<float>(340, 5);
  hipo::node<float>*   traj_y_node          = reader->getBranch<float>(340, 6);
  hipo::node<float>*   traj_z_node          = reader->getBranch<float>(340, 7);
  hipo::node<float>*   traj_cx_node         = reader->getBranch<float>(340, 8);
  hipo::node<float>*   traj_cy_node         = reader->getBranch<float>(340, 9);
  hipo::node<float>*   traj_cz_node         = reader->getBranch<float>(340, 10);
  hipo::node<float>*   traj_pathlength_node = reader->getBranch<float>(340, 11);

  hipo::node<float>*   MC_Header_helicity_node = reader->getBranch<float>(40, 4);
  hipo::node<int16_t>* MC_Event_npart_node     = reader->getBranch<int16_t>(41, 1);
  hipo::node<int32_t>* MC_pid_node             = reader->getBranch<int32_t>(42, 1);
  hipo::node<float>*   MC_px_node              = reader->getBranch<float>(42, 2);
  hipo::node<float>*   MC_py_node              = reader->getBranch<float>(42, 3);
  hipo::node<float>*   MC_pz_node              = reader->getBranch<float>(42, 4);
  hipo::node<float>*   MC_vx_node              = reader->getBranch<float>(42, 5);
  hipo::node<float>*   MC_vy_node              = reader->getBranch<float>(42, 6);
  hipo::node<float>*   MC_vz_node              = reader->getBranch<float>(42, 7);
  hipo::node<float>*   MC_vt_node              = reader->getBranch<float>(42, 8);

  hipo::node<int32_t>* MC_Lund_pid_node   = reader->getBranch<int32_t>(43, 3);
  hipo::node<float>*   MC_Lund_px_node    = reader->getBranch<float>(43, 6);
  hipo::node<float>*   MC_Lund_py_node    = reader->getBranch<float>(43, 7);
  hipo::node<float>*   MC_Lund_pz_node    = reader->getBranch<float>(43, 8);
  hipo::node<float>*   MC_Lund_E_node     = reader->getBranch<float>(43, 9);
  hipo::node<float>*   MC_Lund_vx_node    = reader->getBranch<float>(43, 11);
  hipo::node<float>*   MC_Lund_vy_node    = reader->getBranch<float>(43, 12);
  hipo::node<float>*   MC_Lund_vz_node    = reader->getBranch<float>(43, 13);
  hipo::node<float>*   MC_Lund_ltime_node = reader->getBranch<float>(43, 14);

  hipo::node<int16_t>* CovMat_pindex_node = reader->getBranch<int16_t>(338, 2);
  hipo::node<float>*   CovMat_C11_node    = reader->getBranch<float>(338, 3);
  hipo::node<float>*   CovMat_C12_node    = reader->getBranch<float>(338, 4);
  hipo::node<float>*   CovMat_C13_node    = reader->getBranch<float>(338, 5);
  hipo::node<float>*   CovMat_C14_node    = reader->getBranch<float>(338, 6);
  hipo::node<float>*   CovMat_C15_node    = reader->getBranch<float>(338, 7);
  hipo::node<float>*   CovMat_C22_node    = reader->getBranch<float>(338, 8);
  hipo::node<float>*   CovMat_C23_node    = reader->getBranch<float>(338, 9);
  hipo::node<float>*   CovMat_C24_node    = reader->getBranch<float>(338, 10);
  hipo::node<float>*   CovMat_C25_node    = reader->getBranch<float>(338, 11);
  hipo::node<float>*   CovMat_C33_node    = reader->getBranch<float>(338, 12);
  hipo::node<float>*   CovMat_C34_node    = reader->getBranch<float>(338, 13);
  hipo::node<float>*   CovMat_C35_node    = reader->getBranch<float>(338, 14);
  hipo::node<float>*   CovMat_C44_node    = reader->getBranch<float>(338, 15);
  hipo::node<float>*   CovMat_C45_node    = reader->getBranch<float>(338, 16);
  hipo::node<float>*   CovMat_C55_node    = reader->getBranch<float>(338, 17);

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
  if (VertDoca) {
    clas12->Branch("VertDoca_index1", &VertDoca_index1_vec);
    clas12->Branch("VertDoca_index2", &VertDoca_index2_vec);
    clas12->Branch("VertDoca_x", &VertDoca_x_vec);
    clas12->Branch("VertDoca_y", &VertDoca_y_vec);
    clas12->Branch("VertDoca_z", &VertDoca_z_vec);
    clas12->Branch("VertDoca_x1", &VertDoca_x1_vec);
    clas12->Branch("VertDoca_y1", &VertDoca_y1_vec);
    clas12->Branch("VertDoca_z1", &VertDoca_z1_vec);
    clas12->Branch("VertDoca_cx1", &VertDoca_cx1_vec);
    clas12->Branch("VertDoca_cy1", &VertDoca_cy1_vec);
    clas12->Branch("VertDoca_cz1", &VertDoca_cz1_vec);
    clas12->Branch("VertDoca_x2", &VertDoca_x2_vec);
    clas12->Branch("VertDoca_y2", &VertDoca_y2_vec);
    clas12->Branch("VertDoca_z2", &VertDoca_z2_vec);
    clas12->Branch("VertDoca_cx2", &VertDoca_cx2_vec);
    clas12->Branch("VertDoca_cy2", &VertDoca_cy2_vec);
    clas12->Branch("VertDoca_cz2", &VertDoca_cz2_vec);
    clas12->Branch("VertDoca_r", &VertDoca_r_vec);
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

  int  entry                = 0;
  int  l                    = 0;
  int  len_pid              = 0;
  int  len_pindex           = 0;
  int  tot_events_processed = 0;
  auto start_full           = std::chrono::high_resolution_clock::now();
  while (reader->next()) {
    if (!is_batch && (++entry % 1000) == 0)
      std::cout << "\t" << floor(100 * entry / tot_hipo_events) << "%\r\r" << std::flush;

    if (good_rec && pid_node->getLength() == 0)
      continue;
    if (elec_first && pid_node->getValue(0) != 11)
      continue;

    tot_events_processed++;
    if (NRUN_node->getLength() > 0) {
      NRUN    = NRUN_node->getValue(0);
      NEVENT  = NEVENT_node->getValue(0);
      EVNTime = EVNTime_node->getValue(0);
      TYPE    = TYPE_node->getValue(0);
      TRG     = TRG_node->getValue(0);
      BCG     = BCG_node->getValue(0);
      STTime  = STTime_node->getValue(0);
      RFTime  = RFTime_node->getValue(0);
      Helic   = Helic_node->getValue(0);
    }

    l = pid_node->getLength();
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
      pid[i]     = pid_node->getValue(i);
      p2[i]      = (px_node->getValue(i) * px_node->getValue(i) +
               py_node->getValue(i) * py_node->getValue(i) +
               pz_node->getValue(i) * pz_node->getValue(i));
      p[i]       = sqrt(p2[i]);
      px[i]      = px_node->getValue(i);
      py[i]      = py_node->getValue(i);
      pz[i]      = pz_node->getValue(i);
      vx[i]      = vx_node->getValue(i);
      vy[i]      = vy_node->getValue(i);
      vz[i]      = vz_node->getValue(i);
      charge[i]  = charge_node->getValue(i);
      beta[i]    = ((beta_node->getValue(i) != -9999) ? beta_node->getValue(i) : NAN);
      chi2pid[i] = chi2pid_node->getValue(i);
      status[i]  = status_node->getValue(i);
    }

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
        MC_pid[i]      = MC_pid_node->getValue(i);
        MC_px[i]       = MC_px_node->getValue(i);
        MC_py[i]       = MC_py_node->getValue(i);
        MC_pz[i]       = MC_pz_node->getValue(i);
        MC_vx[i]       = MC_vx_node->getValue(i);
        MC_vy[i]       = MC_vy_node->getValue(i);
        MC_vz[i]       = MC_vz_node->getValue(i);
        MC_vt[i]       = MC_vt_node->getValue(i);

        Lund_pid[i]   = MC_Lund_pid_node->getValue(i);
        Lund_px[i]    = MC_Lund_px_node->getValue(i);
        Lund_py[i]    = MC_Lund_py_node->getValue(i);
        Lund_pz[i]    = MC_Lund_pz_node->getValue(i);
        Lund_E[i]     = MC_Lund_E_node->getValue(i);
        Lund_vx[i]    = MC_Lund_vx_node->getValue(i);
        Lund_vy[i]    = MC_Lund_vy_node->getValue(i);
        Lund_vz[i]    = MC_Lund_vz_node->getValue(i);
        Lund_ltime[i] = MC_Lund_ltime_node->getValue(i);
      }
    }

    len_pid    = pid_node->getLength();
    len_pindex = cal_pindex_node->getLength();

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
        int   pindex   = cal_pindex_node->getValue(k);
        int   detector = cal_detector_node->getValue(k);
        int   layer    = cal_layer_node->getValue(k);
        float energy   = cal_energy_node->getValue(k);

        if (pindex == i && detector == ECAL) {
          etot += energy;
          if (layer == PCAL) {
            pcal += energy;
            ec_pcal_sec[i]  = cal_sector_node->getValue(k);
            ec_pcal_time[i] = cal_time_node->getValue(k);
            ec_pcal_path[i] = cal_path_node->getValue(k);
            ec_pcal_x[i]    = cal_x_node->getValue(k);
            ec_pcal_y[i]    = cal_y_node->getValue(k);
            ec_pcal_z[i]    = cal_z_node->getValue(k);
            ec_pcal_hx[i]   = cal_hx_node->getValue(k);
            ec_pcal_hy[i]   = cal_hy_node->getValue(k);
            ec_pcal_hz[i]   = cal_hz_node->getValue(k);
            ec_pcal_lu[i]   = cal_lu_node->getValue(k);
            ec_pcal_lv[i]   = cal_lv_node->getValue(k);
            ec_pcal_lw[i]   = cal_lw_node->getValue(k);
            ec_pcal_du[i]   = cal_du_node->getValue(k);
            ec_pcal_dv[i]   = cal_dv_node->getValue(k);
            ec_pcal_dw[i]   = cal_dw_node->getValue(k);
            ec_pcal_m2u[i]  = cal_m2u_node->getValue(k);
            ec_pcal_m2v[i]  = cal_m2v_node->getValue(k);
            ec_pcal_m2w[i]  = cal_m2w_node->getValue(k);
            ec_pcal_m3u[i]  = cal_m3u_node->getValue(k);
            ec_pcal_m3v[i]  = cal_m3v_node->getValue(k);
            ec_pcal_m3w[i]  = cal_m3w_node->getValue(k);
          } else if (layer == EC_INNER) {
            einner += energy;
            ec_ecin_sec[i]  = cal_sector_node->getValue(k);
            ec_ecin_time[i] = cal_time_node->getValue(k);
            ec_ecin_path[i] = cal_path_node->getValue(k);
            ec_ecin_x[i]    = cal_x_node->getValue(k);
            ec_ecin_y[i]    = cal_y_node->getValue(k);
            ec_ecin_z[i]    = cal_z_node->getValue(k);
            ec_ecin_hx[i]   = cal_hx_node->getValue(k);
            ec_ecin_hy[i]   = cal_hy_node->getValue(k);
            ec_ecin_hz[i]   = cal_hz_node->getValue(k);
            ec_ecin_lu[i]   = cal_lu_node->getValue(k);
            ec_ecin_lv[i]   = cal_lv_node->getValue(k);
            ec_ecin_lw[i]   = cal_lw_node->getValue(k);
            ec_ecin_du[i]   = cal_du_node->getValue(k);
            ec_ecin_dv[i]   = cal_dv_node->getValue(k);
            ec_ecin_dw[i]   = cal_dw_node->getValue(k);
            ec_ecin_m2u[i]  = cal_m2u_node->getValue(k);
            ec_ecin_m2v[i]  = cal_m2v_node->getValue(k);
            ec_ecin_m2w[i]  = cal_m2w_node->getValue(k);
            ec_ecin_m3u[i]  = cal_m3u_node->getValue(k);
            ec_ecin_m3v[i]  = cal_m3v_node->getValue(k);
            ec_ecin_m3w[i]  = cal_m3w_node->getValue(k);
          } else if (layer == EC_OUTER) {
            eouter += energy;
            ec_ecout_sec[i]  = cal_sector_node->getValue(k);
            ec_ecout_time[i] = cal_time_node->getValue(k);
            ec_ecout_path[i] = cal_path_node->getValue(k);
            ec_ecout_x[i]    = cal_x_node->getValue(k);
            ec_ecout_y[i]    = cal_y_node->getValue(k);
            ec_ecout_z[i]    = cal_z_node->getValue(k);
            ec_ecout_hx[i]   = cal_hx_node->getValue(k);
            ec_ecout_hy[i]   = cal_hy_node->getValue(k);
            ec_ecout_hz[i]   = cal_hz_node->getValue(k);
            ec_ecout_lu[i]   = cal_lu_node->getValue(k);
            ec_ecout_lv[i]   = cal_lv_node->getValue(k);
            ec_ecout_lw[i]   = cal_lw_node->getValue(k);
            ec_ecout_du[i]   = cal_du_node->getValue(k);
            ec_ecout_dv[i]   = cal_dv_node->getValue(k);
            ec_ecout_dw[i]   = cal_dw_node->getValue(k);
            ec_ecout_m2u[i]  = cal_m2u_node->getValue(k);
            ec_ecout_m2v[i]  = cal_m2v_node->getValue(k);
            ec_ecout_m2w[i]  = cal_m2w_node->getValue(k);
            ec_ecout_m3u[i]  = cal_m3u_node->getValue(k);
            ec_ecout_m3v[i]  = cal_m3v_node->getValue(k);
            ec_ecout_m3w[i]  = cal_m3w_node->getValue(k);
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

    len_pid    = pid_node->getLength();
    len_pindex = chern_pindex_node->getLength();

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
        int pindex   = chern_pindex_node->getValue(k);
        int detector = chern_detector_node->getValue(k);

        if (pindex == i && (detector == HTCC || detector == LTCC || detector == RICH))
          nphe_tot += chern_nphe_node->getValue(k);

        if (pindex == i && detector == HTCC) {
          cc_htcc_nphe[i]  = chern_nphe_node->getValue(k);
          cc_htcc_sec[i]   = chern_sector_node->getValue(k);
          cc_htcc_time[i]  = chern_time_node->getValue(k);
          cc_htcc_path[i]  = chern_path_node->getValue(k);
          cc_htcc_theta[i] = chern_theta_node->getValue(k);
          cc_htcc_phi[i]   = chern_phi_node->getValue(k);
          cc_htcc_x[i]     = chern_x_node->getValue(k);
          cc_htcc_y[i]     = chern_y_node->getValue(k);
          cc_htcc_z[i]     = chern_z_node->getValue(k);
        } else if (pindex == i && detector == LTCC) {
          cc_ltcc_nphe[i]  = chern_nphe_node->getValue(k);
          cc_ltcc_sec[i]   = chern_sector_node->getValue(k);
          cc_ltcc_time[i]  = chern_time_node->getValue(k);
          cc_ltcc_path[i]  = chern_path_node->getValue(k);
          cc_ltcc_theta[i] = chern_theta_node->getValue(k);
          cc_ltcc_phi[i]   = chern_phi_node->getValue(k);
          cc_ltcc_x[i]     = chern_x_node->getValue(k);
          cc_ltcc_y[i]     = chern_y_node->getValue(k);
          cc_ltcc_z[i]     = chern_z_node->getValue(k);
        } else if (pindex == i && detector == RICH) {
          cc_rich_nphe[i]  = chern_nphe_node->getValue(k);
          cc_rich_sec[i]   = chern_sector_node->getValue(k);
          cc_rich_time[i]  = chern_time_node->getValue(k);
          cc_rich_path[i]  = chern_path_node->getValue(k);
          cc_rich_theta[i] = chern_theta_node->getValue(k);
          cc_rich_phi[i]   = chern_phi_node->getValue(k);
          cc_rich_x[i]     = chern_x_node->getValue(k);
          cc_rich_y[i]     = chern_y_node->getValue(k);
          cc_rich_z[i]     = chern_z_node->getValue(k);
        }
      }
      if (cc_nphe_tot[i] != cc_nphe_tot[i])
        cc_nphe_tot[i] = ((nphe_tot != 0.0) ? nphe_tot : NAN);
    }

    len_pid    = pid_node->getLength();
    len_pindex = scint_pindex_node->getLength();

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
        int pindex   = scint_pindex_node->getValue(k);
        int detector = scint_detector_node->getValue(k);
        int layer    = scint_layer_node->getValue(k);

        if (pindex == i && detector == FTOF && layer == FTOF_1A) {
          sc_ftof_1a_sec[i]       = scint_sector_node->getValue(k);
          sc_ftof_1a_time[i]      = scint_time_node->getValue(k);
          sc_ftof_1a_path[i]      = scint_path_node->getValue(k);
          sc_ftof_1a_energy[i]    = scint_energy_node->getValue(k);
          sc_ftof_1a_component[i] = scint_component_node->getValue(k);
          sc_ftof_1a_x[i]         = scint_x_node->getValue(k);
          sc_ftof_1a_y[i]         = scint_y_node->getValue(k);
          sc_ftof_1a_z[i]         = scint_z_node->getValue(k);
          sc_ftof_1a_hx[i]        = scint_hx_node->getValue(k);
          sc_ftof_1a_hy[i]        = scint_hy_node->getValue(k);
          sc_ftof_1a_hz[i]        = scint_hz_node->getValue(k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_1B) {
          sc_ftof_1b_sec[i]       = scint_sector_node->getValue(k);
          sc_ftof_1b_time[i]      = scint_time_node->getValue(k);
          sc_ftof_1b_path[i]      = scint_path_node->getValue(k);
          sc_ftof_1b_energy[i]    = scint_energy_node->getValue(k);
          sc_ftof_1b_component[i] = scint_component_node->getValue(k);
          sc_ftof_1b_x[i]         = scint_x_node->getValue(k);
          sc_ftof_1b_y[i]         = scint_y_node->getValue(k);
          sc_ftof_1b_z[i]         = scint_z_node->getValue(k);
          sc_ftof_1b_hx[i]        = scint_hx_node->getValue(k);
          sc_ftof_1b_hy[i]        = scint_hy_node->getValue(k);
          sc_ftof_1b_hz[i]        = scint_hz_node->getValue(k);
        } else if (pindex == i && detector == FTOF && layer == FTOF_2) {
          sc_ftof_2_sec[i]       = scint_sector_node->getValue(k);
          sc_ftof_2_time[i]      = scint_time_node->getValue(k);
          sc_ftof_2_path[i]      = scint_path_node->getValue(k);
          sc_ftof_2_energy[i]    = scint_energy_node->getValue(k);
          sc_ftof_2_component[i] = scint_component_node->getValue(k);
          sc_ftof_2_x[i]         = scint_x_node->getValue(k);
          sc_ftof_2_y[i]         = scint_y_node->getValue(k);
          sc_ftof_2_z[i]         = scint_z_node->getValue(k);
          sc_ftof_2_hx[i]        = scint_hx_node->getValue(k);
          sc_ftof_2_hy[i]        = scint_hy_node->getValue(k);
          sc_ftof_2_hz[i]        = scint_hz_node->getValue(k);
        } else if (pindex == i && detector == CTOF) {
          sc_ctof_time[i]      = scint_time_node->getValue(k);
          sc_ctof_path[i]      = scint_path_node->getValue(k);
          sc_ctof_energy[i]    = scint_energy_node->getValue(k);
          sc_ctof_component[i] = scint_component_node->getValue(k);
          sc_ctof_x[i]         = scint_x_node->getValue(k);
          sc_ctof_y[i]         = scint_y_node->getValue(k);
          sc_ctof_z[i]         = scint_z_node->getValue(k);
          sc_ctof_hx[i]        = scint_hx_node->getValue(k);
          sc_ctof_hy[i]        = scint_hy_node->getValue(k);
          sc_ctof_hz[i]        = scint_hz_node->getValue(k);
        } else if (pindex == i && detector == CND) {
          sc_cnd_time[i]      = scint_time_node->getValue(k);
          sc_cnd_path[i]      = scint_path_node->getValue(k);
          sc_cnd_energy[i]    = scint_energy_node->getValue(k);
          sc_cnd_component[i] = scint_component_node->getValue(k);
          sc_cnd_x[i]         = scint_x_node->getValue(k);
          sc_cnd_y[i]         = scint_y_node->getValue(k);
          sc_cnd_z[i]         = scint_z_node->getValue(k);
          sc_cnd_hx[i]        = scint_hx_node->getValue(k);
          sc_cnd_hy[i]        = scint_hy_node->getValue(k);
          sc_cnd_hz[i]        = scint_hz_node->getValue(k);
        }
      }
    }

    len_pid    = pid_node->getLength();
    len_pindex = track_pindex_node->getLength();

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
        int pindex   = track_pindex_node->getValue(k);
        int detector = track_detector_node->getValue(k);

        if (pindex == i && detector == CVT) {
          cvt_px[i] = track_px_nomm_node->getValue(k);
          cvt_py[i] = track_py_nomm_node->getValue(k);
          cvt_pz[i] = track_pz_nomm_node->getValue(k);
          cvt_vx[i] = track_vx_nomm_node->getValue(k);
          cvt_vy[i] = track_vy_nomm_node->getValue(k);
          cvt_vz[i] = track_vz_nomm_node->getValue(k);

        } else if (pindex == i && detector == DC) {
          dc_sec[i] = track_sector_node->getValue(k);
          dc_px[i]  = track_px_nomm_node->getValue(k);
          dc_py[i]  = track_py_nomm_node->getValue(k);
          dc_pz[i]  = track_pz_nomm_node->getValue(k);
          dc_vx[i]  = track_vx_nomm_node->getValue(k);
          dc_vy[i]  = track_vy_nomm_node->getValue(k);
          dc_vz[i]  = track_vz_nomm_node->getValue(k);
        }
      }
    }

    len_pid    = pid_node->getLength();
    len_pindex = fortag_pindex_node->getLength();

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
        int pindex   = fortag_pindex_node->getValue(k);
        int detector = fortag_detector_node->getValue(k);

        if (pindex == i && detector == FTCAL) {
          ft_cal_energy[i] = fortag_energy_node->getValue(k);
          ft_cal_time[i]   = fortag_time_node->getValue(k);
          ft_cal_path[i]   = fortag_path_node->getValue(k);
          ft_cal_x[i]      = fortag_x_node->getValue(k);
          ft_cal_y[i]      = fortag_y_node->getValue(k);
          ft_cal_z[i]      = fortag_z_node->getValue(k);
          ft_cal_dx[i]     = fortag_dx_node->getValue(k);
          ft_cal_dy[i]     = fortag_dy_node->getValue(k);
          ft_cal_radius[i] = fortag_radius_node->getValue(k);
        } else if (pindex == i && detector == FTHODO) {
          ft_hodo_energy[i] = fortag_energy_node->getValue(k);
          ft_hodo_time[i]   = fortag_time_node->getValue(k);
          ft_hodo_path[i]   = fortag_path_node->getValue(k);
          ft_hodo_x[i]      = fortag_x_node->getValue(k);
          ft_hodo_y[i]      = fortag_y_node->getValue(k);
          ft_hodo_z[i]      = fortag_z_node->getValue(k);
          ft_hodo_dx[i]     = fortag_dx_node->getValue(k);
          ft_hodo_dy[i]     = fortag_dy_node->getValue(k);
          ft_hodo_radius[i] = fortag_radius_node->getValue(k);
        }
      }
    }

    if (cov) {
      len_pid    = pid_node->getLength();
      len_pindex = CovMat_pindex_node->getLength();

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
          int pindex = CovMat_pindex_node->getValue(k);
          if (pindex == i) {
            CovMat_11[i] = CovMat_C11_node->getValue(k);
            CovMat_12[i] = CovMat_C12_node->getValue(k);
            CovMat_13[i] = CovMat_C13_node->getValue(k);
            CovMat_14[i] = CovMat_C14_node->getValue(k);
            CovMat_15[i] = CovMat_C15_node->getValue(k);
            CovMat_22[i] = CovMat_C22_node->getValue(k);
            CovMat_23[i] = CovMat_C23_node->getValue(k);
            CovMat_24[i] = CovMat_C24_node->getValue(k);
            CovMat_25[i] = CovMat_C25_node->getValue(k);
            CovMat_33[i] = CovMat_C33_node->getValue(k);
            CovMat_34[i] = CovMat_C34_node->getValue(k);
            CovMat_35[i] = CovMat_C35_node->getValue(k);
            CovMat_44[i] = CovMat_C44_node->getValue(k);
            CovMat_45[i] = CovMat_C45_node->getValue(k);
            CovMat_55[i] = CovMat_C55_node->getValue(k);
          }
        }
      }
    }

    if (VertDoca) {
      l = VertDoca_index1_node->getLength();
      VertDoca_index1_vec.resize(l);
      VertDoca_index2_vec.resize(l);
      VertDoca_x_vec.resize(l);
      VertDoca_y_vec.resize(l);
      VertDoca_z_vec.resize(l);
      VertDoca_x1_vec.resize(l);
      VertDoca_y1_vec.resize(l);
      VertDoca_z1_vec.resize(l);
      VertDoca_cx1_vec.resize(l);
      VertDoca_cy1_vec.resize(l);
      VertDoca_cz1_vec.resize(l);
      VertDoca_x2_vec.resize(l);
      VertDoca_y2_vec.resize(l);
      VertDoca_z2_vec.resize(l);
      VertDoca_cx2_vec.resize(l);
      VertDoca_cy2_vec.resize(l);
      VertDoca_cz2_vec.resize(l);
      VertDoca_r_vec.resize(l);

      for (int i = 0; i < l; i++) {
        VertDoca_index1_vec[i] = VertDoca_index1_node->getValue(i);
        VertDoca_index2_vec[i] = VertDoca_index2_node->getValue(i);
        VertDoca_x_vec[i]      = VertDoca_x_node->getValue(i);
        VertDoca_y_vec[i]      = VertDoca_y_node->getValue(i);
        VertDoca_z_vec[i]      = VertDoca_z_node->getValue(i);
        VertDoca_x1_vec[i]     = VertDoca_x1_node->getValue(i);
        VertDoca_y1_vec[i]     = VertDoca_y1_node->getValue(i);
        VertDoca_z1_vec[i]     = VertDoca_z1_node->getValue(i);
        VertDoca_cx1_vec[i]    = VertDoca_cx1_node->getValue(i);
        VertDoca_cy1_vec[i]    = VertDoca_cy1_node->getValue(i);
        VertDoca_cz1_vec[i]    = VertDoca_cz1_node->getValue(i);
        VertDoca_x2_vec[i]     = VertDoca_x2_node->getValue(i);
        VertDoca_y2_vec[i]     = VertDoca_y2_node->getValue(i);
        VertDoca_z2_vec[i]     = VertDoca_z2_node->getValue(i);
        VertDoca_cx2_vec[i]    = VertDoca_cx2_node->getValue(i);
        VertDoca_cy2_vec[i]    = VertDoca_cy2_node->getValue(i);
        VertDoca_cz2_vec[i]    = VertDoca_cz2_node->getValue(i);
        VertDoca_r_vec[i]      = VertDoca_r_node->getValue(i);
      }
    }

    if (traj) {
      l = traj_pindex_node->getLength();
      traj_pindex_vec.resize(l);
      traj_index_vec.resize(l);
      traj_detId_vec.resize(l);
      traj_q_vec.resize(l);
      traj_x_vec.resize(l);
      traj_y_vec.resize(l);
      traj_z_vec.resize(l);
      traj_cx_vec.resize(l);
      traj_cy_vec.resize(l);
      traj_cz_vec.resize(l);
      traj_pathlength_vec.resize(l);

      for (int i = 0; i < l; i++) {
        traj_pindex_vec[i]     = traj_pindex_node->getValue(i);
        traj_index_vec[i]      = traj_index_node->getValue(i);
        traj_detId_vec[i]      = traj_detId_node->getValue(i);
        traj_q_vec[i]          = traj_q_node->getValue(i);
        traj_x_vec[i]          = traj_x_node->getValue(i);
        traj_y_vec[i]          = traj_y_node->getValue(i);
        traj_z_vec[i]          = traj_z_node->getValue(i);
        traj_cx_vec[i]         = traj_cx_node->getValue(i);
        traj_cy_vec[i]         = traj_cy_node->getValue(i);
        traj_cz_vec[i]         = traj_cz_node->getValue(i);
        traj_pathlength_vec[i] = traj_pathlength_node->getValue(i);
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
    std::cout << "Events/Sec: " << tot_hipo_events / elapsed_full.count() << " Hz" << std::endl;
    std::cout << "Total events in file: " << tot_hipo_events << std::endl;
    std::cout << "Events converted: " << tot_events_processed << "\t ("
              << 100.0 * tot_events_processed / tot_hipo_events << "%)" << std::endl;
    std::cout << "Events converted/Sec: " << tot_events_processed / elapsed_full.count() << " Hz"
              << std::endl;
  }

  return 0;
}
