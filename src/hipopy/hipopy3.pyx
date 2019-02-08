# cython: profile=True
# distutils: language = c++
# cython: boundscheck=False

cimport cython
from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp.map cimport map
from libcpp.utility cimport pair
from libcpp cimport bool
from cython.view cimport array as cvarray
from libc.stdlib cimport free

from collections import defaultdict

from libc.math cimport isnan, sqrt, log, atan2

import numpy as np

import json

cdef dict get_id = {'PROTON': 2212, 'NEUTRON': 2112, 'PIP': 211, 'PIM': -211, 'PI0': 111, 'KP': 321, 'KM': -321, 'PHOTON': 22, 'ELECTRON': 11}

cdef dict part_mass = {11: 0.000511, 211: 0.13957, -211: 0.13957, 2212: 0.93827, 2112: 0.939565, 321: 0.493667, -321: 0.493667, 22: 0}

cdef extern from "hipo3/node.h" namespace "hipo":
    cdef cppclass node[T]:
      node() except +
      node(int, int) except +
      T getValue(int)
      void   reset()
      void   show()
      int    getLength()
      char  *getAddress()
      int    getBytesLength()
      void   setLength(int)
      void   setAddress(char *)

cdef extern from "hipo3/reader.h" namespace "hipo":
    cdef cppclass reader:
      reader(string) except +
      vector[string] getDictionary()
      void open(char*)
      int getRecordCount()
      bool next()
      bool  isOpen()
      void  showInfo()
      node *getBranch[T](char*,char*)

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

cdef extern from "TVector3.h":
  cdef cppclass TVector3:
    TVector3 () except +
    TVector3 (double x, double y, double z) except +
    void SetXYZ(double x, double y, double z)
    double x()
    double y()
    double z()
    double X()
    double Y()
    double Z()
    double Px()
    double Py()
    double Pz()
    double Phi()
    double Theta()
    double CosTheta()
    double Mag2()
    double Mag()
    double Perp2()
    double Pt()
    double Perp()
    double Eta ()
    void RotateX (double x)
    void RotateY (double x)
    void RotateZ (double x)
    void Print()

cdef char* str_to_char(str name):
  """Convert python string to char*"""
  cdef bytes name_bytes = name.encode()
  cdef char* c_name = name_bytes
  return c_name

cdef class int_node:
  cdef node[int]*c_node

  def __cinit__(self):
    self.c_node = new node[int]()
  def __dealloc__(self):
    free(self.c_node)
  def __getitem__(self, int x):
    return self.c_node.getValue(x)
  def __len__(self):
    return self.c_node.getLength()
  def show(self):
    self.c_node.show()
  cdef setup(self, node[int]* node):
    self.c_node = node
  cdef int getValue(self, int x):
    return self.c_node.getValue(x)
  cdef int getLength(self):
    return self.c_node.getLength()


cdef class char_node:
  cdef node[char]*c_node
  def __cinit__(self):
    self.c_node = new node[char]()
  def __dealloc__(self):
    free(self.c_node)
  def __getitem__(self, int x):
    return self.c_node.getValue(x)
  def __len__(self):
    return self.c_node.getLength()
  def show(self):
    self.c_node.show()
  cdef setup(self, node[char]* node):
    self.c_node = node
  cdef char getValue(self, int x):
    return self.c_node.getValue(x)
  cdef int getLength(self):
    return self.c_node.getLength()


cdef class float_node:
  cdef node[float]*c_node
  def __cinit__(self):
    self.c_node = new node[float]()
  def __dealloc__(self):
    free(self.c_node)
  def __getitem__(self, int x):
    return self.c_node.getValue(x)
  def __len__(self):
    return self.c_node.getLength()
  def show(self):
    self.c_node.show()
  cdef setup(self, node[float]* node):
    self.c_node = node
  cdef float getValue(self, int x):
    return self.c_node.getValue(x)
  cdef int getLength(self):
    return self.c_node.getLength()

cdef class short_node:
  cdef node[short]*c_node
  def __cinit__(self):
    self.c_node = new node[short]()
  def __dealloc__(self):
    free(self.c_node)
  def __getitem__(self, int x):
    return self.c_node.getValue(x)
  def __len__(self):
    return self.c_node.getLength()
  def show(self):
    self.c_node.show()
  cdef setup(self, node[short]* node):
    self.c_node = node
  cdef short getValue(self, int x):
    return self.c_node.getValue(x)
  cdef int getLength(self):
    return self.c_node.getLength()

