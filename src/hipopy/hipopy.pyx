# distutils: language = c++

from libcpp cimport bool

cdef extern from "utils.cpp":
  pass
cdef extern from "node.cpp":
  pass
cdef extern from "reader.cpp":
  pass
cdef extern from "event.cpp":
  pass
cdef extern from "record.cpp":
  pass
cdef extern from "dictionary.cpp":
  pass

cdef extern from "hipo/node.h" namespace "hipo":
    cdef cppclass node[T]:
      node() except +
      int getLength()

cdef extern from "hipo/reader.h" namespace "hipo":
    cdef cppclass reader:
      reader() except +
      reader(bool) except +
      void open(char*)
      int getRecordCount()
      bool next()
      bool  isOpen()
      void  showInfo()
      node *getBranch(int, int)
      node *getBranch(char*,char*)

cdef class hipo_reader:
  cdef reader*c_reader
  def __cinit__(self):
    self.c_reader = new reader(True)

  def __cinit__(self, filename):
    self.c_reader = new reader(True)
    cdef bytes py_bytes = filename.encode()
    cdef char* c_string = py_bytes
    self.c_reader.open(c_string)

  def open(self, filename):
    cdef bytes py_bytes = filename.encode()
    cdef char* c_string = py_bytes
    self.c_reader.open(c_string)

  def isOpen(self):
    return self.c_reader.isOpen()

  def showInfo(self):
    return self.c_reader.showInfo()

  def getRecordCount(self):
    return self.c_reader.getRecordCount()

  def next(self):
    return self.c_reader.next()

  def getIntBranch(self, group, item):
    print(group, item)
    #cdef node[int]*c_int_node(group,item)
    #print(c_int_node.getLength())
