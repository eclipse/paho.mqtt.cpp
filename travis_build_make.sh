#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then

  if [ "$COMPILER" == "" ]; then
    COMPILER=g++;
  fi

  make CXX=$COMPILER VERBOSE=1
  sudo make install

  make CXX=$COMPILER VERBOSE=1 check

  make clean
  sudo make uninstall

  pushd test/unit
  make clean
  popd

  make CXX=$COMPILER VERBOSE=1 SSL=0
  sudo make install

  make CXX=$COMPILER VERBOSE=1 SSL=0 check

  make clean
  sudo make uninstall

  pushd test/unit
  make clean
  popd

fi
