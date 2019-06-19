# cython: profile=True
# distutils: language = c++
# cython: boundscheck=False

cimport cython

cimport cpython
cimport libc.stdlib
from libcpp.memory cimport unique_ptr
from cython.operator cimport dereference as deref

from cython.view cimport array
from libcpp.string cimport string
from libcpp cimport bool
from libcpp.map cimport map
from libc.stdlib cimport free
from libc.math cimport abs, nan

cdef double NAN = nan("NAN")

import numpy as np
cimport numpy as np

cdef dict det = {1:"FT", 2:"FD", 4:"CD"}

cdef dict clas12_detector = {"BMT":  1, "BST":  2, "CND":  3, "CTOF":  4,
                              "CVT":  5, "DC":  6, "ECAL":  7, "FMT":  8,
                              "FT":  9, "FTCAL": 10, "FTHODO": 11, "FTOF": 12,
                              "FTTRK": 13, "HTCC": 15, "LTCC": 16, "RF": 17,
                              "RICH": 18, "RTPC": 19, "HEL": 20, "BAND": 21}


cdef dict clas12_layer = {"FTOF_1A": 1,"FTOF_1B": 2,"FTOF_2": 3,"PCAL": 1,"EC_INNER": 4,"EC_OUTER": 7}

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
      int    getInt(int, int)
      int    getShort(char*, int)
      int    getShort(int, int)
      int    getByte(char*, int)
      int    getByte(int, int)
      float  getFloat(char*, int)
      float  getFloat(int, int)
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

