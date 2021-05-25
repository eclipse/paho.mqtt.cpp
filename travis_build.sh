#!/bin/bash

set -ex

if [ "$TRAVIS_OS_NAME" == "linux" ]; then

  if [ "$COMPILER" == "" ]; then
    COMPILER=g++;
  fi

  echo "Travis build dir: $TRAVIS_BUILD_DIR,  pwd: $PWD"

  rm -rf build/
  cmake -Bbuild -H. -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_BUILD_TESTS=ON -DPAHO_WITH_SSL=OFF
  sudo env "PATH=$PATH" cmake --build build/ --target install

  # Run the unit tests
  ./build/test/unit/unit_tests  # --success

  #ctest -VV --timeout 600
  #cpack --verbose

  rm -rf build/
  cmake -Bbuild -H. -DCMAKE_CXX_COMPILER=$COMPILER -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_DOCUMENTATION=OFF -DPAHO_BUILD_TESTS=ON -DPAHO_WITH_SSL=ON
  sudo env "PATH=$PATH" cmake --build build/ --target install

  # Run the unit tests
  ./build/test/unit/unit_tests  # --success

fi

