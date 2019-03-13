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
std::vector<float> ec_pcal_hx;
std::vector<float> ec_pcal_hy;
std::vector<float> ec_pcal_hz;
std::vector<float> ec_pcal_lu;
std::vector<float> ec_pcal_lv;
std::vector<float> ec_pcal_lw;
std::vector<float> ec_pcal_du;
std::vector<float> ec_pcal_dv;
std::vector<float> ec_pcal_dw;
std::vector<float> ec_pcal_m2u;
std::vector<float> ec_pcal_m2v;
std::vector<float> ec_pcal_m2w;
std::vector<float> ec_pcal_m3u;
std::vector<float> ec_pcal_m3v;
std::vector<float> ec_pcal_m3w;

std::vector<float> ec_ecin_energy;
std::vector<int>   ec_ecin_sec;
std::vector<float> ec_ecin_time;
std::vector<float> ec_ecin_path;
std::vector<float> ec_ecin_x;
std::vector<float> ec_ecin_y;
std::vector<float> ec_ecin_z;
std::vector<float> ec_ecin_hx;
std::vector<float> ec_ecin_hy;
std::vector<float> ec_ecin_hz;
std::vector<float> ec_ecin_lu;
std::vector<float> ec_ecin_lv;
std::vector<float> ec_ecin_lw;
std::vector<float> ec_ecin_du;
std::vector<float> ec_ecin_dv;
std::vector<float> ec_ecin_dw;
std::vector<float> ec_ecin_m2u;
std::vector<float> ec_ecin_m2v;
std::vector<float> ec_ecin_m2w;
std::vector<float> ec_ecin_m3u;
std::vector<float> ec_ecin_m3v;
std::vector<float> ec_ecin_m3w;

std::vector<float> ec_ecout_energy;
std::vector<int>   ec_ecout_sec;
std::vector<float> ec_ecout_time;
std::vector<float> ec_ecout_path;
std::vector<float> ec_ecout_x;
std::vector<float> ec_ecout_y;
std::vector<float> ec_ecout_z;
std::vector<float> ec_ecout_hx;
std::vector<float> ec_ecout_hy;
std::vector<float> ec_ecout_hz;
std::vector<float> ec_ecout_lu;
std::vector<float> ec_ecout_lv;
std::vector<float> ec_ecout_lw;
std::vector<float> ec_ecout_du;
std::vector<float> ec_ecout_dv;
std::vector<float> ec_ecout_dw;
std::vector<float> ec_ecout_m2u;
std::vector<float> ec_ecout_m2v;
std::vector<float> ec_ecout_m2w;
std::vector<float> ec_ecout_m3u;
std::vector<float> ec_ecout_m3v;
std::vector<float> ec_ecout_m3w;

std::vector<float> cc_nphe_tot;
std::vector<int>   cc_ltcc_sec;
std::vector<float> cc_ltcc_nphe;
std::vector<float> cc_ltcc_time;
std::vector<float> cc_ltcc_path;
std::vector<float> cc_ltcc_theta;
std::vector<float> cc_ltcc_phi;
std::vector<float> cc_ltcc_x;
std::vector<float> cc_ltcc_y;
std::vector<float> cc_ltcc_z;
std::vector<int>   cc_htcc_sec;
std::vector<float> cc_htcc_nphe;
std::vector<float> cc_htcc_time;
std::vector<float> cc_htcc_path;
std::vector<float> cc_htcc_theta;
std::vector<float> cc_htcc_phi;
std::vector<float> cc_htcc_x;
std::vector<float> cc_htcc_y;
std::vector<float> cc_htcc_z;
std::vector<int>   cc_rich_sec;
std::vector<float> cc_rich_nphe;
std::vector<float> cc_rich_time;
std::vector<float> cc_rich_path;
std::vector<float> cc_rich_theta;
std::vector<float> cc_rich_phi;
std::vector<float> cc_rich_x;
std::vector<float> cc_rich_y;
std::vector<float> cc_rich_z;

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

std::vector<int>   VertDoca_index1_vec;
std::vector<int>   VertDoca_index2_vec;
std::vector<float> VertDoca_x_vec;
std::vector<float> VertDoca_y_vec;
std::vector<float> VertDoca_z_vec;
std::vector<float> VertDoca_x1_vec;
std::vector<float> VertDoca_y1_vec;
std::vector<float> VertDoca_z1_vec;
std::vector<float> VertDoca_cx1_vec;
std::vector<float> VertDoca_cy1_vec;
std::vector<float> VertDoca_cz1_vec;
std::vector<float> VertDoca_x2_vec;
std::vector<float> VertDoca_y2_vec;
std::vector<float> VertDoca_z2_vec;
std::vector<float> VertDoca_cx2_vec;
std::vector<float> VertDoca_cy2_vec;
std::vector<float> VertDoca_cz2_vec;
std::vector<float> VertDoca_r_vec;

std::vector<int>   traj_pindex_vec;
std::vector<int>   traj_index_vec;
std::vector<int>   traj_detId_vec;
std::vector<int>   traj_q_vec;
std::vector<float> traj_x_vec;
std::vector<float> traj_y_vec;
std::vector<float> traj_z_vec;
std::vector<float> traj_cx_vec;
std::vector<float> traj_cy_vec;
std::vector<float> traj_cz_vec;
std::vector<float> traj_pathlength_vec;