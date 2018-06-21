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
