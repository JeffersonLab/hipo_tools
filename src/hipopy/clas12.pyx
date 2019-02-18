# cython: profile=True
# distutils: language = c++
# cython: boundscheck=False

cimport cython

cimport cpython
cimport libc.stdlib
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref

from libcpp.string cimport string
from libcpp cimport bool
from libcpp.map cimport map
from libc.stdlib cimport free
import numpy as np
cimport numpy as np



cdef float NAN = float("NAN")

cdef dict clas12_detector = {"BMT":  1,
"BST":  2,
"CND":  3,
"CTOF":  4,
"CVT":  5,
"DC":  6,
"ECAL":  7,
"FMT":  8,
"FT":  9,
"FTCAL": 10,
"FTHODO": 11,
"FTOF": 12,
"FTTRK": 13,
"HTCC": 15,
"LTCC": 16,
"RF": 17,
"RICH": 18,
"RTPC": 19,
"HEL": 20,
"BAND": 21}


cdef dict clas12_layer = {"FTOF_1A": 1,"FTOF_1B": 2,"FTOF_2": 3,"PCAL": 1,"EC_INNER": 4,"EC_OUTER": 7}

cdef dict get_id = {'PROTON': 2212, 'NEUTRON': 2112, 'PIP': 211, 'PIM': -211, 'PI0': 111, 'KP': 321, 'KM': -321, 'PHOTON': 22, 'ELECTRON': 11}
cdef dict part_mass = {11: 0.000511, 211: 0.13957, -211: 0.13957, 2212: 0.93827, 2112: 0.939565, 321: 0.493667, -321: 0.493667, 22: 0}

cdef extern from "hipo4/dictionary.h" namespace "hipo":
    cdef cppclass schema:
      schema() except +
      string json()

cdef extern from "hipo4/dictionary.h" namespace "hipo":
    cdef cppclass dictionary:
      dictionary() except +
      schema getSchema(string)
      schema getSchema(char *)

cdef extern from "hipo4/bank.h" namespace "hipo":
    cdef cppclass bank:
      bank() except +
      bank(schema) except +
      int    getRows()
      int    getInt(char*, int)
      int    getShort(char*, int)
      int    getByte(char*, int)
      float  getFloat(char*, int)
      double getDouble(char*, int)
      long   getLong(char*, int)
      int    getInt(string, int)
      int    getShort(string, int)
      int    getByte(string, int)
      float  getFloat(string, int)
      double getDouble(string, int)
      long   getLong(string, int)

cdef extern from "hipo4/event.h" namespace "hipo":
    cdef cppclass event:
      event() except +
      void getStructure(bank)


cdef extern from "hipo4/reader.h" namespace "hipo":
    cdef cppclass reader:
      reader() except +
      reader(char*) except +
      reader(string) except +
      void        read(event)
      dictionary* dictionary()
      long numEvents()
      void readDictionary(dictionary)
      void open(string)
      bool hasNext()
      bool next()

cdef char* str_to_char(str name):
  """Convert python string to char*"""
  cdef bytes name_bytes = name.encode()
  cdef char* c_name = name_bytes
  return c_name

cdef extern from "TLorentzVector.h":
  cdef cppclass TLorentzVector:
    TLorentzVector() except +
    TLorentzVector(double x, double y, double z, double t) except +
    void Boost(double, double, double)
    void SetXYZM(double x, double y, double z, double m)
    void SetXYZT (double x, double y, double z, double t)
    double Px()
    double Py()
    double Pz()
    double P()
    double E()
    double Energy()
    double Theta()
    double CosTheta()
    double Phi()
    double Rho()
    double Perp2()
    double Pt()
    double Perp()
    double Et2()
    double Et()
    double Mag2()
    double M2()
    double Mag()
    double M()
    double Mt2()
    double Mt()
    double Beta()
    double Gamma()
    double Plus()
    double Minus()
    double Rapidity()
    double Eta()
    double PseudoRapidity()

