#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then

  if [ "$COMPILER" == "" ]; then
    COMPILER=g++;
  fi

  rm -rf build_cmake
  mkdir build_cmake
  pushd build_cmake
  cmake -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_WITH_SSL=OFF ..
  make
  sudo make install
  popd

  rm -rf build_cmake
  mkdir build_cmake
  pushd build_cmake
  cmake -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=ON  -DPAHO_WITH_SSL=ON  ..
  make
  sudo make install
  popd

fi
