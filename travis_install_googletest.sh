#!/bin/bash
#
# Installs the Google Test.
#

wget https://github.com/google/googletest/archive/release-1.7.0.tar.gz
tar xf release-1.7.0.tar.gz
pushd googletest-release-1.7.0
if [ "$COMPILER" == "" ]; then COMPILER=g++; fi && cmake -DCMAKE_CXX_COMPILER=$COMPILER -DBUILD_SHARED_LIBS=ON .
make
sudo cp -a include/gtest /usr/include
sudo cp -a libgtest_main.so libgtest.so /usr/lib/
popd

exit 0

