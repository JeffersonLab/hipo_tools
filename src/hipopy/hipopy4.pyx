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

cdef extern from "hipo4/reader.h" namespace "hipo":
    cdef cppclass reader:
      reader() except +
      reader(char*) except +
      reader(string) except +
      void open(char*)
      bool hasNext()
      bool next()


cdef extern from "hipo4/dictionary.h" namespace "hipo":
    cdef cppclass schema:
      schema() except +
    cdef cppclass dictionary:
      dictionary() except +
      schema getSchema(string)

cdef extern from "hipo4/bank.h" namespace "hipo":
    cdef cppclass bank:
      bank() except +
      bank(schema) except +
      int    getInt(string, int)
      int    getShort(string, int)
      int    getByte(string, int)
      float  getFloat(string, int)
      double getDouble(string, int)
      long   getLong(string, int)




cdef class node:
  cdef dictionary *c_dict
  cdef schema *c_schema
  cdef bank *c_bank
  def __init__(self, name):
      self.name = name
      self.c_dict = new dictionary()
      self.c_bank = new bank(self.c_dict.getSchema(self.name))
  def getInt(self, name, i):
    return self.c_bank.getInt(name, i)
  def getShort(self, name, i):
    return self.c_bank.getShort(name, i)
  def getByte(self, name, i):
    return self.c_bank.getByte(name, i)
  def getFloat(self, name, i):
    return self.c_brank.getFloat(name, i)
  def getDouble(self, name, i):
    return self.c_brank.getDouble(name, i)
  def getLong(self, name, i):
    return self.c_brank.getLong(name, i)


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

cdef class hipo4_reader:
  """Hipo_reader based on hipo::reader class"""
  # Define hipo::reader class
  cdef reader*c_reader
  def __cinit__(self, filename):
    """Initialize hipo_reader with a file"""
    self.c_reader = new reader()
    self.open(filename)

  def __str__(self):
    return self.jsonString()

  def __repr__(self):
    return self.jsonString()
  cdef void open(self, filename):
    """Open a new hipo file with the hipo::reader"""
    cdef bytes filename_bytes = filename.encode()
    cdef char* c_filename = filename_bytes
    self.c_reader.open(c_filename)

  def next(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  cdef bool c_next(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  def __next__(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

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
    hipo4_reader hiporeader
    int run

    public list particle, ids

  def __cinit__(Event self, hipo4_reader reader):
    self.hiporeader = reader

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
