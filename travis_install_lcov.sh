#!/bin/bash
#
# Installs lcov.
#

cd ${TRAVIS_BUILD_DIR}
wget http://ftp.de.debian.org/debian/pool/main/l/lcov/lcov_1.13.orig.tar.gz
tar xf lcov_1.13.orig.tar.gz
pushd lcov-1.13/
sudo make install
popd

exit 0

