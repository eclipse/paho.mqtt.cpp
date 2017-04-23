#!/bin/bash
#
# buildtst.sh for the Paho C++ library.
#
# Test the build with a few compilers on Linux.
#
# This does a build against the GNU Make, and runs the unit tests for each
# of the compilers in the list. If a particular compiler is not installed on
# the system, it is just skipped.
#
# This is not meant to replace Travis or other CI on the repo server, but
# is rather meant to be a quick test to use during development.
#

COMPILERS="g++-4.8 g++-4.9 g++-5 g++-6 clang++-3.6 clang++-3.8"
[ "$#" -gt 0 ] && COMPILERS="$@"

[ -z "${BUILD_JOBS}" ] && BUILD_JOBS=4

for COMPILER in $COMPILERS
do
    if [ -z "$(which ${COMPILER})" ]; then
        printf "Compiler not found: %s\n" "${COMPILER}"
    else
        printf "===== Testing: %s =====\n\n" "${COMPILER}"
        make distclean
        if ! make CXX=${COMPILER} SSL=1 -j${BUILD_JOBS} ; then
            printf "\nCompilation failed for %s\n" "${COMPILER}"
            exit 1
        fi
        pushd test/unit &> /dev/null
        make clean
        if ! make CXX=${COMPILER} SSL=1 ; then
            printf "\nUnit test compilation failed for %s\n" "${COMPILER}"
            exit 2
        fi
        rm -rf tmp/*
	printf "Running unit tests:\n"
        if ! ./mqttpp-unittest ; then
            printf "\nUnit test failed for  %s\n" "${COMPILER}"
            exit 3
        fi
        make clean
        popd &> /dev/null
    fi
    printf "\n"
done

make distclean
printf "\n===== All tests completed successfully =====\n\n"

exit 0