cdef class long_node:
  cdef node[long]*c_node
  def __cinit__(self):
    self.c_node = new node[long]()
  def __dealloc__(self):
    free(self.c_node)
  def __getitem__(self, int x):
    return self.c_node.getValue(x)
  def __len__(self):
    return self.c_node.getLength()
  def show(self):
    self.c_node.show()
  cdef setup(self, node[long]* node):
    self.c_node = node
  cdef long getValue(self, int x):
    return self.c_node.getValue(x)
  cdef int getLength(self):
    return self.c_node.getLength()

cdef class hipo3_reader:
  """hipo3_reader based on hipo::reader class"""
  # Define hipo::reader class
  cdef reader*c_reader
  def __cinit__(self, filename):
    """Initialize hipo3_reader with a file"""
    self.c_reader = new reader(filename)
  def __str__(self):
    return self.jsonString()

  def __repr__(self):
    return self.jsonString()
  cdef void open(self, filename):
    """Open a new hipo file with the hipo::reader"""
    cdef bytes filename_bytes = filename.encode()
    cdef char* c_filename = filename_bytes
    self.c_reader.open(c_filename)

  def isOpen(self):
    """Check if the file is open"""
    return self.c_reader.isOpen()

  def showInfo(self):
    """Shows the files info from hipo::reader.showInfo()"""
    self.c_reader.showInfo()

  def getRecordCount(self):
    """Return the number of records in the file"""
    return self.c_reader.getRecordCount()

  def next(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  cdef bool c_next(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  def __next__(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  def getDictionary(self):
    """Get dictionary string from hipo file [More useful to use getjson]"""
    return self.c_reader.getDictionary()

  def jsonString(self):
    """Get dictionary as string"""
    hipo_dict = self.c_reader.getDictionary()
    out = []
    out.append("[{\n")
    for dic in hipo_dict:
        dic = str(dic)
        dic = dic.split("{")[1].split("}")
        bank = dic[0].split(",")
        out.append("\t\"bank\" : \""+bank[1]+"\",\n")
        out.append("\t\"group\" : "+ bank[0]+",\n")
        out.append("\t\t\"items\": [\n")
        items = dic[1].split("[")
        ids = []
        for x in items[1:]:
            ids = x.split("]")[0].split(",")
            out.append("\t\t{")
            out.append("\"name\": \"{0}\", \"id\": {1}, \"type\": \"{2}\"".format(ids[1],ids[0],str(ids[2]).lower()))
            out.append("},\n")
        out[-1] = "}\t]\n},\n{\n"

    out[-1] = "}\n]\n}\n]"
    out = ''.join(out)
    return out

  def getJson(self):
    """Get dictionary as a json object"""
    return json.loads(self.jsonString())

  cpdef getIntNode(self, group, item):
    """Create a hipo::node<int> which is accesible to python"""
    cdef node[int]*c_node
    group = str_to_char(group)
    item = str_to_char(item)
    c_node = self.c_reader.getBranch[int](group, item)
    py_node = int_node()
    py_node.setup(c_node)
    return py_node

  cpdef getLongNode(self, group, item):
    """Create a hipo::node<long> which is accesible to python"""
    cdef node[long]*c_node
    group = str_to_char(group)
    item = str_to_char(item)
    c_node = self.c_reader.getBranch[long](group, item)
    py_node = long_node()
    py_node.setup(c_node)
    return py_node

  cpdef getInt8Node(self, group, item):
    """Create a hipo::node<int8_t> which is accesible to python"""
    cdef node[char]*c_node
    group = str_to_char(group)
    item = str_to_char(item)
    c_node = self.c_reader.getBranch[char](group, item)
    py_node = char_node()
    py_node.setup(c_node)
    return py_node

  cpdef getFloatNode(self, group, item):
    """Create a hipo::node<float> which is accesible to python"""
    cdef node[float]*c_node
    group = str_to_char(group)
    item = str_to_char(item)
    c_node = self.c_reader.getBranch[float](group, item)
    py_node = float_node()
    py_node.setup(c_node)
    return py_node

  cpdef getInt16Node(self, group, item):
    """Create a hipo::node<int16_t> which is accesible to python"""
    cdef node[short]*c_node
    group = str_to_char(group)
    item = str_to_char(item)
    c_node = self.c_reader.getBranch[short](group, item)
    py_node = short_node()
    py_node.setup(c_node)
    return py_node

cdef class LorentzVector:
  cdef TLorentzVector*c_TLorentzVector
  def __cinit__(LorentzVector self, double px, double py, double pz, **kwargs):
    if "energy" in kwargs:
      self.c_TLorentzVector = new TLorentzVector(px, py, pz, kwargs["energy"])
    elif "mass" in kwargs:
      self.c_TLorentzVector = new TLorentzVector()
      self.c_TLorentzVector.SetXYZM(px, py, pz, kwargs["mass"])
    elif "pid" in kwargs:
      self.c_TLorentzVector = new TLorentzVector()
      self.c_TLorentzVector.SetXYZM(px, py, pz, part_mass[kwargs["pid"]])
    else:
      self.c_TLorentzVector = new TLorentzVector(px, py, pz, 0)
  def __dealloc__(self):
    free(self.c_TLorentzVector)
  def __del__(self):
    free(self.c_TLorentzVector)
  def __add__(LorentzVector self, LorentzVector other):
    cdef double X = self.c_TLorentzVector.Px() + other.c_TLorentzVector.Px()
    cdef double Y = self.c_TLorentzVector.Py() + other.c_TLorentzVector.Py()
    cdef double Z = self.c_TLorentzVector.Pz() + other.c_TLorentzVector.Pz()
    cdef double E = self.c_TLorentzVector.E() + other.c_TLorentzVector.E()
    return LorentzVector(X, Y, Z, energy=E)
  def __iadd__(LorentzVector self, LorentzVector other):
    cdef double X = self.c_TLorentzVector.Px() + other.c_TLorentzVector.Px()
    cdef double Y = self.c_TLorentzVector.Py() + other.c_TLorentzVector.Py()
    cdef double Z = self.c_TLorentzVector.Pz() + other.c_TLorentzVector.Pz()
    cdef double E = self.c_TLorentzVector.E() + other.c_TLorentzVector.E()
    return LorentzVector(X, Y, Z, energy=E)
  def __sub__(LorentzVector self, LorentzVector other):
    cdef double X = self.c_TLorentzVector.Px() - other.c_TLorentzVector.Px()
    cdef double Y = self.c_TLorentzVector.Py() - other.c_TLorentzVector.Py()
    cdef double Z = self.c_TLorentzVector.Pz() - other.c_TLorentzVector.Pz()
    cdef double E = self.c_TLorentzVector.E() - other.c_TLorentzVector.E()
    return LorentzVector(X, Y, Z, energy=E)
  def __isub__(LorentzVector self, LorentzVector other):
    cdef double X = self.c_TLorentzVector.Px() - other.c_TLorentzVector.Px()
    cdef double Y = self.c_TLorentzVector.Py() - other.c_TLorentzVector.Py()
    cdef double Z = self.c_TLorentzVector.Pz() - other.c_TLorentzVector.Pz()
    cdef double E = self.c_TLorentzVector.E() - other.c_TLorentzVector.E()
    return LorentzVector(X, Y, Z, energy=E)
  def __str__(self):
    return "Px {0: 0.2f} | Py {1: 0.2f} | Pz {2: 0.2f} | E {3: 0.2f}".format(self.Px,self.Py ,self.Pz, self.E)
  def __repr__(self):
    return self.__str__()
  def MomentumVec(LorentzVector self):
    return ThreeVector(self.c_TLorentzVector.Px(), self.c_TLorentzVector.Py(), self.c_TLorentzVector.Pz())
  def SetPxPyPzM(LorentzVector self, double px, double py, double pz, double mass):
    self.c_TLorentzVector.SetXYZM(px, py, pz, mass)
  @property
  def Px(LorentzVector self):
    return self.c_TLorentzVector.Px()
  @property
  def Py(LorentzVector self):
    return self.c_TLorentzVector.Py()
  @property
  def Pz(LorentzVector self):
    return self.c_TLorentzVector.Pz()
  @property
  def P(LorentzVector self):
    return self.c_TLorentzVector.P()
  @property
  def E(LorentzVector self):
    return self.c_TLorentzVector.E()
  @property
  def Energy(LorentzVector self):
    return self.c_TLorentzVector.E()
  @property
  def Theta(LorentzVector self):
    return self.c_TLorentzVector.Theta()
  @property
  def CosTheta(LorentzVector self):
    return self.c_TLorentzVector.CosTheta()
  @property
  def Phi(LorentzVector self):
    return self.c_TLorentzVector.Phi()
  @property
  def Rho(LorentzVector self):
    return self.c_TLorentzVector.Rho()
  @property
  def Perp2(LorentzVector self):
    return self.c_TLorentzVector.Perp2()
  @property
  def Pt(LorentzVector self):
    return self.c_TLorentzVector.Pt()
  @property
  def Perp(LorentzVector self):
    return self.c_TLorentzVector.Perp()
  @property
  def Et2(LorentzVector self):
    return self.c_TLorentzVector.Et2()
  @property
  def Et(LorentzVector self):
    return self.c_TLorentzVector.Et()
  @property
  def Mag2(LorentzVector self):
    return self.c_TLorentzVector.Mag2()
  @property
  def M2(LorentzVector self):
    return self.c_TLorentzVector.M2()
  @property
  def Mag(LorentzVector self):
    return self.c_TLorentzVector.Mag()
  @property
  def M(LorentzVector self):
    return self.c_TLorentzVector.M()
  @property
  def Mt2(LorentzVector self):
    return self.c_TLorentzVector.Mt2()
  @property
  def Mt(LorentzVector self):
    return self.c_TLorentzVector.Mt()
  @property
  def Beta(LorentzVector self):
    return self.c_TLorentzVector.Beta()
  @property
  def Gamma(LorentzVector self):
    return self.c_TLorentzVector.Gamma()
  @property
  def Plus(LorentzVector self):
    return self.c_TLorentzVector.Plus()
  @property
  def Minus(LorentzVector self):
    return self.c_TLorentzVector.Minus()
  @property
  def Rapidity(LorentzVector self):
    return self.c_TLorentzVector.Rapidity()
  @property
  def Eta(LorentzVector self):
    return self.c_TLorentzVector.Eta()
  @property
  def PseudoRapidity(LorentzVector self):
    return self.c_TLorentzVector.PseudoRapidity()

cdef class ThreeVector:
  cdef TVector3*c_TVector3
  def __cinit__(ThreeVector self, double vx, double vy, double vz):
    c_TVector3 = new TVector3(vx, vy, vz)
  def __add__(ThreeVector self, ThreeVector other):
    cdef double X = self.c_TVector3.x() + other.c_TVector3.x()
    cdef double Y = self.c_TVector3.y() + other.c_TVector3.y()
    cdef double Z = self.c_TVector3.z() + other.c_TVector3.z()
    return ThreeVector(X, Y, Z)
  def __str__(ThreeVector self):
    return "Vx {0: 0.2f} | Vy {1: 0.2f} | Vz {2: 0.2f}".format(self.vx,self.vy ,self.vz)
  def __repr__(self):
    return self.__str__()
  def __dealloc__(self):
    free(self.c_TVector3)
  def __del__(self):
    free(self.c_TVector3)
  @property
  def x(ThreeVector self):
    return self.c_TVector3.x()
  @property
  def y(ThreeVector self):
    return self.c_TVector3.y()
  @property
  def z(ThreeVector self):
    return self.c_TVector3.z()
  @property
  def X(ThreeVector self):
    return self.c_TVector3.X()
  @property
  def Y(ThreeVector self):
    return self.c_TVector3.Y()
  @property
  def Z(ThreeVector self):
    return self.c_TVector3.Z()
  @property
  def Px(ThreeVector self):
    return self.c_TVector3.Px()
  @property
  def Py(ThreeVector self):
    return self.c_TVector3.Py()
  @property
  def Pz(ThreeVector self):
    return self.c_TVector3.Pz()
  @property
  def Phi(ThreeVector self):
    return self.c_TVector3.Phi()
  @property
  def Theta(ThreeVector self):
    return self.c_TVector3.Theta()
  @property
  def CosTheta(ThreeVector self):
    return self.c_TVector3.CosTheta()
  @property
  def Mag2(ThreeVector self):
    return self.c_TVector3.Mag2()
  @property
  def Mag(ThreeVector self):
    return self.c_TVector3.Mag()
  @property
  def Perp2(ThreeVector self):
    return self.c_TVector3.Perp2()
  @property
  def Pt(ThreeVector self):
    return self.c_TVector3.Pt()
  @property
  def Perp(ThreeVector self):
    return self.c_TVector3.Perp()
  @property
  def Eta (ThreeVector self):
    return self.c_TVector3.Eta()


cdef class Particle:
  cdef:
    int pid, charge
    double mass, beta
    LorentzVector FourVector
    ThreeVector Vertex
  def __cinit__(Particle self, double px, double py, double pz, int pid, double vx, double vy, double vz, int charge, double beta):
    self.pid = pid
    self.charge = charge
    self.beta = beta
    self.mass = part_mass.get(self.pid, 0)
    self.Vertex = ThreeVector(vx, vy, vz)
    self.FourVector = LorentzVector(px, py, pz, mass=self.mass)
  def __add__(Particle self, Particle other):
    return self.FourVector + other.FourVector
  def __sub__(Particle self, Particle other):
    return self.FourVector - other.FourVector
  def __str__(Particle self):
    return "pid {:5d} | ".format(self.pid) + self.FourVector.__str__()
  def __repr__(Particle self):
    return self.__str__()
  @property
  def FourVector(Particle self):
    return self.FourVector
  @property
  def Vertex(Particle self):
    return self.Vertex
  @property
  def pid(Particle self):
    return self.pid
  @property
  def charge(Particle self):
    return self.charge
  @property
  def mass(Particle self):
    return self.mass
  @property
  def beta(Particle self):
    return self.beta
  @property
  def Px(Particle self):
    return self.FourVector.Px
  @property
  def Py(Particle self):
    return self.FourVector.Py
  @property
  def Pz(Particle self):
    return self.FourVector.Pz
  @property
  def Vx(Particle self):
    return self.Vertex.x
  @property
  def Vy(Particle self):
    return self.Vertex.y
  @property
  def Vz(Particle self):
    return self.Vertex.z
  @property
  def P(Particle self):
    return self.FourVector.P
  @property
  def E(Particle self):
    return self.FourVector.E
  @property
  def Energy(Particle self):
    return self.FourVector.E
  @property
  def Theta(Particle self):
    return self.FourVector.Theta
  @property
  def CosTheta(Particle self):
    return self.FourVector.CosTheta
  @property
  def Phi(Particle self):
    return self.FourVector.Phi
  @property
  def Rho(Particle self):
    return self.FourVector.Rho
  @property
  def Perp2(Particle self):
    return self.FourVector.Perp2
  @property
  def Pt(Particle self):
    return self.FourVector.Pt
  @property
  def Perp(Particle self):
    return self.FourVector.Perp
  @property
  def Et2(Particle self):
    return self.FourVector.Et2
  @property
  def Et(Particle self):
    return self.FourVector.Et
  @property
  def Mag2(Particle self):
    return self.FourVector.Mag2
  @property
  def M2(Particle self):
    return self.FourVector.M2
  @property
  def Mag(Particle self):
    return self.FourVector.Mag
  @property
  def M(Particle self):
    return self.FourVector.M
  @property
  def Mt2(Particle self):
    return self.FourVector.Mt2
  @property
  def Mt(Particle self):
    return self.FourVector.Mt
  @property
  def Beta(Particle self):
    return self.FourVector.Beta
  @property
  def Gamma(Particle self):
    return self.FourVector.Gamma
  @property
  def Plus(Particle self):
    return self.FourVector.Plus
  @property
  def Minus(Particle self):
    return self.FourVector.Minus
  @property
  def Rapidity(Particle self):
    return self.FourVector.Rapidity
  @property
  def Eta(Particle self):
    return self.FourVector.Eta
  @property
  def PseudoRapidity(Particle self):
    return self.FourVector.PseudoRapidity


cdef class Event:
  cdef:
    hipo3_reader hiporeader
    int run
    #RUN::config
    #REC::Particle
    int_node _run, _pid, _event
    char_node _charge, _pindex
    float_node _px, _py, _pz, _vx, _vy, _vz, _beta, _torus, _solenoid
    #REC::Calorimeter
    short_node _ec_pindex
    char_node _ec_detector, _ec_sector, _ec_layer
    float_node _ec_energy, _ec_time, _ec_path, _ec_x, _ec_y, _ec_z, _ec_lu, _ec_lv, _ec_lw
    #REC::Cherenkov
    short_node _cc_pindex
    char_node _cc_detector, _cc_sector
    float_node _cc_nphe, _cc_time, _cc_path, _cc_theta, _cc_phi
    #REC::ForwardTagger
    short_node _ft_pindex, _ft_size
    char_node _ft_detector
    float_node _ft_energy, _ft_time, _ft_path, _ft_x, _ft_y, _ft_z, _ft_dx, _ft_dy, _ft_radius
    #REC::Scintillator
    short_node _sc_pindex, _sc_component
    char_node _sc_detector, _sc_sector, _sc_layer
    float_node _sc_energy, _sc_time, _sc_path
    #REC::Track
    short_node _track_pindex, _track_NDF, _track_NDF_nomm
    char_node _track_detector, _track_sector
    float_node _track_chi2, _track_chi2_nomm
    #REC::Traj
    short_node _traj_pindex, _traj_detId
    float_node _traj_x, _traj_y, _traj_z, _traj_cx, _traj_cy, _traj_cz

    public list particle, ids

  def __cinit__(Event self, hipo3_reader reader):
    self.hiporeader = reader
    #RUN::config
    self._run = reader.getIntNode(u"RUN::config", u"run")
    self._event = reader.getIntNode(u"RUN::config", u"event")
    self._torus = reader.getFloatNode(u"RUN::config", u"torus")
    self._solenoid = reader.getFloatNode(u"RUN::config", u"solenoid")
    #REC::Particle
    self._pid = reader.getIntNode("REC::Particle", "pid")
    self._px = reader.getFloatNode("REC::Particle", "px")
    self._py = reader.getFloatNode("REC::Particle", "py")
    self._pz = reader.getFloatNode("REC::Particle", "pz")
    self._vx = reader.getFloatNode("REC::Particle", "vx")
    self._vy = reader.getFloatNode("REC::Particle", "vy")
    self._vz = reader.getFloatNode("REC::Particle", "vz")
    self._charge = reader.getInt8Node("REC::Particle", "charge")
    self._beta = reader.getFloatNode("REC::Particle", "beta")
    #REC::Calorimeter
    self._ec_pindex = reader.getInt16Node(u"REC::Calorimeter", u"pindex")
    self._ec_detector = reader.getInt8Node(u"REC::Calorimeter", u"detector")
    self._ec_sector = reader.getInt8Node(u"REC::Calorimeter", u"sector")
    self._ec_layer = reader.getInt8Node(u"REC::Calorimeter", u"layer")
    self._ec_energy = reader.getFloatNode(u"REC::Calorimeter", u"energy")
    self._ec_time = reader.getFloatNode(u"REC::Calorimeter", u"time")
    self._ec_path = reader.getFloatNode(u"REC::Calorimeter", u"path")
    self._ec_x = reader.getFloatNode(u"REC::Calorimeter", u"x")
    self._ec_y = reader.getFloatNode(u"REC::Calorimeter", u"y")
    self._ec_z = reader.getFloatNode(u"REC::Calorimeter", u"z")
    self._ec_lu = reader.getFloatNode(u"REC::Calorimeter", u"lu")
    self._ec_lv = reader.getFloatNode(u"REC::Calorimeter", u"lv")
    self._ec_lw = reader.getFloatNode(u"REC::Calorimeter", u"lw")
    #REC::Cherenkov
    self._cc_pindex = reader.getInt16Node(u"REC::Cherenkov", u"pindex")
    self._cc_detector = reader.getInt8Node(u"REC::Cherenkov", u"detector")
    self._cc_sector = reader.getInt8Node(u"REC::Cherenkov", u"sector")
    self._cc_nphe = reader.getFloatNode(u"REC::Cherenkov", u"nphe")
    self._cc_time = reader.getFloatNode(u"REC::Cherenkov", u"time")
    self._cc_path = reader.getFloatNode(u"REC::Cherenkov", u"path")
    self._cc_theta = reader.getFloatNode(u"REC::Cherenkov", u"theta")
    self._cc_phi = reader.getFloatNode(u"REC::Cherenkov", u"phi")
    #REC::ForwardTagger
    self._ft_pindex = reader.getInt16Node(u"REC::ForwardTagger", u"pindex")
    self._ft_detector = reader.getInt8Node(u"REC::ForwardTagger", u"detector")
    self._ft_energy = reader.getFloatNode(u"REC::ForwardTagger", u"energy")
    self._ft_time = reader.getFloatNode(u"REC::ForwardTagger", u"time")
    self._ft_path = reader.getFloatNode(u"REC::ForwardTagger", u"path")
    self._ft_x = reader.getFloatNode(u"REC::ForwardTagger", u"x")
    self._ft_y = reader.getFloatNode(u"REC::ForwardTagger", u"y")
    self._ft_z = reader.getFloatNode(u"REC::ForwardTagger", u"z")
    self._ft_dx = reader.getFloatNode(u"REC::ForwardTagger", u"dx")
    self._ft_dy = reader.getFloatNode(u"REC::ForwardTagger", u"dy")
    self._ft_radius = reader.getFloatNode(u"REC::ForwardTagger", u"radius")
    self._ft_size = reader.getInt16Node(u"REC::ForwardTagger", u"size")
    #REC::Scintillator
    self._sc_pindex = reader.getInt16Node(u"REC::Scintillator", u"pindex")
    self._sc_detector = reader.getInt8Node(u"REC::Scintillator", u"detector")
    self._sc_sector = reader.getInt8Node(u"REC::Scintillator", u"sector")
    self._sc_layer = reader.getInt8Node(u"REC::Scintillator", u"layer")
    self._sc_component = reader.getInt16Node(u"REC::Scintillator", u"component")
    self._sc_energy = reader.getFloatNode(u"REC::Scintillator", u"energy")
    self._sc_time = reader.getFloatNode(u"REC::Scintillator", u"time")
    self._sc_path = reader.getFloatNode(u"REC::Scintillator", u"path")
    #REC::Track
    self._track_pindex = reader.getInt16Node(u"REC::Track", u"pindex")
    self._track_detector = reader.getInt8Node(u"REC::Track", u"detector")
    self._track_sector = reader.getInt8Node(u"REC::Track", u"sector")
    self._track_chi2 = reader.getFloatNode(u"REC::Track", u"chi2")
    self._track_NDF = reader.getInt16Node(u"REC::Track", u"NDF")
    self._track_chi2_nomm = reader.getFloatNode(u"REC::Track", u"chi2_nomm")
    self._track_NDF_nomm = reader.getInt16Node(u"REC::Track", u"NDF_nomm")
    #REC::Traj
    self._traj_pindex = reader.getInt16Node(u"REC::Traj", u"pindex")
    self._traj_detId = reader.getInt16Node(u"REC::Traj", u"detId")
    self._traj_x = reader.getFloatNode(u"REC::Traj", u"x")
    self._traj_y = reader.getFloatNode(u"REC::Traj", u"y")
    self._traj_z = reader.getFloatNode(u"REC::Traj", u"z")
    self._traj_cx = reader.getFloatNode(u"REC::Traj", u"cx")
    self._traj_cy = reader.getFloatNode(u"REC::Traj", u"cy")
    self._traj_cz = reader.getFloatNode(u"REC::Traj", u"cz")
  def __len__(Event self):
    return self._pid.getLength()
  def __iter__(Event self):
      return self
  def __next__(Event self):
    if self.hiporeader.c_next():
      self.loadParts()
      return self
    else:
      raise StopIteration
  cdef void loadParts(Event self):
    if self._run.getLength() > 0:
      self.run = self._run[0]
    cdef int l = len(self)
    cdef int i = 0
    self.particle = [None] * l
    self.ids = [None] * l
    for i in xrange(0, l):
      self.ids[i] = self._pid[i]
      self.particle[i] = Particle(self._px[i], self._py[i], self._pz[i], self._pid[i],
                          self._vx[i], self._vy[i], self._vz[i], self._charge[i], self._beta[i])
