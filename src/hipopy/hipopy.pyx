from libcpp.vector cimport vector
from libcpp.string cimport string
from libcpp cimport bool

import json

cdef extern from "hipo/node.h" namespace "hipo":
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

cdef extern from "hipo/reader.h" namespace "hipo":
    cdef cppclass reader:
      reader() except +
      reader(bool) except +
      vector[string] getDictionary()
      void open(char*)
      int getRecordCount()
      bool next()
      bool  isOpen()
      void  showInfo()
      node *getBranch[T](char*,char*)


cpdef char* str_to_char(str name):
  """Convert python string to char*"""
  cdef bytes name_bytes = name.encode()
  cdef char* c_name = name_bytes
  return c_name


cdef class int_node:
  cdef node[int]*c_node

  def __cinit__(self):
    self.c_node = new node[int]()

  def __getitem__(self, arg):
    return self.c_node.getValue(arg)

  def __len__(self):
    return self.c_node.getLength()

  def __str__(self):
    self.c_node.show()

  def __repr__(self):
    self.c_node.show()

  cdef setup(self, node[int]* node):
    self.c_node = node

  cpdef int getValue(self, x):
    return self.c_node.getValue(x)

  cpdef int getLength(self):
    return self.c_node.getLength()


cdef class char_node:
  cdef node[char]*c_node

  def __cinit__(self):
    self.c_node = new node[char]()

  def __getitem__(self, arg):
    return self.c_node.getValue(arg)

  def __len__(self):
    return self.c_node.getLength()

  def __str__(self):
    self.c_node.show()

  def __repr__(self):
    self.c_node.show()

  cdef setup(self, node[char]* node):
    self.c_node = node

  cpdef char getValue(self, x):
    return self.c_node.getValue(x)

  cpdef char getLength(self):
    return self.c_node.getLength()


cdef class float_node:
  cdef node[float]*c_node
  def __cinit__(self):
    self.c_node = new node[float]()

  def __getitem__(self, arg):
    return self.c_node.getValue(arg)

  def __len__(self):
    return self.c_node.getLength()

  def __str__(self):
    self.c_node.show()

  def __repr__(self):
    self.c_node.show()

  cdef setup(self, node[float]* node):
    self.c_node = node

  cpdef float getValue(self, x):
    return self.c_node.getValue(x)

  cpdef int getLength(self):
    return self.c_node.getLength()

cdef class hipo_reader:
  """Hipo_reader based on hipo::reader class"""
  # Define hipo::reader class
  cdef reader*c_reader
  def __cinit__(self):
    self.c_reader = new reader(True)

  def __cinit__(self, str filename, bool randomAccess = True):
    """Initialize hipo_reader with a file"""
    self.c_reader = new reader(randomAccess)
    self.open(filename)

  def __str__(self):
    return self.getjson()

  def __repr__(self):
    return self.getjson()

  cpdef void open(self, str filename):
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

  def __next__(self):
    """Load the next vaules of the ntuple [Returns true if there is an event]"""
    return self.c_reader.next()

  def getDictionary(self):
    """Get dictionary string from hipo file [More useful to use getjson]"""
    return self.c_reader.getDictionary()

  cpdef string getjson(self):
    """Get dictionary as a json object"""
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

  def getIntNode(self, str group, str item):
    """Create a hipo::node<int> which is accesible to python"""
    cdef node[int]*c_node
    c_group = str_to_char(group)
    c_item = str_to_char(item)
    c_node = self.c_reader.getBranch[int](c_group,c_item)
    py_node = int_node()
    py_node.setup(c_node)
    return py_node

  def getByteNode(self, str group, str item):
    """Create a hipo::node<char> which is accesible to python"""
    cdef node[char]*c_node
    c_group = str_to_char(group)
    c_item = str_to_char(item)
    c_node = self.c_reader.getBranch[char](c_group,c_item)
    py_node = char_node()
    py_node.setup(c_node)
    return py_node

  def getFloatNode(self, str group, str item):
    """Create a hipo::node<float> which is accesible to python"""
    cdef node[float]*c_node
    c_group = str_to_char(group)
    c_item = str_to_char(item)
    c_node = self.c_reader.getBranch[float](c_group,c_item)
    py_node = float_node()
    py_node.setup(c_node)
    return py_node