@cython.boundscheck(False)
@cython.wraparound(False)
@cython.infer_types(True)
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
    float [:] _ec_tot_energy
    float [:] _ec_pcal_energy
    int [:] _ec_pcal_sec
    float [:] _ec_pcal_time
    float [:] _ec_pcal_path
    float [:] _ec_pcal_x
    float [:] _ec_pcal_y
    float [:] _ec_pcal_z
    float [:] _ec_ecin_energy
    int [:] _ec_ecin_sec
    float [:] _ec_ecin_time
    float [:] _ec_ecin_path
    float [:] _ec_ecin_x
    float [:] _ec_ecin_y
    float [:] _ec_ecin_z
    float [:] _ec_ecout_energy
    int [:] _ec_ecout_sec
    float [:] _ec_ecout_time
    float [:] _ec_ecout_path
    float [:] _ec_ecout_x
    float [:] _ec_ecout_y
    float [:] _ec_ecout_z

    # SC
    int [:] _sc_ftof_1a_sec
    float [:] _sc_ftof_1a_time
    float [:] _sc_ftof_1a_path
    float [:] _sc_ftof_1a_energy
    float [:] _sc_ftof_1a_component
    float [:] _sc_ftof_1a_x
    float [:] _sc_ftof_1a_y
    float [:] _sc_ftof_1a_z
    float [:] _sc_ftof_1a_hx
    float [:] _sc_ftof_1a_hy
    float [:] _sc_ftof_1a_hz
    int [:] _sc_ftof_1b_sec
    float [:] _sc_ftof_1b_time
    float [:] _sc_ftof_1b_path
    float [:] _sc_ftof_1b_energy
    float [:] _sc_ftof_1b_component
    float [:] _sc_ftof_1b_x
    float [:] _sc_ftof_1b_y
    float [:] _sc_ftof_1b_z
    float [:] _sc_ftof_1b_hx
    float [:] _sc_ftof_1b_hy
    float [:] _sc_ftof_1b_hz
    int [:] _sc_ftof_2_sec
    float [:] _sc_ftof_2_time
    float [:] _sc_ftof_2_path
    float [:] _sc_ftof_2_energy
    float [:] _sc_ftof_2_component
    float [:] _sc_ftof_2_x
    float [:] _sc_ftof_2_y
    float [:] _sc_ftof_2_z
    float [:] _sc_ftof_2_hx
    float [:] _sc_ftof_2_hy
    float [:] _sc_ftof_2_hz
    float [:] _sc_ctof_time
    float [:] _sc_ctof_path
    float [:] _sc_ctof_energy
    float [:] _sc_ctof_component
    float [:] _sc_ctof_x
    float [:] _sc_ctof_y
    float [:] _sc_ctof_z
    float [:] _sc_ctof_hx
    float [:] _sc_ctof_hy
    float [:] _sc_ctof_hz
    float [:] _sc_cnd_time
    float [:] _sc_cnd_path
    float [:] _sc_cnd_energy
    float [:] _sc_cnd_component
    float [:] _sc_cnd_x
    float [:] _sc_cnd_y
    float [:] _sc_cnd_z
    float [:] _sc_cnd_hx
    float [:] _sc_cnd_hy
    float [:] _sc_cnd_hz

    # CC
    float [:] _cc_nphe_tot
    int [:] _cc_ltcc_sec
    float [:] _cc_ltcc_nphe
    float [:] _cc_ltcc_time
    float [:] _cc_ltcc_path
    int [:] _cc_htcc_sec
    float [:] _cc_htcc_nphe
    float [:] _cc_htcc_time
    float [:] _cc_htcc_path
    int [:] _cc_rich_sec
    float [:] _cc_rich_nphe
    float [:] _cc_rich_time
    float [:] _cc_rich_path
    float [:] _cc_htcc_x
    float [:] _cc_htcc_y
    float [:] _cc_htcc_z
    float [:] _cc_ltcc_x
    float [:] _cc_ltcc_y
    float [:] _cc_ltcc_z
    float [:] _cc_rich_x
    float [:] _cc_rich_y
    float [:] _cc_rich_z

    # FT
    float [:] _ft_cal_energy
    float [:] _ft_cal_time
    float [:] _ft_cal_path
    float [:] _ft_cal_x
    float [:] _ft_cal_y
    float [:] _ft_cal_z
    float [:] _ft_cal_dx
    float [:] _ft_cal_dy
    float [:] _ft_cal_radius
    float [:] _ft_hodo_energy
    float [:] _ft_hodo_time
    float [:] _ft_hodo_path
    float [:] _ft_hodo_x
    float [:] _ft_hodo_y
    float [:] _ft_hodo_z
    float [:] _ft_hodo_dx
    float [:] _ft_hodo_dy
    float [:] _ft_hodo_radius

    # Trk
    int [:] _dc_sec
    float [:] _dc_px
    float [:] _dc_py
    float [:] _dc_pz
    float [:] _dc_vx
    float [:] _dc_vy
    float [:] _dc_vz

    float [:] _cvt_px
    float [:] _cvt_py
    float [:] _cvt_pz
    float [:] _cvt_vx
    float [:] _cvt_vy
    float [:] _cvt_vz

    ## Add load variales
    bool ft_loaded
    bool trk_loaded
    bool cc_loaded
    bool sc_loaded
    bool cal_loaded


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
      self.c_hipo_event.getStructure(self.c_Track[0])
      self.c_hipo_event.getStructure(self.c_Cherenkov[0])
      self.c_hipo_event.getStructure(self.c_Scintillator[0])
      self.c_hipo_event.getStructure(self.c_Calorimeter[0])
      self.ft_loaded = False
      self.trk_loaded = False
      self.cc_loaded = False
      self.sc_loaded = False
      self.cal_loaded = False
      return self
    else:
      raise StopIteration
  def numEvents(clas12Event self):
    return self.c_hiporeader.numEvents()
  def __len__(clas12Event self):
    return self.c_Particle.getRows()
  def pid(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return -9999
    return self.c_Particle.getInt(0,i)
  def px(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(1,i)
  def py(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(2,i)
  def pz(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(3,i)
  def vx(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(4,i)
  def vy(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(5,i)
  def vz(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(6,i)
  def charge(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return -9999
    return self.c_Particle.getByte(7, i)
  def beta(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(8, i)
  def chi2pid(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return NAN
    return self.c_Particle.getFloat(9, i)
  def status(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return -1
    return self.c_Particle.getShort(10, i)
  def detector_system(clas12Event self,unsigned int i):
    if i >= self.c_Particle.getRows():
      return "unknown"
    return det[abs(int(self.c_Particle.getShort(10, i)/1000))]

  cdef bool load_ft(clas12Event self):
    if self.ft_loaded:
      return True
    self.ft_loaded = True
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_ForwardTagger.getRows()

    self._ft_cal_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_dx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_dy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_cal_radius = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    self._ft_hodo_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_dx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_dy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ft_hodo_radius = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_ForwardTagger.getShort(1, k)
        detector = self.c_ForwardTagger.getByte(2, k)

        if pindex == i and detector == 10:
          self._ft_cal_energy[i] = self.c_ForwardTagger.getFloat(3, k)
          self._ft_cal_time[i] = self.c_ForwardTagger.getFloat(4, k)
          self._ft_cal_path[i] = self.c_ForwardTagger.getFloat(5, k)
          self._ft_cal_x[i] = self.c_ForwardTagger.getFloat(7, k)
          self._ft_cal_y[i] = self.c_ForwardTagger.getFloat(8, k)
          self._ft_cal_z[i] = self.c_ForwardTagger.getFloat(9, k)
          self._ft_cal_dx[i] = self.c_ForwardTagger.getFloat(10, k)
          self._ft_cal_dy[i] = self.c_ForwardTagger.getFloat(11, k)
          self._ft_cal_radius[i] = self.c_ForwardTagger.getFloat(12, k)
        elif pindex == i and detector == 11:
          self._ft_hodo_energy[i] = self.c_ForwardTagger.getFloat(3, k)
          self._ft_hodo_time[i] = self.c_ForwardTagger.getFloat(4, k)
          self._ft_hodo_path[i] = self.c_ForwardTagger.getFloat(5, k)
          self._ft_hodo_x[i] = self.c_ForwardTagger.getFloat(7, k)
          self._ft_hodo_y[i] = self.c_ForwardTagger.getFloat(8, k)
          self._ft_hodo_z[i] = self.c_ForwardTagger.getFloat(9, k)
          self._ft_hodo_dx[i] = self.c_ForwardTagger.getFloat(10, k)
          self._ft_hodo_dy[i] = self.c_ForwardTagger.getFloat(11, k)
          self._ft_hodo_radius[i] = self.c_ForwardTagger.getFloat(12, k)

  cdef bool load_trk(clas12Event self):
    if self.trk_loaded:
      return True
    self.trk_loaded = True
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Track.getRows()

    self._dc_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._dc_px = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._dc_py = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._dc_pz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._dc_vx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._dc_vy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._dc_vz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    self._cvt_px = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cvt_py = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cvt_pz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cvt_vx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cvt_vy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cvt_vz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    cdef int i,k,pindex,detector,layer
    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Track.getShort(1, k)
        detector = self.c_Track.getByte(2, k)

        if pindex == i and detector == 5:
          self._cvt_px[i] = self.c_Track.getFloat(str_to_char("px_nomm"), k)
          self._cvt_py[i] = self.c_Track.getFloat(str_to_char("py_nomm"), k)
          self._cvt_pz[i] = self.c_Track.getFloat(str_to_char("pz_nomm"), k)
          self._cvt_vx[i] = self.c_Track.getFloat(str_to_char("vx_nomm"), k)
          self._cvt_vy[i] = self.c_Track.getFloat(str_to_char("vy_nomm"), k)
          self._cvt_vz[i] = self.c_Track.getFloat(str_to_char("vz_nomm"), k)

        elif pindex == i and detector == 6:
          self._dc_sec[i] = self.c_Track.getByte(str_to_char("sector"), k)
          self._dc_px[i] = self.c_Track.getFloat(str_to_char("px_nomm"), k)
          self._dc_py[i] = self.c_Track.getFloat(str_to_char("py_nomm"), k)
          self._dc_pz[i] = self.c_Track.getFloat(str_to_char("pz_nomm"), k)
          self._dc_vx[i] = self.c_Track.getFloat(str_to_char("vx_nomm"), k)
          self._dc_vy[i] = self.c_Track.getFloat(str_to_char("vy_nomm"), k)
          self._dc_vz[i] = self.c_Track.getFloat(str_to_char("vz_nomm"), k)

  cdef bool load_cc(clas12Event self):
    if self.cc_loaded:
      return True
    self.cc_loaded = True
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Cherenkov.getRows()

    self._cc_nphe_tot = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_ltcc_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._cc_ltcc_nphe = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_ltcc_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_ltcc_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_ltcc_theta = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_ltcc_phi = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_htcc_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._cc_htcc_nphe = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_htcc_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_htcc_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_htcc_theta = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_htcc_phi = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_rich_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._cc_rich_nphe = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_rich_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_rich_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_rich_theta = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._cc_rich_phi = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    cdef float nphe_tot = 0.0;
    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Cherenkov.getShort(1, k)
        detector = self.c_Cherenkov.getShort(2, k)

      if pindex == i and (detector == 15 or detector == 16 or detector == 18):
        nphe_tot += self.c_Cherenkov.getFloat(4, k)

        if pindex == i and detector == 15:
          self._cc_htcc_sec[i] = self.c_Cherenkov.getByte(3, k)
          self._cc_htcc_nphe[i] = self.c_Cherenkov.getFloat(4, k)
          self._cc_htcc_time[i] = self.c_Cherenkov.getFloat(5, k)
          self._cc_htcc_path[i] = self.c_Cherenkov.getFloat(6, k)
          self._cc_htcc_x[i] = self.c_Cherenkov.getFloat(7, k)
          self._cc_htcc_y[i] = self.c_Cherenkov.getFloat(8, k)
          self._cc_htcc_z[i] = self.c_Cherenkov.getFloat(9, k)
        elif pindex == i and detector == 16:
          self._cc_ltcc_sec[i] = self.c_Cherenkov.getByte(3, k)
          self._cc_ltcc_nphe[i] = self.c_Cherenkov.getFloat(4, k)
          self._cc_ltcc_time[i] = self.c_Cherenkov.getFloat(5, k)
          self._cc_ltcc_path[i] = self.c_Cherenkov.getFloat(6, k)
          self._cc_ltcc_x[i] = self.c_Cherenkov.getFloat(7, k)
          self._cc_ltcc_y[i] = self.c_Cherenkov.getFloat(8, k)
          self._cc_ltcc_z[i] = self.c_Cherenkov.getFloat(9, k)
        elif pindex == i and detector == 18:
          self._cc_rich_sec[i] = self.c_Cherenkov.getByte(3, k)
          self._cc_rich_nphe[i] = self.c_Cherenkov.getFloat(4, k)
          self._cc_rich_time[i] = self.c_Cherenkov.getFloat(5, k)
          self._cc_rich_path[i] = self.c_Cherenkov.getFloat(6, k)
          self._cc_rich_x[i] = self.c_Cherenkov.getFloat(7, k)
          self._cc_rich_y[i] = self.c_Cherenkov.getFloat(8, k)
          self._cc_rich_z[i] = self.c_Cherenkov.getFloat(9, k)

      self._cc_nphe_tot[i] = nphe_tot if nphe_tot != 0.0 else NAN

  cdef bool load_sc(clas12Event self):
    if self.sc_loaded:
      return True
    self.sc_loaded=True
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Scintillator.getRows()

    self._sc_ftof_1a_sec = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_component = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._sc_ftof_1a_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_hx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_hy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1a_hz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_sec = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_component = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._sc_ftof_1b_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_hx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_hy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_1b_hz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_sec = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_component = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._sc_ftof_2_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_hx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_hy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ftof_2_hz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_component = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._sc_ctof_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_hx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_hy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_ctof_hz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_component = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._sc_cnd_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_hx = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_hy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._sc_cnd_hz = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Scintillator.getShort(1, k)
        detector = self.c_Scintillator.getByte(2, k)
        layer = self.c_Scintillator.getByte(4, k)
        if pindex == i and detector == 12 and layer == 1:
          self._sc_ftof_1a_sec[i] = self.c_Scintillator.getByte(3, k)
          self._sc_ftof_1a_component[i] = self.c_Scintillator.getShort(5, k)
          self._sc_ftof_1a_energy[i] = self.c_Scintillator.getFloat(6, k)
          self._sc_ftof_1a_time[i] = self.c_Scintillator.getFloat(7, k)
          self._sc_ftof_1a_path[i] = self.c_Scintillator.getFloat(8, k)
          self._sc_ftof_1a_x[i] = self.c_Scintillator.getFloat(10, k)
          self._sc_ftof_1a_y[i] = self.c_Scintillator.getFloat(11, k)
          self._sc_ftof_1a_z[i] = self.c_Scintillator.getFloat(12, k)
          self._sc_ftof_1a_hx[i] = self.c_Scintillator.getFloat(13, k)
          self._sc_ftof_1a_hy[i] = self.c_Scintillator.getFloat(14, k)
          self._sc_ftof_1a_hz[i] = self.c_Scintillator.getFloat(15, k)
        elif pindex == i and detector == 12 and layer == 2:
          self._sc_ftof_1b_sec[i] = self.c_Scintillator.getByte(3, k)
          self._sc_ftof_1b_component[i] = self.c_Scintillator.getShort(5, k)
          self._sc_ftof_1b_energy[i] = self.c_Scintillator.getFloat(6, k)
          self._sc_ftof_1b_time[i] = self.c_Scintillator.getFloat(7, k)
          self._sc_ftof_1b_path[i] = self.c_Scintillator.getFloat(8, k)
          self._sc_ftof_1b_x[i] = self.c_Scintillator.getFloat(10, k)
          self._sc_ftof_1b_y[i] = self.c_Scintillator.getFloat(11, k)
          self._sc_ftof_1b_z[i] = self.c_Scintillator.getFloat(12, k)
          self._sc_ftof_1b_hx[i] = self.c_Scintillator.getFloat(13, k)
          self._sc_ftof_1b_hy[i] = self.c_Scintillator.getFloat(14, k)
          self._sc_ftof_1b_hz[i] = self.c_Scintillator.getFloat(15, k)
        elif pindex == i and detector == 12 and layer == 3:
          self._sc_ftof_2_sec[i] = self.c_Scintillator.getByte(3, k)
          self._sc_ftof_2_component[i] = self.c_Scintillator.getShort(5, k)
          self._sc_ftof_2_energy[i] = self.c_Scintillator.getFloat(6, k)
          self._sc_ftof_2_time[i] = self.c_Scintillator.getFloat(7, k)
          self._sc_ftof_2_path[i] = self.c_Scintillator.getFloat(8, k)
          self._sc_ftof_2_x[i] = self.c_Scintillator.getFloat(10, k)
          self._sc_ftof_2_y[i] = self.c_Scintillator.getFloat(11, k)
          self._sc_ftof_2_z[i] = self.c_Scintillator.getFloat(12, k)
          self._sc_ftof_2_hx[i] = self.c_Scintillator.getFloat(13, k)
          self._sc_ftof_2_hy[i] = self.c_Scintillator.getFloat(14, k)
          self._sc_ftof_2_hz[i] = self.c_Scintillator.getFloat(15, k)
        elif pindex == i and detector == 4:
          self._sc_ctof_component[i] = self.c_Scintillator.getShort(5, k)
          self._sc_ctof_energy[i] = self.c_Scintillator.getFloat(6, k)
          self._sc_ctof_time[i] = self.c_Scintillator.getFloat(7, k)
          self._sc_ctof_path[i] = self.c_Scintillator.getFloat(8, k)
          self._sc_ctof_x[i] = self.c_Scintillator.getFloat(10, k)
          self._sc_ctof_y[i] = self.c_Scintillator.getFloat(11, k)
          self._sc_ctof_z[i] = self.c_Scintillator.getFloat(12, k)
          self._sc_ctof_hx[i] = self.c_Scintillator.getFloat(13, k)
          self._sc_ctof_hy[i] = self.c_Scintillator.getFloat(14, k)
          self._sc_ctof_hz[i] = self.c_Scintillator.getFloat(15, k)
        elif pindex == i and detector == 3:
          self._sc_cnd_component[i] = self.c_Scintillator.getShort(5, k)
          self._sc_cnd_energy[i] = self.c_Scintillator.getFloat(6, k)
          self._sc_cnd_time[i] = self.c_Scintillator.getFloat(7, k)
          self._sc_cnd_path[i] = self.c_Scintillator.getFloat(8, k)
          self._sc_cnd_x[i] = self.c_Scintillator.getFloat(10, k)
          self._sc_cnd_y[i] = self.c_Scintillator.getFloat(11, k)
          self._sc_cnd_z[i] = self.c_Scintillator.getFloat(12, k)
          self._sc_cnd_hx[i] = self.c_Scintillator.getFloat(13, k)
          self._sc_cnd_hy[i] = self.c_Scintillator.getFloat(14, k)
          self._sc_cnd_hz[i] = self.c_Scintillator.getFloat(15, k)

  cdef bool load_cal(clas12Event self):
    if self.cal_loaded:
      return True
    self.cal_loaded = True
    cdef int len_pid = self.c_Particle.getRows()
    cdef int len_pindex = self.c_Calorimeter.getRows()

    self._ec_tot_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._ec_pcal_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_pcal_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._ec_ecin_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecin_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_energy = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_sec = array(shape=(len_pid,), itemsize=sizeof(int), format="i")
    self._ec_ecout_time = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_path = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_x = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_y = array(shape=(len_pid,), itemsize=sizeof(float), format="f")
    self._ec_ecout_z = array(shape=(len_pid,), itemsize=sizeof(float), format="f")

    cdef float pcal = 0.0
    cdef float einner = 0.0
    cdef float eouter = 0.0
    cdef float etot = 0.0
    cdef int i,k,pindex,detector,layer

    for i in range(0,len_pid):
      for k in range(0,len_pindex):
        pindex = self.c_Calorimeter.getShort(1, k)
        detector = self.c_Calorimeter.getByte(2, k)
        layer = self.c_Calorimeter.getByte(4, k)
        energy = self.c_Calorimeter.getFloat(5, k)

        if pindex == i and detector == 7:
          etot += energy;
          if layer == 1:
            pcal += energy
            self._ec_pcal_sec[i] = self.c_Calorimeter.getByte(3, k)
            self._ec_pcal_time[i] = self.c_Calorimeter.getFloat(6, k)
            self._ec_pcal_path[i] = self.c_Calorimeter.getFloat(7, k)
            self._ec_pcal_x[i] = self.c_Calorimeter.getFloat(9, k)
            self._ec_pcal_y[i] = self.c_Calorimeter.getFloat(10, k)
            self._ec_pcal_z[i] = self.c_Calorimeter.getFloat(11, k)
          elif layer == 4:
            einner += energy
            self._ec_ecin_sec[i] = self.c_Calorimeter.getByte(3, k)
            self._ec_ecin_time[i] = self.c_Calorimeter.getFloat(6, k)
            self._ec_ecin_path[i] = self.c_Calorimeter.getFloat(7, k)
            self._ec_ecin_x[i] = self.c_Calorimeter.getFloat(9, k)
            self._ec_ecin_y[i] = self.c_Calorimeter.getFloat(10, k)
            self._ec_ecin_z[i] = self.c_Calorimeter.getFloat(11, k)
          elif layer == 7:
            eouter += energy
            self._ec_ecout_sec[i] = self.c_Calorimeter.getByte(3, k)
            self._ec_ecout_time[i] = self.c_Calorimeter.getFloat(6, k)
            self._ec_ecout_path[i] = self.c_Calorimeter.getFloat(7, k)
            self._ec_ecout_x[i] = self.c_Calorimeter.getFloat(9, k)
            self._ec_ecout_y[i] = self.c_Calorimeter.getFloat(10, k)
            self._ec_ecout_z[i] = self.c_Calorimeter.getFloat(11, k)


      self._ec_pcal_energy[i] = pcal if pcal != 0.0 else NAN
      self._ec_ecin_energy[i] = einner if einner != 0.0 else NAN
      self._ec_ecout_energy[i] = eouter if eouter != 0.0 else NAN
      self._ec_tot_energy[i] = etot if etot != 0.0 else NAN


  def ec_tot_energy(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_tot_energy[i]
  def ec_pcal_energy(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_energy[i]
  def ec_pcal_sec(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_pcal_sec[i]
  def ec_pcal_time(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_time[i]
  def ec_pcal_path(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_path[i]
  def ec_pcal_x(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_x[i]
  def ec_pcal_y(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_y[i]
  def ec_pcal_z(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_pcal_z[i]
  def ec_ecin_energy(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_energy[i]
  def ec_ecin_sec(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_ecin_sec[i]
  def ec_ecin_time(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_time[i]
  def ec_ecin_path(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_path[i]
  def ec_ecin_x(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_x[i]
  def ec_ecin_y(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_y[i]
  def ec_ecin_z(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecin_z[i]
  def ec_ecout_energy(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_energy[i]
  def ec_ecout_sec(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return -1
    return self._ec_ecout_sec[i]
  def ec_ecout_time(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_time[i]
  def ec_ecout_path(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_path[i]
  def ec_ecout_x(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_x[i]
  def ec_ecout_y(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_y[i]
  def ec_ecout_z(clas12Event self, int i):
    if not self.cal_loaded:
      self.load_cal()
    if i >= self.c_Calorimeter.getRows():
      return NAN
    return self._ec_ecout_z[i]

  def sc_ftof_1a_sec(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1a_sec[i]
  def sc_ftof_1a_time(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_time[i]
  def sc_ftof_1a_path(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_path[i]
  def sc_ftof_1a_energy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_energy[i]
  def sc_ftof_1a_component(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1a_component[i]
  def sc_ftof_1a_x(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_x[i]
  def sc_ftof_1a_y(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_y[i]
  def sc_ftof_1a_z(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_z[i]
  def sc_ftof_1a_hx(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hx[i]
  def sc_ftof_1a_hy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hy[i]
  def sc_ftof_1a_hz(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1a_hz[i]
  def sc_ftof_1b_sec(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1b_sec[i]
  def sc_ftof_1b_time(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_time[i]
  def sc_ftof_1b_path(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_path[i]
  def sc_ftof_1b_energy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_energy[i]
  def sc_ftof_1b_component(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_1b_component[i]
  def sc_ftof_1b_x(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_x[i]
  def sc_ftof_1b_y(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_y[i]
  def sc_ftof_1b_z(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_z[i]
  def sc_ftof_1b_hx(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hx[i]
  def sc_ftof_1b_hy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hy[i]
  def sc_ftof_1b_hz(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_1b_hz[i]
  def sc_ftof_2_sec(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_2_sec[i]
  def sc_ftof_2_time(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_time[i]
  def sc_ftof_2_path(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_path[i]
  def sc_ftof_2_energy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_energy[i]
  def sc_ftof_2_component(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ftof_2_component[i]
  def sc_ftof_2_x(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_x[i]
  def sc_ftof_2_y(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_y[i]
  def sc_ftof_2_z(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_z[i]
  def sc_ftof_2_hx(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hx[i]
  def sc_ftof_2_hy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hy[i]
  def sc_ftof_2_hz(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ftof_2_hz[i]
  def sc_ctof_time(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_time[i]
  def sc_ctof_path(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_path[i]
  def sc_ctof_energy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_energy[i]
  def sc_ctof_component(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_ctof_component[i]
  def sc_ctof_x(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_x[i]
  def sc_ctof_y(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_y[i]
  def sc_ctof_z(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_z[i]
  def sc_ctof_hx(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hx[i]
  def sc_ctof_hy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hy[i]
  def sc_ctof_hz(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_ctof_hz[i]
  def sc_cnd_time(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_time[i]
  def sc_cnd_path(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_path[i]
  def sc_cnd_energy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_energy[i]
  def sc_cnd_component(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return -1
    return self._sc_cnd_component[i]
  def sc_cnd_x(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_x[i]
  def sc_cnd_y(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_y[i]
  def sc_cnd_z(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_z[i]
  def sc_cnd_hx(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hx[i]
  def sc_cnd_hy(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hy[i]
  def sc_cnd_hz(clas12Event self, int i):
    if not self.sc_loaded:
      self.load_sc()
    if i >= self.c_Scintillator.getRows():
      return NAN
    return self._sc_cnd_hz[i]

  def cc_nphe_tot(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_nphe_tot[i]
  def cc_ltcc_sec(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_sec[i]
  def cc_ltcc_nphe(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_ltcc_nphe[i]
  def cc_ltcc_time(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_time[i]
  def cc_ltcc_path(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_path[i]
  def cc_ltcc_theta(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_theta[i]
  def cc_ltcc_phi(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_phi[i]
  def cc_htcc_sec(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_sec[i]
  def cc_htcc_nphe(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_htcc_nphe[i]
  def cc_htcc_time(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_time[i]
  def cc_htcc_path(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_path[i]
  def cc_htcc_theta(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_theta[i]
  def cc_htcc_phi(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_phi[i]
  def cc_rich_sec(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_sec[i]
  def cc_rich_nphe(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return -1
    return self._cc_rich_nphe[i]
  def cc_rich_time(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_time[i]
  def cc_rich_path(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc()
    if i >= self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_path[i]

  def cc_htcc_x(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_x[i]
  def cc_htcc_y(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_y[i]
  def cc_htcc_z(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_htcc_z[i]
  def cc_ltcc_x(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_x[i]
  def cc_ltcc_y(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_y[i]
  def cc_ltcc_z(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_ltcc_z[i]
  def cc_rich_x(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_x[i]
  def cc_rich_y(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_y[i]
  def cc_rich_z(clas12Event self, int i):
    if not self.cc_loaded:
      self.load_cc
    if i> self.c_Cherenkov.getRows():
      return NAN
    return self._cc_rich_z[i]

  def ft_cal_energy(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_energy[i]
  def ft_cal_time(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_time[i]
  def ft_cal_path(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_path[i]
  def ft_cal_x(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_x[i]
  def ft_cal_y(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_y[i]
  def ft_cal_z(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_z[i]
  def ft_cal_dx(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_dx[i]
  def ft_cal_dy(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_dy[i]
  def ft_cal_radius(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_cal_radius[i]
  def ft_hodo_energy(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_energy[i]
  def ft_hodo_time(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_time[i]
  def ft_hodo_path(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_path[i]
  def ft_hodo_x(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_x[i]
  def ft_hodo_y(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_y[i]
  def ft_hodo_z(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_z[i]
  def ft_hodo_dx(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_dx[i]
  def ft_hodo_dy(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_dy[i]
  def ft_hodo_radius(clas12Event self, int i):
    if not self.ft_loaded:
      self.load_ft()
    if i >= self.c_ForwardTagger.getRows():
      return NAN
    return self._ft_hodo_radius[i]

  def dc_sec(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return -1
    return self._dc_sec[i]
  def dc_px(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_px[i]
  def dc_py(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_py[i]
  def dc_pz(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_pz[i]
  def dc_vx(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vx[i]
  def dc_vy(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vy[i]
  def dc_vz(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._dc_vz[i]
  def cvt_px(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_px[i]
  def cvt_py(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_py[i]
  def cvt_pz(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_pz[i]
  def cvt_vx(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vx[i]
  def cvt_vy(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vy[i]
  def cvt_vz(clas12Event self, int i):
    if not self.trk_loaded:
      self.load_trk()
    if i >= self.c_Track.getRows():
      return NAN
    return self._cvt_vz[i]
