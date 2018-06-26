hipo-tools
==========

"Official" C++ based analysis tools for hipo files.

[Hipo-file 
Documentation](https://userweb.jlab.org/~gavalian/docs/sphinx/hipo/html/index.html) 
(external project)

This code was adopted from source code on the jlab CUE 
`/group/clas12/packages/hipo-io`.

Installing
----------

**Note**: These instructions are for Linux. For MacOS, refer to the next subsection.
```
git clone https://github.com/JeffersonLab/hipo_tools.git
cd hipo_tools && mkdir build
cd build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME
make && make install
```

### Installing on MacOS

For some reason XCode does not currently ship with the necessary C++17
filesystem libraries (even though they have been available in LLVM for several
versions). As temporary workaround, we suggest to install llvm through Homebrew:
```
brew install llvm
```
Homebrew will skip the linking step when you install llvm (as to not screw up
your system), so you will have to tell cmake to use the newer version of
llvm/clang. The installation instructions for MacOS are:
```
git clone https://github.com/JeffersonLab/hipo_tools.git
cd hipo_tools && mkdir build
cd build
CXX=/usr/local/opt/llvm/bin/clang++ cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME
make && make install
```

### Some tips

* Use the latest version of root with the latest compiler (e.g. gcc8)
* Use **C++17** because you have no reason not to!
* Compile root with "-Droot7:bool=ON -Dcxx17:BOOL=ON"
* Don't write loops! Use [RDataFrame](https://root.cern.ch/doc/master/group__tutorial__dataframe.html)


Converting hipo files
---------------------

### toohip4root

```
$ toohip4root -h
SYNOPSIS
        toohip4root (<input file> | [-i <input file>]) [-o <output file>] [-f] [-v] [-N <N events>] [-h]

OPTIONS
        -f, --force force the output to overwrite existing files

        -v, --verbose
                    turn on verbose output

        -h, --help  print help

```

Todo
----

* Isolate json parser from converter
* Create clas12 specific helpers (maybe different repo for clas12 specific json?)
* Setup CI with test data.
* Debug build on mac
* Test library in external cmake and pkgconfig projects


### Install structure:

```
.
├── bin
│   ├── hipo
│   ├── hipo2root
│   └── toohip4root
├── include
│   └── hipo
│       ├── data.h
│       ├── dictionary.h
│       ├── event.h
│       ├── hipoexceptions.h
│       ├── node.h
│       ├── reader.h
│       ├── record.h
│       ├── text.h
│       ├── THipo.h
│       ├── utils.h
│       └── writer.h
├── lib
│   ├── hipo
│   │   ├── HipoToolsConfig.cmake
│   │   ├── HipoToolsConfigVersion.cmake
│   │   ├── HipoToolsTargets.cmake
│   │   └── HipoToolsTargets-noconfig.cmake
│   ├── libhipocpp.so -> libhipocpp.so.0.0.1
│   ├── libhipocpp.so.0.0.1
│   └── libhiporoot.so
└── share
    └── pkgconfig
        └── hipocpp.pc
```

