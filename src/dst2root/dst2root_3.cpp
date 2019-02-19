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
#include "hipo3/reader.h"

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
  bool        cvt         = false;

  auto cli = (clipp::option("-h", "--help").set(print_help) % "print help",
              clipp::option("-mc", "--MC").set(is_mc) % "Convert dst and mc banks",
              clipp::option("-b", "--batch").set(is_batch) % "Don't show progress and statistics",
              clipp::option("-r", "--rec").set(good_rec) %
                  "Only save events where number of partilces in the event > 0",
              clipp::option("-e", "--elec").set(elec_first) %
                  "Only save events with good electron as first particle",
              clipp::option("-c", "--cov").set(cov) % "Save Covariant Matrix for kinematic fitting",
              clipp::option("-cvt", "--CVTDetector").set(cvt) %
                  "Save CVT information for kinematic fitting",
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
  hipo::reader* reader          = new hipo::reader(InFileName.c_str());
  long          tot_hipo_events = reader->numEvents();

  hipo::node<int32_t>* run_node      = reader->getBranch<int32_t>(11, 1);
  hipo::node<int32_t>* event_node    = reader->getBranch<int32_t>(11, 2);
  hipo::node<float>*   torus_node    = reader->getBranch<float>(11, 8);
  hipo::node<float>*   solenoid_node = reader->getBranch<float>(11, 9);
  hipo::node<int8_t>*  crate_node    = reader->getBranch<int8_t>(20013, 1);
  hipo::node<int8_t>*  slot_node     = reader->getBranch<int8_t>(20013, 2);
  hipo::node<int16_t>* channel_node  = reader->getBranch<int16_t>(20013, 3);
  hipo::node<int8_t>*  helicity_node = reader->getBranch<int8_t>(20013, 4);
  hipo::node<int8_t>*  quartet_node  = reader->getBranch<int8_t>(20013, 5);
  hipo::node<int32_t>* value_node    = reader->getBranch<int32_t>(20013, 6);
  hipo::node<int32_t>* NRUN_node     = reader->getBranch<int32_t>(330, 1);
  hipo::node<int32_t>* NEVENT_node   = reader->getBranch<int32_t>(330, 2);
  hipo::node<float>*   EVNTime_node  = reader->getBranch<float>(330, 3);
  hipo::node<int8_t>*  TYPE_node     = reader->getBranch<int8_t>(330, 4);
  hipo::node<int64_t>* TRG_node      = reader->getBranch<int64_t>(330, 7);
  hipo::node<float>*   BCG_node      = reader->getBranch<float>(330, 8);
  hipo::node<float>*   STTime_node   = reader->getBranch<float>(330, 10);
  hipo::node<float>*   RFTime_node   = reader->getBranch<float>(330, 11);
  hipo::node<int8_t>*  Helic_node    = reader->getBranch<int8_t>(330, 12);

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
  hipo::node<float>*   cal_lu_node       = reader->getBranch<float>(332, 16);
  hipo::node<float>*   cal_lv_node       = reader->getBranch<float>(332, 17);
  hipo::node<float>*   cal_lw_node       = reader->getBranch<float>(332, 18);

  hipo::node<int16_t>* chern_pindex_node   = reader->getBranch<int16_t>(333, 2);
  hipo::node<int8_t>*  chern_detector_node = reader->getBranch<int8_t>(333, 3);
  hipo::node<int8_t>*  chern_sector_node   = reader->getBranch<int8_t>(333, 4);
  hipo::node<float>*   chern_nphe_node     = reader->getBranch<float>(333, 5);
  hipo::node<float>*   chern_time_node     = reader->getBranch<float>(333, 6);
  hipo::node<float>*   chern_path_node     = reader->getBranch<float>(333, 7);
  hipo::node<float>*   chern_theta_node    = reader->getBranch<float>(333, 12);
  hipo::node<float>*   chern_phi_node      = reader->getBranch<float>(333, 13);

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
  // hipo::node<float> *scint_chi2_node = reader->getBranch<float>(335, 10);
  hipo::node<float>* scint_x_node  = reader->getBranch<float>(335, 11);
  hipo::node<float>* scint_y_node  = reader->getBranch<float>(335, 12);
  hipo::node<float>* scint_z_node  = reader->getBranch<float>(335, 13);
  hipo::node<float>* scint_hx_node = reader->getBranch<float>(335, 14);
  hipo::node<float>* scint_hy_node = reader->getBranch<float>(335, 15);
  hipo::node<float>* scint_hz_node = reader->getBranch<float>(335, 16);
  // hipo::node<int16_t> *scint_status_node = reader->getBranch<int16_t>(335, 17);

  hipo::node<int16_t>* track_pindex_node   = reader->getBranch<int16_t>(336, 2);
  hipo::node<int8_t>*  track_detector_node = reader->getBranch<int8_t>(336, 3);
  hipo::node<int8_t>*  track_sector_node   = reader->getBranch<int8_t>(336, 4);
  hipo::node<float>*   track_px_nomm_node  = reader->getBranch<float>(336, 9);
  hipo::node<float>*   track_py_nomm_node  = reader->getBranch<float>(336, 10);
  hipo::node<float>*   track_pz_nomm_node  = reader->getBranch<float>(336, 11);
  hipo::node<float>*   track_vx_nomm_node  = reader->getBranch<float>(336, 12);
  hipo::node<float>*   track_vy_nomm_node  = reader->getBranch<float>(336, 13);
  hipo::node<float>*   track_vz_nomm_node  = reader->getBranch<float>(336, 14);

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

  hipo::node<int16_t>* CVT_pid_node         = reader->getBranch<int16_t>(20526, 1);
  hipo::node<int8_t>*  CVT_q_node           = reader->getBranch<int8_t>(20526, 10);
  hipo::node<float>*   CVT_p_node           = reader->getBranch<float>(20526, 11);
  hipo::node<float>*   CVT_pt_node          = reader->getBranch<float>(20526, 12);
  hipo::node<float>*   CVT_phi0_node        = reader->getBranch<float>(20526, 13);
  hipo::node<float>*   CVT_tandip_node      = reader->getBranch<float>(20526, 14);
  hipo::node<float>*   CVT_z0_node          = reader->getBranch<float>(20526, 15);
  hipo::node<float>*   CVT_d0_node          = reader->getBranch<float>(20526, 16);
  hipo::node<float>*   CVT_Cov_d02_node     = reader->getBranch<float>(20526, 17);
  hipo::node<float>*   CVT_Cov_d0phi0_node  = reader->getBranch<float>(20526, 18);
  hipo::node<float>*   CVT_Cov_d0rho_node   = reader->getBranch<float>(20526, 19);
  hipo::node<float>*   CVT_Cov_phi02_node   = reader->getBranch<float>(20526, 20);
  hipo::node<float>*   CVT_Cov_phi0rho_node = reader->getBranch<float>(20526, 21);
  hipo::node<float>*   CVT_Cov_rho2_node    = reader->getBranch<float>(20526, 22);
  hipo::node<float>*   CVT_Cov_z02_node     = reader->getBranch<float>(20526, 23);
  hipo::node<float>*   CVT_Cov_tandip2_node = reader->getBranch<float>(20526, 24);

  std::vector<int>   run;
  std::vector<int>   event;
  std::vector<float> torus;
  std::vector<float> solenoid;
  std::vector<int>   crate;
  std::vector<int>   slot;
  std::vector<int>   channel;
  std::vector<int>   helicity;
  std::vector<int>   quartet;
  std::vector<int>   value;
  std::vector<int>   NRUN;
  std::vector<int>   NEVENT;
  std::vector<float> EVNTime;
  std::vector<int>   TYPE;
  std::vector<int>   TRG;
  std::vector<float> BCG;
  std::vector<float> STTime;
  std::vector<float> RFTime;
  std::vector<int>   Helic;

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

  std::vector<int>   cvt_pid;
  std::vector<int>   cvt_q;
  std::vector<float> cvt_p;
  std::vector<float> cvt_pt;
  std::vector<float> cvt_phi0;
  std::vector<float> cvt_tandip;
  std::vector<float> cvt_z0;
  std::vector<float> cvt_d0;
  std::vector<float> cvt_CovMat_d02;
  std::vector<float> cvt_CovMat_d0phi0;
  std::vector<float> cvt_CovMat_d0rho;
  std::vector<float> cvt_CovMat_phi02;
  std::vector<float> cvt_CovMat_phi0rho;
  std::vector<float> cvt_CovMat_rho2;
  std::vector<float> cvt_CovMat_z02;
  std::vector<float> cvt_CovMat_tandip2;

  clas12->Branch("run", &run);
  clas12->Branch("event", &event);
  clas12->Branch("torus", &torus);
  clas12->Branch("solenoid", &solenoid);
  clas12->Branch("crate", &crate);
  clas12->Branch("slot", &slot);
  clas12->Branch("channel", &channel);
  clas12->Branch("helicity", &helicity);
  clas12->Branch("quartet", &quartet);
  clas12->Branch("value", &value);
  clas12->Branch("STTime", &STTime);
  clas12->Branch("RFTime", &RFTime);

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
  if (cvt) {
    clas12->Branch("cvt_pid", &cvt_pid);
    clas12->Branch("cvt_q", &cvt_q);
    clas12->Branch("cvt_p", &cvt_p);
    clas12->Branch("cvt_pt", &cvt_pt);
    clas12->Branch("cvt_phi0", &cvt_phi0);
    clas12->Branch("cvt_tandip", &cvt_tandip);
    clas12->Branch("cvt_z0", &cvt_z0);
    clas12->Branch("cvt_d0", &cvt_d0);
    clas12->Branch("cvt_CovMat_d02", &cvt_CovMat_d02);
    clas12->Branch("cvt_CovMat_d0rho", &cvt_CovMat_d0rho);
    clas12->Branch("cvt_CovMat_phi02", &cvt_CovMat_phi02);
    clas12->Branch("cvt_CovMat_phi0rho", &cvt_CovMat_phi0rho);
    clas12->Branch("cvt_CovMat_rho2", &cvt_CovMat_rho2);
    clas12->Branch("cvt_CovMat_z02", &cvt_CovMat_z02);
    clas12->Branch("cvt_CovMat_tandip2", &cvt_CovMat_tandip2);
  }

  int entry      = 0;
  int l          = 0;
  int len_pid    = 0;
  int len_pindex = 0;

  while (reader->next() == true) {

    // entry++;
    if (!is_batch && (++entry % 1000) == 0)
      std::cout << "\t" << floor(100 * entry / tot_hipo_events) << "%\r\r" << std::flush;

    if (good_rec && pid_node->getLength() == 0)
      continue;
    if (elec_first && pid_node->getValue(0) != 11)
      continue;

    l = run_node->getLength();
    run.resize(l);
    event.resize(l);
    torus.resize(l);
    solenoid.resize(l);

    for (int i = 0; i < l; i++) {
      run[i]      = run_node->getValue(i);
      event[i]    = event_node->getValue(i);
      torus[i]    = torus_node->getValue(i);
      solenoid[i] = solenoid_node->getValue(i);
    }

    l = crate_node->getLength();
    crate.resize(l);
    slot.resize(l);
    channel.resize(l);
    helicity.resize(l);
    quartet.resize(l);
    value.resize(l);

    for (int i = 0; i < l; i++) {
      crate[i]    = crate_node->getValue(i);
      slot[i]     = slot_node->getValue(i);
      channel[i]  = channel_node->getValue(i);
      helicity[i] = helicity_node->getValue(i);
      quartet[i]  = quartet_node->getValue(i);
      value[i]    = value_node->getValue(i);
    }

    l = STTime_node->getLength();
    STTime.resize(l);
    RFTime.resize(l);

    for (int i = 0; i < l; i++) {
      STTime[i] = STTime_node->getValue(i);
      RFTime[i] = RFTime_node->getValue(i);
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
            ec_pcal_lu[i]   = cal_lu_node->getValue(k);
            ec_pcal_lv[i]   = cal_lv_node->getValue(k);
            ec_pcal_lw[i]   = cal_lw_node->getValue(k);
          } else if (layer == EC_INNER) {
            einner += energy;
            ec_ecin_sec[i]  = cal_sector_node->getValue(k);
            ec_ecin_time[i] = cal_time_node->getValue(k);
            ec_ecin_path[i] = cal_path_node->getValue(k);
            ec_ecin_x[i]    = cal_x_node->getValue(k);
            ec_ecin_y[i]    = cal_y_node->getValue(k);
            ec_ecin_z[i]    = cal_z_node->getValue(k);
            ec_ecin_lu[i]   = cal_lu_node->getValue(k);
            ec_ecin_lv[i]   = cal_lv_node->getValue(k);
            ec_ecin_lw[i]   = cal_lw_node->getValue(k);
          } else if (layer == EC_OUTER) {
            eouter += energy;
            ec_ecout_sec[i]  = cal_sector_node->getValue(k);
            ec_ecout_time[i] = cal_time_node->getValue(k);
            ec_ecout_path[i] = cal_path_node->getValue(k);
            ec_ecout_x[i]    = cal_x_node->getValue(k);
            ec_ecout_y[i]    = cal_y_node->getValue(k);
            ec_ecout_z[i]    = cal_z_node->getValue(k);
            ec_ecout_lu[i]   = cal_lu_node->getValue(k);
            ec_ecout_lv[i]   = cal_lv_node->getValue(k);
            ec_ecout_lw[i]   = cal_lw_node->getValue(k);
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
        } else if (pindex == i && detector == LTCC) {
          cc_ltcc_nphe[i]  = chern_nphe_node->getValue(k);
          cc_ltcc_sec[i]   = chern_sector_node->getValue(k);
          cc_ltcc_time[i]  = chern_time_node->getValue(k);
          cc_ltcc_path[i]  = chern_path_node->getValue(k);
          cc_ltcc_theta[i] = chern_theta_node->getValue(k);
          cc_ltcc_phi[i]   = chern_phi_node->getValue(k);
        } else if (pindex == i && detector == RICH) {
          cc_rich_nphe[i]  = chern_nphe_node->getValue(k);
          cc_rich_sec[i]   = chern_sector_node->getValue(k);
          cc_rich_time[i]  = chern_time_node->getValue(k);
          cc_rich_path[i]  = chern_path_node->getValue(k);
          cc_rich_theta[i] = chern_theta_node->getValue(k);
          cc_rich_phi[i]   = chern_phi_node->getValue(k);
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

    if (cvt) {
      len_pid = CVT_pid_node->getLength();
      l       = CVT_pid_node->getLength();

      cvt_pid.resize(len_pid);
      cvt_q.resize(len_pid);
      cvt_p.resize(len_pid);
      cvt_pt.resize(len_pid);
      cvt_phi0.resize(len_pid);
      cvt_tandip.resize(len_pid);
      cvt_z0.resize(len_pid);
      cvt_d0.resize(len_pid);
      cvt_CovMat_d02.resize(len_pid);
      cvt_CovMat_d0phi0.resize(len_pid);
      cvt_CovMat_d0rho.resize(len_pid);
      cvt_CovMat_phi02.resize(len_pid);
      cvt_CovMat_phi0rho.resize(len_pid);
      cvt_CovMat_rho2.resize(len_pid);
      cvt_CovMat_z02.resize(len_pid);
      cvt_CovMat_tandip2.resize(len_pid);

      for (int i = 0; i < len_pid; i++) {
        cvt_pid[i]            = -1;
        cvt_q[i]              = -1;
        cvt_p[i]              = NAN;
        cvt_pt[i]             = NAN;
        cvt_phi0[i]           = NAN;
        cvt_tandip[i]         = NAN;
        cvt_z0[i]             = NAN;
        cvt_d0[i]             = NAN;
        cvt_CovMat_d02[i]     = NAN;
        cvt_CovMat_d0phi0[i]  = NAN;
        cvt_CovMat_d0rho[i]   = NAN;
        cvt_CovMat_phi02[i]   = NAN;
        cvt_CovMat_phi0rho[i] = NAN;
        cvt_CovMat_rho2[i]    = NAN;
        cvt_CovMat_z02[i]     = NAN;
        cvt_CovMat_tandip2[i] = NAN;
      }

      for (int i = 0; i < len_pid; i++) {
        for (int k = 0; k < len_pindex; ++k) {
          int pindex = CovMat_pindex_node->getValue(k);
          if (pindex == i) {
            cvt_pid[i]            = CVT_pid_node->getValue(k);
            cvt_q[i]              = CVT_q_node->getValue(k);
            cvt_p[i]              = CVT_p_node->getValue(k);
            cvt_phi0[i]           = CVT_phi0_node->getValue(k);
            cvt_tandip[i]         = CVT_tandip_node->getValue(k);
            cvt_z0[i]             = CVT_z0_node->getValue(k);
            cvt_d0[i]             = CVT_d0_node->getValue(k);
            cvt_CovMat_d02[i]     = CVT_Cov_d02_node->getValue(k);
            cvt_CovMat_d0phi0[i]  = CVT_Cov_d0phi0_node->getValue(k);
            cvt_CovMat_d0rho[i]   = CVT_Cov_d0rho_node->getValue(k);
            cvt_CovMat_phi02[i]   = CVT_Cov_phi02_node->getValue(k);
            cvt_CovMat_phi0rho[i] = CVT_Cov_phi0rho_node->getValue(k);
            cvt_CovMat_rho2[i]    = CVT_Cov_rho2_node->getValue(k);
            cvt_CovMat_z02[i]     = CVT_Cov_z02_node->getValue(k);
            cvt_CovMat_tandip2[i] = CVT_Cov_tandip2_node->getValue(k);
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
