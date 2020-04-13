# cython: profile=True
# distutils: language = c++
# cython: boundscheck=False

cimport cython

from libcpp.string cimport string
from libcpp cimport bool

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

cdef class hipo4_reader:
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

    bank*c_MCParticle
    bank*c_MCLund
    bank*c_MCEvent

  def __cinit__(hipo4_reader self, string filename):
    self.c_hiporeader = new reader(filename)
    self.c_dict = self.c_hiporeader.dictionary()
    self.c_event = new event()
    self.c_ForwardTagger = new bank(self.c_dict.getSchema(b"REC::ForwardTagger"))
    self.c_VertDoca = new bank(self.c_dict.getSchema(b"REC::VertDoca"))
    self.c_Track = new bank(self.c_dict.getSchema(b"REC::Track"))
    self.c_Cherenkov = new bank(self.c_dict.getSchema(b"REC::Cherenkov"))
    self.c_Event = new bank(self.c_dict.getSchema(b"REC::Event"))
    self.c_Particle = new bank(self.c_dict.getSchema(b"REC::Particle"))
    self.c_Scintillator = new bank(self.c_dict.getSchema(b"REC::Scintillator"))
    self.c_Calorimeter = new bank(self.c_dict.getSchema(b"REC::Calorimeter"))
    self.c_CovMat = new bank(self.c_dict.getSchema(b"REC::CovMat"))

    self.c_MCEvent = new bank(self.c_dict.getSchema(b"MC::Event"))
    self.c_MCLund = new bank(self.c_dict.getSchema(b"MC::Lund"))
    self.c_MCParticle = new bank(self.c_dict.getSchema(b"MC::Particle"))  

  def __iter__(hipo4_reader self):
      return self
  def __str__(hipo4_reader self):
    cdef bytes out = b"["
    out += self.c_dict.getSchema(b"REC::ForwardTagger").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::VertDoca").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Track").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Cherenkov").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Event").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Particle").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Scintillator").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::Calorimeter").json()
    out += b","
    out += self.c_dict.getSchema(b"REC::CovMat").json()
    out += b"]"
    return str(out)
  def __next__(hipo4_reader self):
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

      self.c_event.getStructure(self.c_MCParticle[0])
      self.c_event.getStructure(self.c_MCLund[0])
      self.c_event.getStructure(self.c_MCEvent[0])
      return self
    else:
      raise StopIteration
  def __len__(hipo4_reader self):
    return self.c_Particle.getRows()
  def pid(hipo4_reader self, int i):
    return self.c_Particle.getInt(b"pid" ,i)
  def px(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"px" ,i)
  def py(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"py" ,i)
  def pz(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"pz" ,i)
  def vx(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"vx" ,i)
  def vy(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"vy" ,i)
  def vz(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"vz" ,i)
  def charge(hipo4_reader self, int i):
    return self.c_Particle.getByte(b"charge", i)
  def beta(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"beta", i)
  def chi2pid(hipo4_reader self, int i):
    return self.c_Particle.getFloat(b"chi2pid", i)
  def status(hipo4_reader self, int i):
    return self.c_Particle.getShort(b"status", i)

  def event_len(hipo4_reader self):
    return self.c_Event.getRows()

  @property
  def category(hipo4_reader self):
    return self.c_Event.getLong(b"category",0)
  @property
  def topology(hipo4_reader self):
    return self.c_Event.getLong(b"topology",0)
  @property
  def beamCharge(hipo4_reader self):
    return self.c_Event.getFloat(b"beamCharge",0)
  @property
  def liveTime(hipo4_reader self):
    return self.c_Event.getDouble(b"liveTime",0)
  @property
  def startTime(hipo4_reader self):
    return self.c_Event.getFloat(b"startTime",0)
  @property
  def RFTime(hipo4_reader self):
    return self.c_Event.getFloat(b"RFTime",0)
  @property
  def helicity(hipo4_reader self):
    return self.c_Event.getByte(b"helicity",0)
  @property
  def helicityRaw(hipo4_reader self):
    return self.c_Event.getByte(b"helicityRaw",0)
  @property
  def procTime(hipo4_reader self):
    return self.c_Event.getFloat(b"procTime",0)
  
  def ft_len(hipo4_reader self):
    return self.c_ForwardTagger.getRows()
  def ft_pindex(hipo4_reader self, int i):
    return self.c_ForwardTagger.getShort(b"pindex", i)
  def ft_detector(hipo4_reader self, int i):
    return self.c_ForwardTagger.getByte(b"detector", i)
  def ft_energy(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"energy", i)
  def ft_time(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"time", i)
  def ft_path(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"path", i)
  def ft_chi2(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"chi2", i)
  def ft_x(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"x", i)
  def ft_y(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"y", i)
  def ft_z(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"z", i)
  def ft_dx(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"dx", i)
  def ft_dy(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"dy", i)
  def ft_radius(hipo4_reader self, int i):
    return self.c_ForwardTagger.getFloat(b"radius", i)
  def ft_size(hipo4_reader self, int i):
    return self.c_ForwardTagger.getShort(b"size", i)
  def ft_status(hipo4_reader self, int i):
    return self.c_ForwardTagger.getShort(b"status", i)

  def vd_len(hipo4_reader self):
    return self.c_VertDoca.getRows()
  def vd_index1(hipo4_reader self, int i):
    return self.c_VertDoca.getShort(b"index1",i)
  def vd_index2(hipo4_reader self, int i):
    return self.c_VertDoca.getShort(b"index2",i)
  def vd_x(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"x",i)
  def vd_y(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"y",i)
  def vd_z(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"z",i)
  def vd_x1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"x1",i)
  def vd_y1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"y1",i)
  def vd_z1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"z1",i)
  def vd_cx1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cx1",i)
  def vd_cy1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cy1",i)
  def vd_cz1(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cz1",i)
  def vd_x2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"x2",i)
  def vd_y2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"y2",i)
  def vd_z2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"z2",i)
  def vd_cx2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cx2",i)
  def vd_cy2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cy2",i)
  def vd_cz2(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"cz2",i)
  def vd_r(hipo4_reader self, int i):
    return self.c_VertDoca.getFloat(b"r",i)

  def trk_len(hipo4_reader self):
      return self.c_Track.getRows()
  def trk_index(hipo4_reader self, int i):
    return self.c_Track.getShort(b"index",i)
  def trk_pindex(hipo4_reader self, int i):
    return self.c_Track.getShort(b"pindex",i)
  def trk_detector(hipo4_reader self, int i):
    return self.c_Track.getByte(b"detector",i)
  def trk_sector(hipo4_reader self, int i):
    return self.c_Track.getByte(b"sector",i)
  def trk_status(hipo4_reader self, int i):
    return self.c_Track.getShort(b"status",i)
  def trk_q(hipo4_reader self, int i):
    return self.c_Track.getByte(b"q",i)
  def trk_chi2(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"chi2",i)
  def trk_NDF(hipo4_reader self, int i):
    return self.c_Track.getShort(b"NDF",i)
  def trk_px_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"px_nomm",i)
  def trk_py_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"py_nomm",i)
  def trk_pz_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"pz_nomm",i)
  def trk_vx_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"vx_nomm",i)
  def trk_vy_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"vy_nomm",i)
  def trk_vz_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"vz_nomm",i)
  def trk_chi2_nomm(hipo4_reader self, int i):
    return self.c_Track.getFloat(b"chi2_nomm",i)
  def trk_NDF_nomm(hipo4_reader self, int i):
    return self.c_Track.getShort(b"NDF_nomm",i)

  def chern_len(hipo4_reader self):
      return self.c_Cherenkov.getRows()
  def chern_index(hipo4_reader self, int i):
    return self.c_Cherenkov.getShort(b"index",i)
  def chern_pindex(hipo4_reader self, int i):
    return self.c_Cherenkov.getShort(b"pindex",i)
  def chern_detector(hipo4_reader self, int i):
    return self.c_Cherenkov.getByte(b"detector",i)
  def chern_sector(hipo4_reader self, int i):
    return self.c_Cherenkov.getByte(b"sector",i)
  def chern_nphe(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"nphe",i)
  def chern_time(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"time",i)
  def chern_path(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"path",i)
  def chern_chi2(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"chi2",i)
  def chern_x(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"x",i)
  def chern_y(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"y",i)
  def chern_z(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"z",i)
  def chern_theta(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"theta",i)
  def chern_phi(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"phi",i)
  def chern_dtheta(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"dtheta",i)
  def chern_dphi(hipo4_reader self, int i):
    return self.c_Cherenkov.getFloat(b"dphi",i)
  def chern_status(hipo4_reader self, int i):
    return self.c_Cherenkov.getShort(b"status",i)

  def sc_len(hipo4_reader self):
    return self.c_Scintillator.getRows()
  def sc_index(hipo4_reader self, int i):
    return self.c_Scintillator.getShort(b"index",i)
  def sc_pindex(hipo4_reader self, int i):
    return self.c_Scintillator.getShort(b"pindex",i)
  def sc_detector(hipo4_reader self, int i):
    return self.c_Scintillator.getByte(b"detector",i)
  def sc_sector(hipo4_reader self, int i):
    return self.c_Scintillator.getByte(b"sector",i)
  def sc_layer(hipo4_reader self, int i):
    return self.c_Scintillator.getByte(b"layer",i)
  def sc_component(hipo4_reader self, int i):
    return self.c_Scintillator.getShort(b"component",i)
  def sc_energy(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"energy",i)
  def sc_time(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"time",i)
  def sc_path(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"path",i)
  def sc_chi2(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"chi2",i)
  def sc_x(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"x",i)
  def sc_y(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"y",i)
  def sc_z(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"z",i)
  def sc_hx(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"hx",i)
  def sc_hy(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"hy",i)
  def sc_hz(hipo4_reader self, int i):
    return self.c_Scintillator.getFloat(b"hz",i)
  def sc_status(hipo4_reader self, int i):
    return self.c_Scintillator.getShort(b"status",i)

  def cal_len(hipo4_reader self):
    return self.c_Calorimeter.getRows()
  def cal_index(hipo4_reader self, int i):
    return self.c_Calorimeter.getShort(b"index",i)
  def cal_pindex(hipo4_reader self, int i):
    return self.c_Calorimeter.getShort(b"pindex",i)
  def cal_detector(hipo4_reader self, int i):
    return self.c_Calorimeter.getByte(b"detector",i)
  def cal_sector(hipo4_reader self, int i):
    return self.c_Calorimeter.getByte(b"sector",i)
  def cal_layer(hipo4_reader self, int i):
    return self.c_Calorimeter.getByte(b"layer",i)
  def cal_energy(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"energy",i)
  def cal_time(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"time",i)
  def cal_path(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"path",i)
  def cal_chi2(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"chi2",i)
  def cal_x(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"x",i)
  def cal_y(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"y",i)
  def cal_z(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"z",i)
  def cal_hx(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"hx",i)
  def cal_hy(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"hy",i)
  def cal_hz(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"hz",i)
  def cal_lu(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"lu",i)
  def cal_lv(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"lv",i)
  def cal_lw(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"lw",i)
  def cal_du(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"du",i)
  def cal_dv(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"dv",i)
  def cal_dw(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"dw",i)
  def cal_m2u(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m2u",i)
  def cal_m2v(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m2v",i)
  def cal_m2w(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m2w",i)
  def cal_m3u(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m3u",i)
  def cal_m3v(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m3v",i)
  def cal_m3w(hipo4_reader self, int i):
    return self.c_Calorimeter.getFloat(b"m3w",i)
  def cal_status(hipo4_reader self, int i):
    return self.c_Calorimeter.getShort(b"status",i)

  def mc_pid(hipo4_reader self, int i):
    return self.c_MCParticle.getInt(b"pid", i)
  def mc_px(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"px",i)
  def mc_py(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"py",i)
  def mc_pz(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"pz",i)
  def mc_vx(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"vx",i)
  def mc_vy(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"vy",i)
  def mc_vz(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"vz",i)
  def mc_vt(hipo4_reader self, int i):
    return self.c_MCParticle.getFloat(b"vt",i)

  def mc_lund_index(hipo4_reader self, int i):
    return self.c_MCLund.getByte(b"index",i)
  def mc_lund_lifetime(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"lifetime",i)
  def mc_lund_type(hipo4_reader self, int i):
    return self.c_MCLund.getByte(b"type",i)
  def mc_lund_pid(hipo4_reader self, int i):
    return self.c_MCLund.getInt(b"pid",i)
  def mc_lund_parent(hipo4_reader self, int i):
    return self.c_MCLund.getByte(b"parent",i)
  def mc_lund_daughter(hipo4_reader self, int i):
    return self.c_MCLund.getByte(b"daughter",i)
  def mc_lund_px(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"px",i)
  def mc_lund_py(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"py",i)
  def mc_lund_pz(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"pz",i)
  def mc_lund_energy(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"energy",i)
  def mc_lund_mass(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"mass",i)
  def mc_lund_vx(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"vx",i)
  def mc_lund_vy(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"vy",i)
  def mc_lund_vz(hipo4_reader self, int i):
    return self.c_MCLund.getFloat(b"vz",i)

  @property
  def mc_npart(hipo4_reader self):
    return self.c_MCEvent.getShort(b"npart",0)
  @property
  def mc_atarget(hipo4_reader self):
    return self.c_MCEvent.getShort(b"atarget",0)
  @property
  def mc_ztarget(hipo4_reader self):
    return self.c_MCEvent.getShort(b"ztarget",0)
  @property
  def mc_ptarget(hipo4_reader self):
    return self.c_MCEvent.getFloat(b"ptarget",0)
  @property
  def mc_pbeam(hipo4_reader self):
    return self.c_MCEvent.getFloat(b"pbeam",0)
  @property
  def mc_btype(hipo4_reader self):
    return self.c_MCEvent.getShort(b"btype",0)
  @property
  def mc_ebeam(hipo4_reader self):
    return self.c_MCEvent.getFloat(b"ebeam",0)
  @property
  def mc_targetid(hipo4_reader self):
    return self.c_MCEvent.getShort(b"targetid",0)
  @property
  def mc_processid(hipo4_reader self):
    return self.c_MCEvent.getShort(b"processid",0)
  @property
  def mc_weight(hipo4_reader self):
    return self.c_MCEvent.getFloat(b"weight",0)
