#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then

  if [ "$COMPILER" == "" ]; then
    COMPILER=g++;
  fi

  echo "travis build dir $TRAVIS_BUILD_DIR pwd $PWD"

  rm -rf build/
  cmake -Bbuild -H. -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_WITH_SSL=OFF
  cmake --build build/
  pushd build
  sudo make install
  popd

  # Run the unit tests
  ./build/tests/unit/unit_tests    #--success

  #ctest -VV --timeout 600
  #cpack --verbose

  rm -rf build/
  cmake -Bbuild -H. -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_WITH_SSL=ON
  cmake --build build/
  pushd build
  sudo make install
  popd

fi
