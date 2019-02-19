hipo-tools
==========

[![Build Status](https://travis-ci.com/JeffersonLab/hipo_tools.svg?branch=master)](https://travis-ci.com/JeffersonLab/hipo_tools)

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

### dst2root

A clas12 specific converter to extract the DST banks from a hipo file and
put them into a root file. Useful for analysis especially if downloading the
files to an offsite location or a personal computer.

```
$ dst2root -h
SYNOPSIS
        dst2root [-h] [-mc] [-b] [-r] [-e] [-c] <inputFile.hipo> [<outputFile.root>]

OPTIONS
        -h, --help  print help
        -mc, --MC   Convert dst and mc banks
        -b, --batch Don't show progress and statistics
        -r, --rec   Only save events where number of partilces in the event > 0
        -e, --elec  Only save events with good electron as first particle
        -c, --cov   Save Covariant Matrix for kinematic fitting
```


Todo
----

- [ ] Isolate json parser from converter
- [ ] Create clas12 specific helpers (maybe different repo for clas12 specific json?)
- [ ] Setup CI with test data.
- [x] Debug build on mac (#7)
- [ ] Test library in external cmake and pkgconfig projects


### Install structure:

```
.
├── bin
│   ├── dst2root
│   ├── dst2root_3
│   ├── dst2root_examples
│   │   ├── MonteCarlo
│   │   ├── WvsQ2
│   │   ├── deltat
│   │   ├── pvsb
│   │   └── samplingFraction
│   ├── hipo3_examples
│   │   ├── benchmark
│   │   ├── hipo
│   │   ├── pulse_compression
│   │   ├── read_event_file
│   │   ├── read_file
│   │   ├── read_file_advanced
│   │   └── write_file
│   ├── recon2root
│   └── recon2root_mc
├── include
│   ├── hipo3
│   │   ├── data.h
│   │   ├── dictionary.h
│   │   ├── event.h
│   │   ├── hipoexceptions.h
│   │   ├── node.h
│   │   ├── reader.h
│   │   ├── record.h
│   │   ├── text.h
│   │   ├── utils.h
│   │   └── writer.h
│   └── hipo4
│       ├── bank.h
│       ├── dictionary.h
│       ├── event.h
│       ├── hipoexceptions.h
│       ├── reader.h
│       ├── record.h
│       ├── recordbuilder.h
│       └── utils.h
├── lib
│   ├── clas12.so
│   ├── hipo
│   │   ├── HipoToolsConfig.cmake
│   │   └── HipoToolsConfigVersion.cmake
│   ├── hipopy3.so
│   ├── hipopy4.so
│   ├── libhipocpp3.dylib
│   ├── libhipocpp3_static.a
│   ├── libhipocpp4.dylib
│   └── libhipocpp4_static.a
└── share
    └── pkgconfig
        ├── hipocpp3.pc
        └── hipocpp4.pc

```
