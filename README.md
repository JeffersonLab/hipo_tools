hipo-io
=======

[Documentation](https://userweb.jlab.org/~gavalian/docs/sphinx/hipo/html/index.html)

This code was adopted from source code on the jlab CUE 
`/group/clas12/packages/hipo-io`. This repo exists because `hipo-io` was not 
version controlled or publicly available. Also, it now has a proper cmake 
build.

Installing
----------

```
git clone ...
cd hipo-io && mkdir build
cd build
cmake ../. -DCMAKE_INSTALL_PREFIX=$HOME
make && make install
```
