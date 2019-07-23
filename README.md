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
First choose where to install hipo_tools.

For bash/zsh:
```
export HIPO_TOOLS=/path/to/hipo_tools
```
or for csh/tcsh:
```
setenv HIPO_TOOLS /path/to/hipo_tools
```

Instructions are for Linux, should work on MacOS as well. Must have ROOT installed as prerequisit.
```
git clone --recurse-submodules https://github.com/JeffersonLab/hipo_tools.git
cd hipo_tools && mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$HIPO_TOOLS
make && make install
```

### Setting up the environment

Once installed add this to the end of your .bashrc/.zshrc to get hipo_tools to work:

```
#Hipo_tools
export HIPO_TOOLS=/path/to/hipo_tools
export PATH=$PATH:$HIPO_TOOLS/bin
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$HIPO_TOOLS/share/pkgconfig
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HIPO_TOOLS/lib
export PYTHONPATH=$PYTHONPATH:$HIPO_TOOLS/lib
```
Or for csh/tcsh add:
```
#Hipo_tools
setenv HIPO_TOOLS /path/to/hipo_tools
setenv PATH $PATH:$HIPO_TOOLS/bin
setenv PKG_CONFIG_PATH $PKG_CONFIG_PATH:$HIPO_TOOLS/share/pkgconfig
setenv LD_LIBRARY_PATH $LD_LIBRARY_PATH:$HIPO_TOOLS/lib
setenv PYTHONPATH $PYTHONPATH:$HIPO_TOOLS/lib
```

### Some tips

* Use the latest version of root with the latest compiler (e.g. gcc8)
* Use **C++17** because you have no reason not to!
* Compile root with "-Droot7:bool=ON -Dcxx17:BOOL=ON"
* Don't write loops! Use [RDataFrame](https://root.cern.ch/doc/master/group__tutorial__dataframe.html)


Reading hipo files in python
---------------------
### hipopy

Easily read files in python using hipopy, which is build automatically if Cython is found.
An example of reading a hipo4 file in python can be found [here](examples/hipo4/clas12_event.py).


Converting hipo files
---------------------

### hipo2root

```
$ hipo2root -h
SYNOPSIS
        hipo2root [-h] [-mc] [-b] [-c] [-v] [-t] [-tbt] [-test] <inputFile.hipo> [<outputFile.root>]

OPTIONS
        -h, --help  print help
        -mc, --MC   Convert dst and mc banks
        -b, --batch Don't show progress and statistics
        -c, --cov   Save Covariant Matrix for kinematic fitting

        -v, --VertDoca
                    Save VertDoca information

        -t, --traj  Save traj information
        -tbt, --tbt Save TimeBasedTrkg information

        -test, --test
                    Only convert first 50000 events for testing
```

### dst2root

A clas12 specific converter to extract the DST banks from a hipo file and
put them into a root file. Useful for analysis especially if downloading the
files to an offsite location or a personal computer.

```
$ dst2root -h
SYNOPSIS
        src/dst2root/dst2root [-h] [-mc] [-b] [-r] [-e] [-c] [-v] [-t] [-test] [-m <max_size>] <inputFile.hipo> [<outputFile.root>]

OPTIONS
        -h, --help  print help
        -mc, --MC   Convert dst and mc banks
        -b, --batch Don't show progress and statistics
        -r, --rec   Only save events where number of partilces in the event > 0
        -e, --elec  Only save events with good electron as first particle
        -c, --cov   Save Covariant Matrix for kinematic fitting

        -v, --VertDoca
                    Save VertDoca information

        -t, --traj  Save traj information

        -test, --test
                    Testing

        <max_size>  Max file size in GB (150GB default)
```

The contents of the file have been re-ordered from the original file and places
into banks specific to each detector system. This is done to reduce the amount
of looping increase the speed for the end user. There are also a few banks
which are added for connivence to the user such as the total number of
photoelectrons for a particle, the total energy deposited for a particle, as
well as the magnitude of the momentum and the momentum squared.

For particles with missing banks, due to the particle not being detected by that detector/layer/etc.
then the information in the bank is filled with a flag value. For integers the flag value is set to
be -1 and for floats the flag value is set to be std::nan.

Here are a few examples of equivalent java and c++ code to complete the same tasks:

##### Sampling Fraction
```C++
// C++ example filling sampling fraction histogram
sf_hist->Fill(p->at(ipart), ec_tot_energy->at(ipart) / p->at(ipart));
```

```Java
// Java example to get get EC energy:
double energy=0;
for (int icalo=0; icalo<calos.getMaxSize(); icalo++) {
    if (calos.getNode("pindex").getShort(icalo) == ipart) {
          energy += calos.getNode("energy").getFloat(icalo);
    }
}
// and then fill the sampling fraction histogram
if (energy>0) {
    final float px   = parts.getNode("px").getFloat(ipart);
    final float py   = parts.getNode("py").getFloat(ipart);
    final float pz   = parts.getNode("pz").getFloat(ipart);
    final float p = Math.sqrt(px*px+py*py+pz*pz);
    hist.fill(p, energy/p);
}
```

##### FTOF time and path
```C++
// get FTOF path length and time
float time = sc_ftof_1b_time->at(ipart);
float path = sc_ftof_1b_path->at(ipart);
```

```Java
// get FTOF path length and time
for (int iscin=0; iscin<scins.getNode("pindex").getShort().size(); iscin++) {
    if (scins.getNode("pindex").getShort(iscin) == ipart) {
        if (scins.getNode("detector").getByte(iscin) == 12) {
            final float path = scins.getNode("path").getFloat(iscin);
            final float time = scins.getNode("time").getFloat(iscin);
            break;
        }
    }
}
```
For more detailed examples of using dst2root converted files checkout the more detailed [examples](examples/dst2root).

[java examples](https://userweb.jlab.org/~gavalian/docs/sphinx/hipo/html/chapters/java_groovy_analysis.html#ec-sampling-fraction)


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
│   ├── hipo2root
│   └── hipo2root_mc
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
