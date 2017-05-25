#!/bin/bash

set -e

if [ "$TRAVIS_OS_NAME" == "linux" ]; then

  if [ "$COMPILER" == "" ]; then
    COMPILER=g++;
  fi

  ./bootstrap
  rm -rf build_autotools/
  mkdir build_autotools/
  pushd build_autotools/
  ../configure CXX=$COMPILER --enable-samples=yes --enable-static=yes --enable-doc=no  --with-ssl=no
  make
  make check
  cat test-suite.log
  popd

  ./bootstrap
  rm -rf build_autotools/
  mkdir build_autotools/
  pushd build_autotools/
  ../configure CXX=$COMPILER --enable-samples=yes --enable-static=yes --enable-doc=yes --with-ssl=yes
  make
  make check
  cat test-suite.log
  popd

fi
