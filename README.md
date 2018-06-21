hipo-tools
==========

"Official" C++ based analysis tools for hipo files.

[Hipo Documentation](https://userweb.jlab.org/~gavalian/docs/sphinx/hipo/html/index.html)

This code was adopted from source code on the jlab CUE 
`/group/clas12/packages/hipo-io`.

Installing
----------

```
git clone ...
cd hipo-tools && mkdir build
cd build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME
make && make install
```

### Some tips

* Use the latest version of root with the latest compiler (e.g. gcc8)
* Use **C++17** because you have no reason not to!
* Compile root with "-Droot7:bool=ON -Dcxx17:BOOL=ON"
* Don't write loops! Use [RDataFrame](https://root.cern.ch/doc/master/group__tutorial__dataframe.html)


### Install structure:

```
.
├── bin
│   ├── hipo
│   └── toohipforroot
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
│   ├── libhipocpp.so -> libhipocpp.so.0.0.1
│   ├── libhipocpp.so.0.0.1
│   └── libhiporoot.so
└── share
    └── pkgconfig
        └── hipocpp.pc
```

Converting hipo files
---------------------




