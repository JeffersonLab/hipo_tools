#!/bin/bash

if [[ $TRAVIS_OS_NAME == 'osx' ]]; then
  brew tap homebrew/science
  brew install root6
  brew install llvm
  cd hipo_tools
  mkdir build && cd build
  CXX=/usr/local/opt/llvm/bin/clang++ cmake .. -DCMAKE_INSTALL_PREFIX=install
  make
  make install
else
    echo "Wrong os"
fi