cdef class LorentzVector:
  cdef unique_ptr[TLorentzVector] c_TLorentzVector
  def __cinit__(LorentzVector self, double px, double py, double pz, **kwargs):
    if "energy" in kwargs:
      self.c_TLorentzVector.reset(new TLorentzVector(px, py, pz, kwargs["energy"]))
    elif "mass" in kwargs:
      self.c_TLorentzVector.reset(new TLorentzVector())
      deref(self.c_TLorentzVector).SetXYZM(px, py, pz, kwargs["mass"])
    elif "pid" in kwargs:
      self.c_TLorentzVector.reset(new TLorentzVector())
      deref(self.c_TLorentzVector).SetXYZM(px, py, pz, part_mass[kwargs["pid"]])
    else:
      self.c_TLorentzVector.reset(new TLorentzVector(px, py, pz, 0))
  def __add__(LorentzVector self, LorentzVector other):
    cdef double X = deref(self.c_TLorentzVector).Px() + deref(other.c_TLorentzVector).Px()
    cdef double Y = deref(self.c_TLorentzVector).Py() + deref(other.c_TLorentzVector).Py()
    cdef double Z = deref(self.c_TLorentzVector).Pz() + deref(other.c_TLorentzVector).Pz()
    cdef double E = deref(self.c_TLorentzVector).E() + deref(other.c_TLorentzVector).E()
    return LorentzVector(X, Y, Z, energy=E)
  def __iadd__(LorentzVector self, LorentzVector other):
    cdef double X = deref(self.c_TLorentzVector).Px() + deref(other.c_TLorentzVector).Px()
    cdef double Y = deref(self.c_TLorentzVector).Py() + deref(other.c_TLorentzVector).Py()
    cdef double Z = deref(self.c_TLorentzVector).Pz() + deref(other.c_TLorentzVector).Pz()
    cdef double E = deref(self.c_TLorentzVector).E() + deref(other.c_TLorentzVector).E()
    return LorentzVector(X, Y, Z, energy=E)
  def __sub__(LorentzVector self, LorentzVector other):
    cdef double X = deref(self.c_TLorentzVector).Px() - deref(other.c_TLorentzVector).Px()
    cdef double Y = deref(self.c_TLorentzVector).Py() - deref(other.c_TLorentzVector).Py()
    cdef double Z = deref(self.c_TLorentzVector).Pz() - deref(other.c_TLorentzVector).Pz()
    cdef double E = deref(self.c_TLorentzVector).E() - deref(other.c_TLorentzVector).E()
    return LorentzVector(X, Y, Z, energy=E)
  def __isub__(LorentzVector self, LorentzVector other):
    cdef double X = deref(self.c_TLorentzVector).Px() - deref(other.c_TLorentzVector).Px()
    cdef double Y = deref(self.c_TLorentzVector).Py() - deref(other.c_TLorentzVector).Py()
    cdef double Z = deref(self.c_TLorentzVector).Pz() - deref(other.c_TLorentzVector).Pz()
    cdef double E = deref(self.c_TLorentzVector).E() - deref(other.c_TLorentzVector).E()
    return LorentzVector(X, Y, Z, energy=E)
  def __str__(self):
    return "Px {0: 0.2f} | Py {1: 0.2f} | Pz {2: 0.2f} | E {3: 0.2f}".format(self.Px,self.Py ,self.Pz, self.E)
  def __repr__(self):
    return self.__str__()
  def SetPxPyPzM(LorentzVector self, double px, double py, double pz, double mass):
    deref(self.c_TLorentzVector).SetXYZM(px, py, pz, mass)
  @property
  def Px(LorentzVector self):
    return deref(self.c_TLorentzVector).Px()
  @property
  def Py(LorentzVector self):
    return deref(self.c_TLorentzVector).Py()
  @property
  def Pz(LorentzVector self):
    return deref(self.c_TLorentzVector).Pz()
  @property
  def P(LorentzVector self):
    return deref(self.c_TLorentzVector).P()
  @property
  def E(LorentzVector self):
    return deref(self.c_TLorentzVector).E()
  @property
  def Energy(LorentzVector self):
    return deref(self.c_TLorentzVector).E()
  @property
  def Theta(LorentzVector self):
    return deref(self.c_TLorentzVector).Theta()
  @property
  def CosTheta(LorentzVector self):
    return deref(self.c_TLorentzVector).CosTheta()
  @property
  def Phi(LorentzVector self):
    return deref(self.c_TLorentzVector).Phi()
  @property
  def Rho(LorentzVector self):
    return deref(self.c_TLorentzVector).Rho()
  @property
  def Perp2(LorentzVector self):
    return deref(self.c_TLorentzVector).Perp2()
  @property
  def Pt(LorentzVector self):
    return deref(self.c_TLorentzVector).Pt()
  @property
  def Perp(LorentzVector self):
    return deref(self.c_TLorentzVector).Perp()
  @property
  def Et2(LorentzVector self):
    return deref(self.c_TLorentzVector).Et2()
  @property
  def Et(LorentzVector self):
    return deref(self.c_TLorentzVector).Et()
  @property
  def Mag2(LorentzVector self):
    return deref(self.c_TLorentzVector).Mag2()
  @property
  def M2(LorentzVector self):
    return deref(self.c_TLorentzVector).M2()
  @property
  def Mag(LorentzVector self):
    return deref(self.c_TLorentzVector).Mag()
  @property
  def M(LorentzVector self):
    return deref(self.c_TLorentzVector).M()
  @property
  def Mt2(LorentzVector self):
    return deref(self.c_TLorentzVector).Mt2()
  @property
  def Mt(LorentzVector self):
    return deref(self.c_TLorentzVector).Mt()
  @property
  def Beta(LorentzVector self):
    return deref(self.c_TLorentzVector).Beta()
  @property
  def Gamma(LorentzVector self):
    return deref(self.c_TLorentzVector).Gamma()
  @property
  def Plus(LorentzVector self):
    return deref(self.c_TLorentzVector).Plus()
  @property
  def Minus(LorentzVector self):
    return deref(self.c_TLorentzVector).Minus()
  @property
  def Rapidity(LorentzVector self):
    return deref(self.c_TLorentzVector).Rapidity()
  @property
  def Eta(LorentzVector self):
    return deref(self.c_TLorentzVector).Eta()
  @property
  def PseudoRapidity(LorentzVector self):
    return deref(self.c_TLorentzVector).PseudoRapidity()

