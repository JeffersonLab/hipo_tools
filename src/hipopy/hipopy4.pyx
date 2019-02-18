# cython: profile=True
# distutils: language = c++
# cython: boundscheck=False

cimport cython

from libcpp.string cimport string
from libcpp cimport bool
#from libcpp.map cimport map
#from libcpp.utility cimport pair
#from libcpp.vector cimport vector
#from cython.view cimport array as cvarray
#from libc.stdlib cimport free
#import numpy as np

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
      void read(event)
      dictionary* dictionary()
      void readDictionary(dictionary)
      void open(string)
      bool hasNext()
      bool next()


cdef char* str_to_char(str name):
  """Convert python string to char*"""
  cdef bytes name_bytes = name.encode()
  cdef char* c_name = name_bytes
  return c_name

cdef class Event:
  cdef:
    reader*c_hiporeader
    dictionary*c_dict
    event*c_event
    bank*c_ForwardTagger
    bank*c_VertDoca
    bank*c_Track
    bank*c_Cherenkov
    bank*c_Event
    bank*c_Particle
    bank*c_Scintillator
    bank*c_Calorimeter
    bank*c_CovMat
  def __cinit__(Event self, string filename):
    self.c_hiporeader = new reader(filename)
    self.c_dict = self.c_hiporeader.dictionary()
    self.c_event = new event()
    self.c_ForwardTagger = new bank(self.c_dict.getSchema(str_to_char("REC::ForwardTagger")))
    self.c_VertDoca = new bank(self.c_dict.getSchema(str_to_char("REC::VertDoca")))
    self.c_Track = new bank(self.c_dict.getSchema(str_to_char("REC::Track")))
    self.c_Cherenkov = new bank(self.c_dict.getSchema(str_to_char("REC::Cherenkov")))
    self.c_Event = new bank(self.c_dict.getSchema(str_to_char("REC::Event")))
    self.c_Particle = new bank(self.c_dict.getSchema(str_to_char("REC::Particle")))
    self.c_Scintillator = new bank(self.c_dict.getSchema(str_to_char("REC::Scintillator")))
    self.c_Calorimeter = new bank(self.c_dict.getSchema(str_to_char("REC::Calorimeter")))
    self.c_CovMat = new bank(self.c_dict.getSchema(str_to_char("REC::CovMat")))

  def __iter__(Event self):
      return self
  def __str__(Event self):
    out = ""
    out += self.c_dict.getSchema(str_to_char("REC::ForwardTagger")).json()
    out += self.c_dict.getSchema(str_to_char("REC::VertDoca")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Track")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Cherenkov")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Event")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Particle")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Scintillator")).json()
    out += self.c_dict.getSchema(str_to_char("REC::Calorimeter")).json()
    out += self.c_dict.getSchema(str_to_char("REC::CovMat")).json()
    return out
  def __next__(Event self):
    if self.c_hiporeader.next():
      self.c_hiporeader.read(self.c_event[0])
      self.c_event.getStructure(self.c_Particle[0])
      self.c_event.getStructure(self.c_ForwardTagger[0])
      self.c_event.getStructure(self.c_VertDoca[0])
      self.c_event.getStructure(self.c_Track[0])
      self.c_event.getStructure(self.c_Cherenkov[0])
      self.c_event.getStructure(self.c_Event[0])
      self.c_event.getStructure(self.c_Scintillator[0])
      self.c_event.getStructure(self.c_Calorimeter[0])
      self.c_event.getStructure(self.c_CovMat[0])
      return self
    else:
      raise StopIteration
  def __len__(Event self):
    return self.c_Particle.getRows()
  def pid(Event self, int i):
    return self.c_Particle.getInt(str_to_char("pid") ,i)
  def px(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("px") ,i)
  def py(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("py") ,i)
  def pz(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("pz") ,i)
  def vx(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("vx") ,i)
  def vy(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("vy") ,i)
  def vz(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("vz") ,i)
  def charge(Event self, int i):
    return self.c_Particle.getByte(str_to_char("charge"), i)
  def beta(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("beta"), i)
  def chi2pid(Event self, int i):
    return self.c_Particle.getFloat(str_to_char("chi2pid"), i)
  def status(Event self, int i):
    return self.c_Particle.getShort(str_to_char("status"), i)

  def event_len(Event self):
    return self.c_Event.getRows()
  @property
  def NRUN(Event self):
    return self.c_Event.getInt(str_to_char("NRUN"),0)
  @property
  def NEVENT(Event self):
    return self.c_Event.getInt(str_to_char("NEVENT"),0)
  @property
  def EVNTime(Event self):
    return self.c_Event.getFloat(str_to_char("EVNTime"),0)
  @property
  def TYPE(Event self):
    return self.c_Event.getByte(str_to_char("TYPE"),0)
  @property
  def EvCAT(Event self):
    return self.c_Event.getShort(str_to_char("EvCAT"),0)
  @property
  def NPGP(Event self):
    return self.c_Event.getShort(str_to_char("NPGP"),0)
  @property
  def TRG(Event self):
    return self.c_Event.getLong(str_to_char("TRG"),0)
  @property
  def BCG(Event self):
    return self.c_Event.getFloat(str_to_char("BCG"),0)
  @property
  def LT(Event self):
    return self.c_Event.getDouble(str_to_char("LT"),0)
  @property
  def STTime(Event self):
    return self.c_Event.getFloat(str_to_char("STTime"),0)
  @property
  def RFTime(Event self):
    return self.c_Event.getFloat(str_to_char("RFTime"),0)
  @property
  def Helic(Event self):
    return self.c_Event.getByte(str_to_char("Helic"),0)
  @property
  def PTIME(Event self):
    return self.c_Event.getFloat(str_to_char("PTIME"),0)

  def ft_len(Event self):
    return self.c_ForwardTagger.getRows()
  def ft_pindex(Event self, int i):
    return self.c_ForwardTagger.getShort(str_to_char("pindex"), i)
  def ft_detector(Event self, int i):
    return self.c_ForwardTagger.getByte(str_to_char("detector"), i)
  def ft_energy(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("energy"), i)
  def ft_time(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("time"), i)
  def ft_path(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("path"), i)
  def ft_chi2(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("chi2"), i)
  def ft_x(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("x"), i)
  def ft_y(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("y"), i)
  def ft_z(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("z"), i)
  def ft_dx(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("dx"), i)
  def ft_dy(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("dy"), i)
  def ft_radius(Event self, int i):
    return self.c_ForwardTagger.getFloat(str_to_char("radius"), i)
  def ft_size(Event self, int i):
    return self.c_ForwardTagger.getShort(str_to_char("size"), i)
  def ft_status(Event self, int i):
    return self.c_ForwardTagger.getShort(str_to_char("status"), i)

  def vd_len(Event self):
    return self.c_VertDoca.getRows()
  def vd_index1(Event self, int i):
    return self.c_VertDoca.getShort(str_to_char("index1"),i)
  def vd_index2(Event self, int i):
    return self.c_VertDoca.getShort(str_to_char("index2"),i)
  def vd_x(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("x"),i)
  def vd_y(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("y"),i)
  def vd_z(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("z"),i)
  def vd_x1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("x1"),i)
  def vd_y1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("y1"),i)
  def vd_z1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("z1"),i)
  def vd_cx1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cx1"),i)
  def vd_cy1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cy1"),i)
  def vd_cz1(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cz1"),i)
  def vd_x2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("x2"),i)
  def vd_y2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("y2"),i)
  def vd_z2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("z2"),i)
  def vd_cx2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cx2"),i)
  def vd_cy2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cy2"),i)
  def vd_cz2(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("cz2"),i)
  def vd_r(Event self, int i):
    return self.c_VertDoca.getFloat(str_to_char("r"),i)

  def trk_len(Event self):
      return self.c_Track.getRows()
  def trk_index(Event self, int i):
    return self.c_Track.getShort(str_to_char("index"),i)
  def trk_pindex(Event self, int i):
    return self.c_Track.getShort(str_to_char("pindex"),i)
  def trk_detector(Event self, int i):
    return self.c_Track.getByte(str_to_char("detector"),i)
  def trk_sector(Event self, int i):
    return self.c_Track.getByte(str_to_char("sector"),i)
  def trk_status(Event self, int i):
    return self.c_Track.getShort(str_to_char("status"),i)
  def trk_q(Event self, int i):
    return self.c_Track.getByte(str_to_char("q"),i)
  def trk_chi2(Event self, int i):
    return self.c_Track.getFloat(str_to_char("chi2"),i)
  def trk_NDF(Event self, int i):
    return self.c_Track.getShort(str_to_char("NDF"),i)
  def trk_px_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("px_nomm"),i)
  def trk_py_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("py_nomm"),i)
  def trk_pz_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("pz_nomm"),i)
  def trk_vx_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("vx_nomm"),i)
  def trk_vy_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("vy_nomm"),i)
  def trk_vz_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("vz_nomm"),i)
  def trk_chi2_nomm(Event self, int i):
    return self.c_Track.getFloat(str_to_char("chi2_nomm"),i)
  def trk_NDF_nomm(Event self, int i):
    return self.c_Track.getShort(str_to_char("NDF_nomm"),i)

  def chern_len(Event self):
      return self.c_Cherenkov.getRows()
  def chern_index(Event self, int i):
    return self.c_Cherenkov.getShort(str_to_char("index"),i)
  def chern_pindex(Event self, int i):
    return self.c_Cherenkov.getShort(str_to_char("pindex"),i)
  def chern_detector(Event self, int i):
    return self.c_Cherenkov.getByte(str_to_char("detector"),i)
  def chern_sector(Event self, int i):
    return self.c_Cherenkov.getByte(str_to_char("sector"),i)
  def chern_nphe(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("nphe"),i)
  def chern_time(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("time"),i)
  def chern_path(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("path"),i)
  def chern_chi2(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("chi2"),i)
  def chern_x(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("x"),i)
  def chern_y(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("y"),i)
  def chern_z(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("z"),i)
  def chern_theta(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("theta"),i)
  def chern_phi(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("phi"),i)
  def chern_dtheta(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("dtheta"),i)
  def chern_dphi(Event self, int i):
    return self.c_Cherenkov.getFloat(str_to_char("dphi"),i)
  def chern_status(Event self, int i):
    return self.c_Cherenkov.getShort(str_to_char("status"),i)

  def sc_len(Event self):
    return self.c_Scintillator.getRows()
  def sc_index(Event self, int i):
    return self.c_Scintillator.getShort(str_to_char("index"),i)
  def sc_pindex(Event self, int i):
    return self.c_Scintillator.getShort(str_to_char("pindex"),i)
  def sc_detector(Event self, int i):
    return self.c_Scintillator.getByte(str_to_char("detector"),i)
  def sc_sector(Event self, int i):
    return self.c_Scintillator.getByte(str_to_char("sector"),i)
  def sc_layer(Event self, int i):
    return self.c_Scintillator.getByte(str_to_char("layer"),i)
  def sc_component(Event self, int i):
    return self.c_Scintillator.getShort(str_to_char("component"),i)
  def sc_energy(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("energy"),i)
  def sc_time(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("time"),i)
  def sc_path(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("path"),i)
  def sc_chi2(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("chi2"),i)
  def sc_x(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("x"),i)
  def sc_y(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("y"),i)
  def sc_z(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("z"),i)
  def sc_hx(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("hx"),i)
  def sc_hy(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("hy"),i)
  def sc_hz(Event self, int i):
    return self.c_Scintillator.getFloat(str_to_char("hz"),i)
  def sc_status(Event self, int i):
    return self.c_Scintillator.getShort(str_to_char("status"),i)

  def cal_len(Event self):
    return self.c_Calorimeter.getRows()
  def cal_index(Event self, int i):
    return self.c_Calorimeter.getShort(str_to_char("index"),i)
  def cal_pindex(Event self, int i):
    return self.c_Calorimeter.getShort(str_to_char("pindex"),i)
  def cal_detector(Event self, int i):
    return self.c_Calorimeter.getByte(str_to_char("detector"),i)
  def cal_sector(Event self, int i):
    return self.c_Calorimeter.getByte(str_to_char("sector"),i)
  def cal_layer(Event self, int i):
    return self.c_Calorimeter.getByte(str_to_char("layer"),i)
  def cal_energy(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("energy"),i)
  def cal_time(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("time"),i)
  def cal_path(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("path"),i)
  def cal_chi2(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("chi2"),i)
  def cal_x(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("x"),i)
  def cal_y(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("y"),i)
  def cal_z(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("z"),i)
  def cal_hx(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("hx"),i)
  def cal_hy(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("hy"),i)
  def cal_hz(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("hz"),i)
  def cal_lu(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("lu"),i)
  def cal_lv(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("lv"),i)
  def cal_lw(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("lw"),i)
  def cal_du(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("du"),i)
  def cal_dv(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("dv"),i)
  def cal_dw(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("dw"),i)
  def cal_m2u(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m2u"),i)
  def cal_m2v(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m2v"),i)
  def cal_m2w(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m2w"),i)
  def cal_m3u(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m3u"),i)
  def cal_m3v(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m3v"),i)
  def cal_m3w(Event self, int i):
    return self.c_Calorimeter.getFloat(str_to_char("m3w"),i)
  def cal_status(Event self, int i):
    return self.c_Calorimeter.getShort(str_to_char("status"),i)
