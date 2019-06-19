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

cdef dict get_id = {'PROTON': 2212, 'NEUTRON': 2112, 'PIP': 211, 'PIM': -211, 'PI0': 111, 'KP': 321, 'KM': -321, 'PHOTON': 22, 'ELECTRON': 11}
cdef dict part_mass = {11: 0.000511, 211: 0.13957, -211: 0.13957, 2212: 0.93827, 2112: 0.939565, 321: 0.493667, -321: 0.493667, 22: 0}


cdef extern from "TLorentzVector.h":
  cdef cppclass TLorentzVector:
    TLorentzVector() except +
    TLorentzVector(double x, double y, double z, double t) except +
    TLorentzVector operator*(double)
    double operator*(TLorentzVector)
    TLorentzVector operator+(TLorentzVector)
    TLorentzVector operator-(TLorentzVector)
    bool operator!=(TLorentzVector)
    bool operator==(TLorentzVector)
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