cdef class clas12Event:
  cdef:
    reader*c_hiporeader
    dictionary*c_dict
    event*c_hipo_event
    bank*c_ForwardTagger
    bank*c_Track
    bank*c_Cherenkov
    bank*c_Particle
    bank*c_Scintillator
    bank*c_Calorimeter
    # EC
    np.ndarray _ec_tot_energy
    np.ndarray _ec_pcal_energy
    np.ndarray _ec_pcal_sec
    np.ndarray _ec_pcal_time
    np.ndarray _ec_pcal_path
    np.ndarray _ec_pcal_x
    np.ndarray _ec_pcal_y
    np.ndarray _ec_pcal_z
    np.ndarray _ec_pcal_lu
    np.ndarray _ec_pcal_lv
    np.ndarray _ec_pcal_lw
    np.ndarray _ec_ecin_energy
    np.ndarray _ec_ecin_sec
    np.ndarray _ec_ecin_time
    np.ndarray _ec_ecin_path
    np.ndarray _ec_ecin_x
    np.ndarray _ec_ecin_y
    np.ndarray _ec_ecin_z
    np.ndarray _ec_ecin_lu
    np.ndarray _ec_ecin_lv
    np.ndarray _ec_ecin_lw
    np.ndarray _ec_ecout_energy
    np.ndarray _ec_ecout_sec
    np.ndarray _ec_ecout_time
    np.ndarray _ec_ecout_path
    np.ndarray _ec_ecout_x
    np.ndarray _ec_ecout_y
    np.ndarray _ec_ecout_z
    np.ndarray _ec_ecout_lu
    np.ndarray _ec_ecout_lv
    np.ndarray _ec_ecout_lw

    # SC
    np.ndarray _sc_ftof_1a_sec
    np.ndarray _sc_ftof_1a_time
    np.ndarray _sc_ftof_1a_path
    np.ndarray _sc_ftof_1a_energy
    np.ndarray _sc_ftof_1a_component
    np.ndarray _sc_ftof_1a_x
    np.ndarray _sc_ftof_1a_y
    np.ndarray _sc_ftof_1a_z
    np.ndarray _sc_ftof_1a_hx
    np.ndarray _sc_ftof_1a_hy
    np.ndarray _sc_ftof_1a_hz
    np.ndarray _sc_ftof_1b_sec
    np.ndarray _sc_ftof_1b_time
    np.ndarray _sc_ftof_1b_path
    np.ndarray _sc_ftof_1b_energy
    np.ndarray _sc_ftof_1b_component
    np.ndarray _sc_ftof_1b_x
    np.ndarray _sc_ftof_1b_y
    np.ndarray _sc_ftof_1b_z
    np.ndarray _sc_ftof_1b_hx
    np.ndarray _sc_ftof_1b_hy
    np.ndarray _sc_ftof_1b_hz
    np.ndarray _sc_ftof_2_sec
    np.ndarray _sc_ftof_2_time
    np.ndarray _sc_ftof_2_path
    np.ndarray _sc_ftof_2_energy
    np.ndarray _sc_ftof_2_component
    np.ndarray _sc_ftof_2_x
    np.ndarray _sc_ftof_2_y
    np.ndarray _sc_ftof_2_z
    np.ndarray _sc_ftof_2_hx
    np.ndarray _sc_ftof_2_hy
    np.ndarray _sc_ftof_2_hz
    np.ndarray _sc_ctof_time
    np.ndarray _sc_ctof_path
    np.ndarray _sc_ctof_energy
    np.ndarray _sc_ctof_component
    np.ndarray _sc_ctof_x
    np.ndarray _sc_ctof_y
    np.ndarray _sc_ctof_z
    np.ndarray _sc_ctof_hx
    np.ndarray _sc_ctof_hy
    np.ndarray _sc_ctof_hz
    np.ndarray _sc_cnd_time
    np.ndarray _sc_cnd_path
    np.ndarray _sc_cnd_energy
    np.ndarray _sc_cnd_component
    np.ndarray _sc_cnd_x
    np.ndarray _sc_cnd_y
    np.ndarray _sc_cnd_z
    np.ndarray _sc_cnd_hx
    np.ndarray _sc_cnd_hy
    np.ndarray _sc_cnd_hz

    # CC
    np.ndarray _cc_nphe_tot
    np.ndarray _cc_ltcc_sec
    np.ndarray _cc_ltcc_nphe
    np.ndarray _cc_ltcc_time
    np.ndarray _cc_ltcc_path
    np.ndarray _cc_ltcc_theta
    np.ndarray _cc_ltcc_phi
    np.ndarray _cc_htcc_sec
    np.ndarray _cc_htcc_nphe
    np.ndarray _cc_htcc_time
    np.ndarray _cc_htcc_path
    np.ndarray _cc_htcc_theta
    np.ndarray _cc_htcc_phi
    np.ndarray _cc_rich_sec
    np.ndarray _cc_rich_nphe
    np.ndarray _cc_rich_time
    np.ndarray _cc_rich_path
    np.ndarray _cc_rich_theta
    np.ndarray _cc_rich_phi
    # FT
    np.ndarray _ft_cal_energy
    np.ndarray _ft_cal_time
    np.ndarray _ft_cal_path
    np.ndarray _ft_cal_x
    np.ndarray _ft_cal_y
    np.ndarray _ft_cal_z
    np.ndarray _ft_cal_dx
    np.ndarray _ft_cal_dy
    np.ndarray _ft_cal_radius
    np.ndarray _ft_hodo_energy
    np.ndarray _ft_hodo_time
    np.ndarray _ft_hodo_path
    np.ndarray _ft_hodo_x
    np.ndarray _ft_hodo_y
    np.ndarray _ft_hodo_z
    np.ndarray _ft_hodo_dx
    np.ndarray _ft_hodo_dy
    np.ndarray _ft_hodo_radius

    # Trk
    np.ndarray _dc_sec
    np.ndarray _dc_px
    np.ndarray _dc_py
    np.ndarray _dc_pz
    np.ndarray _dc_vx
    np.ndarray _dc_vy
    np.ndarray _dc_vz

    np.ndarray _cvt_px
    np.ndarray _cvt_py
    np.ndarray _cvt_pz
    np.ndarray _cvt_vx
    np.ndarray _cvt_vy
    np.ndarray _cvt_vz

  def __cinit__(clas12Event self, string filename):
    self.c_hiporeader = new reader(filename)
    self.c_dict = self.c_hiporeader.dictionary()
    self.c_hipo_event = new event()
    self.c_ForwardTagger = new bank(self.c_dict.getSchema(str_to_char("REC::ForwardTagger")))
    self.c_Track = new bank(self.c_dict.getSchema(str_to_char("REC::Track")))
    self.c_Cherenkov = new bank(self.c_dict.getSchema(str_to_char("REC::Cherenkov")))
    self.c_Particle = new bank(self.c_dict.getSchema(str_to_char("REC::Particle")))
    self.c_Scintillator = new bank(self.c_dict.getSchema(str_to_char("REC::Scintillator")))
    self.c_Calorimeter = new bank(self.c_dict.getSchema(str_to_char("REC::Calorimeter")))


  def __iter__(clas12Event self):
      return self
  def __str__(clas12Event self):
    out = "["
    out += self.c_dict.getSchema(str_to_char("REC::Particle")).json()
    out += self.c_dict.getSchema(str_to_char("REC::ForwardTagger")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Track")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Cherenkov")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Scintillator")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Calorimeter")).json()
    out += "]"
    return out
  def __next__(clas12Event self):
    if self.c_hiporeader.next():
      self.c_hiporeader.read(self.c_hipo_event[0])
      self.c_hipo_event.getStructure(self.c_Particle[0])
      self.c_hipo_event.getStructure(self.c_ForwardTagger[0])
      self.load_ft()
      self.c_hipo_event.getStructure(self.c_Track[0])
      self.load_trk()
      self.c_hipo_event.getStructure(self.c_Cherenkov[0])
      self.load_cc()
      self.c_hipo_event.getStructure(self.c_Scintillator[0])
      self.load_sc()
      self.c_hipo_event.getStructure(self.c_Calorimeter[0])
      self.load_cal()
      return self
    else:
      raise StopIteration
  def numEvents(clas12Event self):
    return self.c_hiporeader.numEvents()
  def __len__(clas12Event self):
    return self.c_Particle.getRows()
  def pid(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return -9999
    return self.c_Particle.getInt(str_to_char("pid"),i)
  def px(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("px"),i)
  def py(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("py"),i)
  def pz(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("pz"),i)
  def vx(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("vx"),i)
  def vy(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("vy"),i)
  def vz(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("vz"),i)
  def charge(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return -9999
    return self.c_Particle.getByte(str_to_char("charge"), i)
  def beta(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("beta"), i)
  def chi2pid(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(str_to_char("chi2pid"), i)
  def status(clas12Event self, int i):
    if i >= self.c_Particle.getRows():
      return -1
    return self.c_Particle.getShort(str_to_char("status"), i)

  cdef void load_ft(clas12Event self):
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_ForwardTagger.getRows()

    self._ft_cal_energy = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_time = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_path = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_x = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_y = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_z = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_dx = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_dy = np.ones(len_pid, dtype=float) * NAN
    self._ft_cal_radius = np.ones(len_pid, dtype=float) * NAN

    self._ft_hodo_energy = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_time = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_path = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_x = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_y = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_z = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_dx = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_dy = np.ones(len_pid, dtype=float) * NAN
    self._ft_hodo_radius = np.ones(len_pid, dtype=float) * NAN

    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_ForwardTagger.getShort(str_to_char("pindex"), k)
        detector = self.c_ForwardTagger.getByte(str_to_char("detector"), k)

        if pindex == i and detector == clas12_detector["FTCAL"]:
          self._ft_cal_energy[i] = self.c_ForwardTagger.getFloat(str_to_char("energy"), k)
          self._ft_cal_time[i] = self.c_ForwardTagger.getFloat(str_to_char("time"), k)
          self._ft_cal_path[i] = self.c_ForwardTagger.getFloat(str_to_char("path"), k)
          self._ft_cal_x[i] = self.c_ForwardTagger.getFloat(str_to_char("x"), k)
          self._ft_cal_y[i] = self.c_ForwardTagger.getFloat(str_to_char("y"), k)
          self._ft_cal_z[i] = self.c_ForwardTagger.getFloat(str_to_char("z"), k)
          self._ft_cal_dx[i] = self.c_ForwardTagger.getFloat(str_to_char("dx"), k)
          self._ft_cal_dy[i] = self.c_ForwardTagger.getFloat(str_to_char("dy"), k)
          self._ft_cal_radius[i] = self.c_ForwardTagger.getFloat(str_to_char("radius"), k)
        elif pindex == i and detector == clas12_detector["FTHODO"]:
          self._ft_hodo_energy[i] = self.c_ForwardTagger.getFloat(str_to_char("energy"), k)
          self._ft_hodo_time[i] = self.c_ForwardTagger.getFloat(str_to_char("time"), k)
          self._ft_hodo_path[i] = self.c_ForwardTagger.getFloat(str_to_char("path"), k)
          self._ft_hodo_x[i] = self.c_ForwardTagger.getFloat(str_to_char("x"), k)
          self._ft_hodo_y[i] = self.c_ForwardTagger.getFloat(str_to_char("y"), k)
          self._ft_hodo_z[i] = self.c_ForwardTagger.getFloat(str_to_char("z"), k)
          self._ft_hodo_dx[i] = self.c_ForwardTagger.getFloat(str_to_char("dx"), k)
          self._ft_hodo_dy[i] = self.c_ForwardTagger.getFloat(str_to_char("dy"), k)
          self._ft_hodo_radius[i] = self.c_ForwardTagger.getFloat(str_to_char("radius"), k)

  cdef void load_trk(clas12Event self):
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Track.getRows()

    self._dc_sec = np.ones(len_pid, dtype=int) * (-1)
    self._dc_px = np.ones(len_pid, dtype=float) * NAN
    self._dc_py = np.ones(len_pid, dtype=float) * NAN
    self._dc_pz = np.ones(len_pid, dtype=float) * NAN
    self._dc_vx = np.ones(len_pid, dtype=float) * NAN
    self._dc_vy = np.ones(len_pid, dtype=float) * NAN
    self._dc_vz = np.ones(len_pid, dtype=float) * NAN

    self._cvt_px = np.ones(len_pid, dtype=float) * NAN
    self._cvt_py = np.ones(len_pid, dtype=float) * NAN
    self._cvt_pz = np.ones(len_pid, dtype=float) * NAN
    self._cvt_vx = np.ones(len_pid, dtype=float) * NAN
    self._cvt_vy = np.ones(len_pid, dtype=float) * NAN
    self._cvt_vz = np.ones(len_pid, dtype=float) * NAN

    cdef int i,k,pindex,detector,layer
    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Track.getShort(str_to_char("pindex"), k)
        detector = self.c_Track.getByte(str_to_char("detector"), k)

        if pindex == i and detector == clas12_detector["CVT"]:
          self._cvt_px[i] = self.c_Track.getFloat(str_to_char("px_nomm"), k)
          self._cvt_py[i] = self.c_Track.getFloat(str_to_char("py_nomm"), k)
          self._cvt_pz[i] = self.c_Track.getFloat(str_to_char("pz_nomm"), k)
          self._cvt_vx[i] = self.c_Track.getFloat(str_to_char("vx_nomm"), k)
          self._cvt_vy[i] = self.c_Track.getFloat(str_to_char("vy_nomm"), k)
          self._cvt_vz[i] = self.c_Track.getFloat(str_to_char("vz_nomm"), k)

        elif pindex == i and detector == clas12_detector["DC"]:
          self._dc_sec[i] = self.c_Track.getByte(str_to_char("sector"), k)
          self._dc_px[i] = self.c_Track.getFloat(str_to_char("px_nomm"), k)
          self._dc_py[i] = self.c_Track.getFloat(str_to_char("py_nomm"), k)
          self._dc_pz[i] = self.c_Track.getFloat(str_to_char("pz_nomm"), k)
          self._dc_vx[i] = self.c_Track.getFloat(str_to_char("vx_nomm"), k)
          self._dc_vy[i] = self.c_Track.getFloat(str_to_char("vy_nomm"), k)
          self._dc_vz[i] = self.c_Track.getFloat(str_to_char("vz_nomm"), k)

  cdef void load_cc(clas12Event self):
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Cherenkov.getRows()

    self._cc_nphe_tot = np.ones(len_pid, dtype=float) * NAN
    self._cc_ltcc_sec = np.ones(len_pid, dtype=int) * (-1)
    self._cc_ltcc_nphe = np.ones(len_pid, dtype=float) * NAN
    self._cc_ltcc_time = np.ones(len_pid, dtype=float) * NAN
    self._cc_ltcc_path = np.ones(len_pid, dtype=float) * NAN
    self._cc_ltcc_theta = np.ones(len_pid, dtype=float) * NAN
    self._cc_ltcc_phi = np.ones(len_pid, dtype=float) * NAN
    self._cc_htcc_sec = np.ones(len_pid, dtype=int) * (-1)
    self._cc_htcc_nphe = np.ones(len_pid, dtype=float) * NAN
    self._cc_htcc_time = np.ones(len_pid, dtype=float) * NAN
    self._cc_htcc_path = np.ones(len_pid, dtype=float) * NAN
    self._cc_htcc_theta = np.ones(len_pid, dtype=float) * NAN
    self._cc_htcc_phi = np.ones(len_pid, dtype=float) * NAN
    self._cc_rich_sec = np.ones(len_pid, dtype=int) * (-1)
    self._cc_rich_nphe = np.ones(len_pid, dtype=float) * NAN
    self._cc_rich_time = np.ones(len_pid, dtype=float) * NAN
    self._cc_rich_path = np.ones(len_pid, dtype=float) * NAN
    self._cc_rich_theta = np.ones(len_pid, dtype=float) * NAN
    self._cc_rich_phi = np.ones(len_pid, dtype=float) * NAN

    cdef float nphe_tot = 0.0;
    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Cherenkov.getShort(str_to_char("pindex"), k)
        detector = self.c_Cherenkov.getShort(str_to_char("detector"), k)

      if pindex == i and (detector == clas12_detector["HTCC"] or detector == clas12_detector["LTCC"] or detector == clas12_detector["RICH"]):
        nphe_tot += self.c_Cherenkov.getFloat(str_to_char("nphe"), k)

        if pindex == i and detector == clas12_detector["HTCC"]:
          self._cc_htcc_nphe[i] = self.c_Cherenkov.getFloat(str_to_char("nphe"), k)
          self._cc_htcc_sec[i] = self.c_Cherenkov.getByte(str_to_char("sector"), k)
          self._cc_htcc_time[i] = self.c_Cherenkov.getFloat(str_to_char("time"), k)
          self._cc_htcc_path[i] = self.c_Cherenkov.getFloat(str_to_char("path"), k)
          self._cc_htcc_theta[i] = self.c_Cherenkov.getFloat(str_to_char("theta"), k)
          self._cc_htcc_phi[i] = self.c_Cherenkov.getFloat(str_to_char("phi"), k)
        elif pindex == i and detector == clas12_detector["LTCC"]:
          self._cc_ltcc_nphe[i] = self.c_Cherenkov.getFloat(str_to_char("nphe"), k)
          self._cc_ltcc_sec[i] = self.c_Cherenkov.getByte(str_to_char("sector"), k)
          self._cc_ltcc_time[i] = self.c_Cherenkov.getFloat(str_to_char("time"), k)
          self._cc_ltcc_path[i] = self.c_Cherenkov.getFloat(str_to_char("path"), k)
          self._cc_ltcc_theta[i] = self.c_Cherenkov.getFloat(str_to_char("theta"), k)
          self._cc_ltcc_phi[i] = self.c_Cherenkov.getFloat(str_to_char("phi"), k)
        elif pindex == i and detector == clas12_detector["RICH"]:
          self._cc_rich_nphe[i] = self.c_Cherenkov.getFloat(str_to_char("nphe"), k)
          self._cc_rich_sec[i] = self.c_Cherenkov.getByte(str_to_char("sector"), k)
          self._cc_rich_time[i] = self.c_Cherenkov.getFloat(str_to_char("time"), k)
          self._cc_rich_path[i] = self.c_Cherenkov.getFloat(str_to_char("path"), k)
          self._cc_rich_theta[i] = self.c_Cherenkov.getFloat(str_to_char("theta"), k)
          self._cc_rich_phi[i] = self.c_Cherenkov.getFloat(str_to_char("phi"), k)

      self._cc_nphe_tot[i] = nphe_tot if nphe_tot != 0.0 else NAN

  cdef void load_sc(clas12Event self):
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Scintillator.getRows()

    self._sc_ftof_1a_sec = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_time = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_path = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_energy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_component = np.ones(len_pid, dtype=float) * (-1)
    self._sc_ftof_1a_x = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_y = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_z = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_hx = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_hy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1a_hz = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_sec = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_time = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_path = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_energy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_component = np.ones(len_pid, dtype=float) * (-1)
    self._sc_ftof_1b_x = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_y = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_z = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_hx = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_hy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_1b_hz = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_sec = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_time = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_path = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_energy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_component = np.ones(len_pid, dtype=float) * (-1)
    self._sc_ftof_2_x = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_y = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_z = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_hx = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_hy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ftof_2_hz = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_time = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_path = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_energy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_component = np.ones(len_pid, dtype=float) * (-1)
    self._sc_ctof_x = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_y = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_z = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_hx = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_hy = np.ones(len_pid, dtype=float) * NAN
    self._sc_ctof_hz = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_time = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_path = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_energy = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_component = np.ones(len_pid, dtype=float) * (-1)
    self._sc_cnd_x = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_y = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_z = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_hx = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_hy = np.ones(len_pid, dtype=float) * NAN
    self._sc_cnd_hz = np.ones(len_pid, dtype=float) * NAN

    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Scintillator.getShort(str_to_char("pindex"), k)
        detector = self.c_Scintillator.getByte(str_to_char("detector"), k)
        layer = self.c_Scintillator.getByte(str_to_char("layer"), k)
        if pindex == i and detector == clas12_detector["FTOF"] and layer == clas12_layer["FTOF_1A"]:
          self._sc_ftof_1a_sec[i] = self.c_Scintillator.getByte(str_to_char("sector"), k)
          self._sc_ftof_1a_time[i] = self.c_Scintillator.getFloat(str_to_char("time"), k)
          self._sc_ftof_1a_path[i] = self.c_Scintillator.getFloat(str_to_char("path"), k)
          self._sc_ftof_1a_energy[i] = self.c_Scintillator.getFloat(str_to_char("energy"), k)
          self._sc_ftof_1a_component[i] = self.c_Scintillator.getShort(str_to_char("component"), k)
          self._sc_ftof_1a_x[i] = self.c_Scintillator.getFloat(str_to_char("x"), k)
          self._sc_ftof_1a_y[i] = self.c_Scintillator.getFloat(str_to_char("y"), k)
          self._sc_ftof_1a_z[i] = self.c_Scintillator.getFloat(str_to_char("z"), k)
          self._sc_ftof_1a_hx[i] = self.c_Scintillator.getFloat(str_to_char("hx"), k)
          self._sc_ftof_1a_hy[i] = self.c_Scintillator.getFloat(str_to_char("hy"), k)
          self._sc_ftof_1a_hz[i] = self.c_Scintillator.getFloat(str_to_char("hz"), k)
        elif pindex == i and detector == clas12_detector["FTOF"] and layer == clas12_layer["FTOF_1B"]:
          self._sc_ftof_1b_sec[i] = self.c_Scintillator.getByte(str_to_char("sector"), k)
          self._sc_ftof_1b_time[i] = self.c_Scintillator.getFloat(str_to_char("time"), k)
          self._sc_ftof_1b_path[i] = self.c_Scintillator.getFloat(str_to_char("path"), k)
          self._sc_ftof_1b_energy[i] = self.c_Scintillator.getFloat(str_to_char("energy"), k)
          self._sc_ftof_1b_component[i] = self.c_Scintillator.getShort(str_to_char("component"), k)
          self._sc_ftof_1b_x[i] = self.c_Scintillator.getFloat(str_to_char("x"), k)
          self._sc_ftof_1b_y[i] = self.c_Scintillator.getFloat(str_to_char("y"), k)
          self._sc_ftof_1b_z[i] = self.c_Scintillator.getFloat(str_to_char("z"), k)
          self._sc_ftof_1b_hx[i] = self.c_Scintillator.getFloat(str_to_char("hx"), k)
          self._sc_ftof_1b_hy[i] = self.c_Scintillator.getFloat(str_to_char("hy"), k)
          self._sc_ftof_1b_hz[i] = self.c_Scintillator.getFloat(str_to_char("hz"), k)
        elif pindex == i and detector == clas12_detector["FTOF"] and layer == clas12_layer["FTOF_2"]:
          self._sc_ftof_2_sec[i] = self.c_Scintillator.getByte(str_to_char("sector"), k)
          self._sc_ftof_2_time[i] = self.c_Scintillator.getFloat(str_to_char("time"), k)
          self._sc_ftof_2_path[i] = self.c_Scintillator.getFloat(str_to_char("path"), k)
          self._sc_ftof_2_energy[i] = self.c_Scintillator.getFloat(str_to_char("energy"), k)
          self._sc_ftof_2_component[i] = self.c_Scintillator.getShort(str_to_char("component"), k)
          self._sc_ftof_2_x[i] = self.c_Scintillator.getFloat(str_to_char("x"), k)
          self._sc_ftof_2_y[i] = self.c_Scintillator.getFloat(str_to_char("y"), k)
          self._sc_ftof_2_z[i] = self.c_Scintillator.getFloat(str_to_char("z"), k)
          self._sc_ftof_2_hx[i] = self.c_Scintillator.getFloat(str_to_char("hx"), k)
          self._sc_ftof_2_hy[i] = self.c_Scintillator.getFloat(str_to_char("hy"), k)
          self._sc_ftof_2_hz[i] = self.c_Scintillator.getFloat(str_to_char("hz"), k)
        elif pindex == i and detector == clas12_detector["CTOF"]:
          self._sc_ctof_time[i] = self.c_Scintillator.getFloat(str_to_char("time"), k)
          self._sc_ctof_path[i] = self.c_Scintillator.getFloat(str_to_char("path"), k)
          self._sc_ctof_energy[i] = self.c_Scintillator.getFloat(str_to_char("energy"), k)
          self._sc_ctof_component[i] = self.c_Scintillator.getShort(str_to_char("component"), k)
          self._sc_ctof_x[i] = self.c_Scintillator.getFloat(str_to_char("x"), k)
          self._sc_ctof_y[i] = self.c_Scintillator.getFloat(str_to_char("y"), k)
          self._sc_ctof_z[i] = self.c_Scintillator.getFloat(str_to_char("z"), k)
          self._sc_ctof_hx[i] = self.c_Scintillator.getFloat(str_to_char("hx"), k)
          self._sc_ctof_hy[i] = self.c_Scintillator.getFloat(str_to_char("hy"), k)
          self._sc_ctof_hz[i] = self.c_Scintillator.getFloat(str_to_char("hz"), k)
        elif pindex == i and detector == clas12_detector["CND"]:
          self._sc_cnd_time[i] = self.c_Scintillator.getFloat(str_to_char("time"), k)
          self._sc_cnd_path[i] = self.c_Scintillator.getFloat(str_to_char("path"), k)
          self._sc_cnd_energy[i] = self.c_Scintillator.getFloat(str_to_char("energy"), k)
          self._sc_cnd_component[i] = self.c_Scintillator.getShort(str_to_char("component"), k)
          self._sc_cnd_x[i] = self.c_Scintillator.getFloat(str_to_char("x"), k)
          self._sc_cnd_y[i] = self.c_Scintillator.getFloat(str_to_char("y"), k)
          self._sc_cnd_z[i] = self.c_Scintillator.getFloat(str_to_char("z"), k)
          self._sc_cnd_hx[i] = self.c_Scintillator.getFloat(str_to_char("hx"), k)
          self._sc_cnd_hy[i] = self.c_Scintillator.getFloat(str_to_char("hy"), k)
          self._sc_cnd_hz[i] = self.c_Scintillator.getFloat(str_to_char("hz"), k)

  cdef void load_cal(clas12Event self):
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Calorimeter.getRows()
    self._ec_tot_energy = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_energy = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_sec = np.ones(len_pid, dtype=int) * (-1)
    self._ec_pcal_time = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_path = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_x = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_y = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_z = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_lu = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_lv = np.ones(len_pid, dtype=float) * NAN
    self._ec_pcal_lw = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_energy = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_sec = np.ones(len_pid, dtype=int) * (-1)
    self._ec_ecin_time = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_path = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_x = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_y = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_z = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_lu = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_lv = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecin_lw = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_energy = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_sec = np.ones(len_pid, dtype=int) * (-1)
    self._ec_ecout_time = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_path = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_x = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_y = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_z = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_lu = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_lv = np.ones(len_pid, dtype=float) * NAN
    self._ec_ecout_lw = np.ones(len_pid, dtype=float) * NAN
    cdef float pcal = 0.0
    cdef float einner = 0.0
    cdef float eouter = 0.0
    cdef float etot = 0.0
    cdef int i,k,pindex,detector,layer
    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Calorimeter.getShort(str_to_char("pindex"), k)
        detector = self.c_Calorimeter.getByte(str_to_char("detector"), k)
        layer = self.c_Calorimeter.getByte(str_to_char("layer"), k)
        energy = self.c_Calorimeter.getFloat(str_to_char("energy"), k)

        if pindex == i and detector == clas12_detector["ECAL"]:
          etot += energy;
          if layer == clas12_layer["PCAL"]:
            pcal += energy
            self._ec_pcal_sec[i] = self.c_Calorimeter.getByte(str_to_char("sector"), k)
            self._ec_pcal_time[i] = self.c_Calorimeter.getFloat(str_to_char("time"), k)
            self._ec_pcal_path[i] = self.c_Calorimeter.getFloat(str_to_char("path"), k)
            self._ec_pcal_x[i] = self.c_Calorimeter.getFloat(str_to_char("x"), k)
            self._ec_pcal_y[i] = self.c_Calorimeter.getFloat(str_to_char("y"), k)
            self._ec_pcal_z[i] = self.c_Calorimeter.getFloat(str_to_char("z"), k)
            self._ec_pcal_lu[i] = self.c_Calorimeter.getFloat(str_to_char("lu"), k)
            self._ec_pcal_lv[i] = self.c_Calorimeter.getFloat(str_to_char("lv"), k)
            self._ec_pcal_lw[i] = self.c_Calorimeter.getFloat(str_to_char("lw"), k)
          elif layer == clas12_layer["EC_INNER"]:
            einner += energy
            self._ec_ecin_sec[i] = self.c_Calorimeter.getByte(str_to_char("sector"), k)
            self._ec_ecin_time[i] = self.c_Calorimeter.getFloat(str_to_char("time"), k)
            self._ec_ecin_path[i] = self.c_Calorimeter.getFloat(str_to_char("path"), k)
            self._ec_ecin_x[i] = self.c_Calorimeter.getFloat(str_to_char("x"), k)
            self._ec_ecin_y[i] = self.c_Calorimeter.getFloat(str_to_char("y"), k)
            self._ec_ecin_z[i] = self.c_Calorimeter.getFloat(str_to_char("z"), k)
            self._ec_ecin_lu[i] = self.c_Calorimeter.getFloat(str_to_char("lu"), k)
            self._ec_ecin_lv[i] = self.c_Calorimeter.getFloat(str_to_char("lv"), k)
            self._ec_ecin_lw[i] = self.c_Calorimeter.getFloat(str_to_char("lw"), k)
          elif layer == clas12_layer["EC_OUTER"]:
            eouter += energy
            self._ec_ecout_sec[i] = self.c_Calorimeter.getByte(str_to_char("sector"), k)
            self._ec_ecout_time[i] = self.c_Calorimeter.getFloat(str_to_char("time"), k)
            self._ec_ecout_path[i] = self.c_Calorimeter.getFloat(str_to_char("path"), k)
            self._ec_ecout_x[i] = self.c_Calorimeter.getFloat(str_to_char("x"), k)
            self._ec_ecout_y[i] = self.c_Calorimeter.getFloat(str_to_char("y"), k)
            self._ec_ecout_z[i] = self.c_Calorimeter.getFloat(str_to_char("z"), k)
            self._ec_ecout_lu[i] = self.c_Calorimeter.getFloat(str_to_char("lu"), k)
            self._ec_ecout_lv[i] = self.c_Calorimeter.getFloat(str_to_char("lv"), k)
            self._ec_ecout_lw[i] = self.c_Calorimeter.getFloat(str_to_char("lw"), k)


      self._ec_pcal_energy[i] = pcal if pcal != 0.0 else NAN
      self._ec_ecin_energy[i] = einner if einner != 0.0 else NAN
      self._ec_ecout_energy[i] = eouter if eouter != 0.0 else NAN
      self._ec_tot_energy[i] = etot if etot != 0.0 else NAN


  def ec_tot_energy(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_tot_energy[i]
  def ec_pcal_energy(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_energy[i]
  def ec_pcal_sec(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_pcal_sec[i]
  def ec_pcal_time(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_time[i]
  def ec_pcal_path(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_path[i]
  def ec_pcal_x(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_x[i]
  def ec_pcal_y(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_y[i]
  def ec_pcal_z(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_z[i]
  def ec_pcal_lu(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_lu[i]
  def ec_pcal_lv(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_lv[i]
  def ec_pcal_lw(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_lw[i]
  def ec_ecin_energy(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_energy[i]
  def ec_ecin_sec(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_ecin_sec[i]
  def ec_ecin_time(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_time[i]
  def ec_ecin_path(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_path[i]
  def ec_ecin_x(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_x[i]
  def ec_ecin_y(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_y[i]
  def ec_ecin_z(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_z[i]
  def ec_ecin_lu(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_lu[i]
  def ec_ecin_lv(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_lv[i]
  def ec_ecin_lw(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_lw[i]
  def ec_ecout_energy(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_energy[i]
  def ec_ecout_sec(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_ecout_sec[i]
  def ec_ecout_time(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_time[i]
  def ec_ecout_path(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_path[i]
  def ec_ecout_x(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_x[i]
  def ec_ecout_y(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_y[i]
  def ec_ecout_z(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_z[i]
  def ec_ecout_lu(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_lu[i]
  def ec_ecout_lv(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_lv[i]
  def ec_ecout_lw(clas12Event self, int i):
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_lw[i]

  def sc_ftof_1a_sec(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1a_sec[i]
  def sc_ftof_1a_time(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_time[i]
  def sc_ftof_1a_path(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_path[i]
  def sc_ftof_1a_energy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_energy[i]
  def sc_ftof_1a_component(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1a_component[i]
  def sc_ftof_1a_x(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_x[i]
  def sc_ftof_1a_y(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_y[i]
  def sc_ftof_1a_z(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_z[i]
  def sc_ftof_1a_hx(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hx[i]
  def sc_ftof_1a_hy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hy[i]
  def sc_ftof_1a_hz(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hz[i]
  def sc_ftof_1b_sec(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1b_sec[i]
  def sc_ftof_1b_time(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_time[i]
  def sc_ftof_1b_path(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_path[i]
  def sc_ftof_1b_energy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_energy[i]
  def sc_ftof_1b_component(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1b_component[i]
  def sc_ftof_1b_x(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_x[i]
  def sc_ftof_1b_y(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_y[i]
  def sc_ftof_1b_z(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_z[i]
  def sc_ftof_1b_hx(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hx[i]
  def sc_ftof_1b_hy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hy[i]
  def sc_ftof_1b_hz(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hz[i]
  def sc_ftof_2_sec(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_2_sec[i]
  def sc_ftof_2_time(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_time[i]
  def sc_ftof_2_path(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_path[i]
  def sc_ftof_2_energy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_energy[i]
  def sc_ftof_2_component(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_2_component[i]
  def sc_ftof_2_x(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_x[i]
  def sc_ftof_2_y(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_y[i]
  def sc_ftof_2_z(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_z[i]
  def sc_ftof_2_hx(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hx[i]
  def sc_ftof_2_hy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hy[i]
  def sc_ftof_2_hz(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hz[i]
  def sc_ctof_time(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_time[i]
  def sc_ctof_path(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_path[i]
  def sc_ctof_energy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_energy[i]
  def sc_ctof_component(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ctof_component[i]
  def sc_ctof_x(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_x[i]
  def sc_ctof_y(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_y[i]
  def sc_ctof_z(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_z[i]
  def sc_ctof_hx(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hx[i]
  def sc_ctof_hy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hy[i]
  def sc_ctof_hz(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hz[i]
  def sc_cnd_time(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_time[i]
  def sc_cnd_path(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_path[i]
  def sc_cnd_energy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_energy[i]
  def sc_cnd_component(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_cnd_component[i]
  def sc_cnd_x(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_x[i]
  def sc_cnd_y(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_y[i]
  def sc_cnd_z(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_z[i]
  def sc_cnd_hx(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hx[i]
  def sc_cnd_hy(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hy[i]
  def sc_cnd_hz(clas12Event self, int i):
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hz[i]

  def cc_nphe_tot(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_nphe_tot[i]
  def cc_ltcc_sec(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_sec[i]
  def cc_ltcc_nphe(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_ltcc_nphe[i]
  def cc_ltcc_time(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_time[i]
  def cc_ltcc_path(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_path[i]
  def cc_ltcc_theta(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_theta[i]
  def cc_ltcc_phi(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_phi[i]
  def cc_htcc_sec(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_sec[i]
  def cc_htcc_nphe(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_htcc_nphe[i]
  def cc_htcc_time(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_time[i]
  def cc_htcc_path(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_path[i]
  def cc_htcc_theta(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_theta[i]
  def cc_htcc_phi(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_phi[i]
  def cc_rich_sec(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_sec[i]
  def cc_rich_nphe(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_rich_nphe[i]
  def cc_rich_time(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_time[i]
  def cc_rich_path(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_path[i]
  def cc_rich_theta(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_theta[i]
  def cc_rich_phi(clas12Event self, int i):
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_phi[i]

  def ft_cal_energy(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_energy[i]
  def ft_cal_time(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_time[i]
  def ft_cal_path(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_path[i]
  def ft_cal_x(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_x[i]
  def ft_cal_y(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_y[i]
  def ft_cal_z(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_z[i]
  def ft_cal_dx(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_dx[i]
  def ft_cal_dy(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_dy[i]
  def ft_cal_radius(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_radius[i]
  def ft_hodo_energy(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_energy[i]
  def ft_hodo_time(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_time[i]
  def ft_hodo_path(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_path[i]
  def ft_hodo_x(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_x[i]
  def ft_hodo_y(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_y[i]
  def ft_hodo_z(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_z[i]
  def ft_hodo_dx(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_dx[i]
  def ft_hodo_dy(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_dy[i]
  def ft_hodo_radius(clas12Event self, int i):
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_radius[i]

  def dc_sec(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return -1
    return self._dc_sec[i]
  def dc_px(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_px[i]
  def dc_py(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_py[i]
  def dc_pz(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_pz[i]
  def dc_vx(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vx[i]
  def dc_vy(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vy[i]
  def dc_vz(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vz[i]
  def cvt_px(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_px[i]
  def cvt_py(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_py[i]
  def cvt_pz(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_pz[i]
  def cvt_vx(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vx[i]
  def cvt_vy(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vy[i]
  def cvt_vz(clas12Event self, int i):
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vz[i]
