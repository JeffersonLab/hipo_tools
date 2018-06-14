#=================================================
# The SCONSTRUCT file for building HIPO project.
# 
#=================================================
import glob
import os
import sys
#=================================================
# LOADING THE ENVIRONMENT
#=================================================
env = Environment(CPPPATH=["include",".","/usr/include","/usr/local/include","/opt/local/include","/group/clas12/packages/lz4/lib","libcpp"])
env.Append(ENV = os.environ)
env.Append(CPPPATH=["src/root","src/evio"])
env.Append(CCFLAGS=["-O2","-fPIC","-m64","-fmessage-length=0","-g"])
env.Append(LIBPATH=["/opt/local/lib","/usr/lib","/usr/local/lib","/group/clas12/packages/lz4/lib","lib"])
env.Append(CONFIGUREDIR=["/group/clas12/packages/lz4/lib"])
#=================================================
# Check for compression libraries.
#=================================================
conf = Configure(env)

if conf.CheckLib('liblz4'):
   print '\n\033[32m[**] >>>>> found library : LZ4'
   print '[**] >>>>> enabling lz4 compression. \033[0m'
   print ''
   env.Append(CCFLAGS="-D__LZ4__")
    
if conf.CheckLib('libz'):
   print '\n\033[32m[**] >>>>> found library : libz'
   print '[**] >>>>> enabling gzip compression. \033[0m'
   print ''
   env.Append(CCFLAGS="-D__LIBZ__")
#=================================================
# 
#=================================================
codeList_CPP  = glob.glob('libcpp/*.cpp')
lib   = env.Library(target="lib/libhipo.a",source = codeList_CPP)
#==================================================
# Building example codes
#==================================================
read_file   = env.Program(target="examples/read_file",source=codeList_CPP+["examples/read_file.cpp"])
read_hipo   = env.Program(target="bin/hipo",source=codeList_CPP+["examples/hipo.cpp"])
read_file_e = env.Program(target="examples/read_event_file",source=codeList_CPP+["examples/read_event_file.cpp"])
read_file_a = env.Program(target="examples/read_file_advanced",source=codeList_CPP+["examples/read_file_advanced.cpp"])
benchmark   = env.Program(target="bin/benchmark",source=codeList_CPP+["examples/benchmark.cpp"])
#write_file  = env.Program(target="examples/write_file",source=codeList_CPP+["examples/write_file.cpp"])
#pulse_comp  = env.Program(target="examples/pulse_compression",source=codeList_CPP+["examples/pulse_compression.cpp"])

env.Alias('lib',lib)
