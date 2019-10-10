#!/bin/bash
#
# buildtst.sh
# 
# Build test for the Paho C++ library.
#
# This test the build with a few compilers on Linux. It does a build using 
# CMake, for the library, tests, and examples, then runs the unit tests.
# This is repeated for each of the compilers in the list. If a particular 
# compiler is not installed on the system, it is just skipped.
#
# This is not meant to replace Travis or other CI on the repo server, but
# is a quick test to use locally during development.
#

COMPILERS="g++-5 g++-6 g++-7 g++-8 clang++-3.9 clang++-4.0 clang++-5.0 clang++-6.0 clang++-7 clang++-8"
[ "$#" -gt 0 ] && COMPILERS="$@"

[ -z "${BUILD_JOBS}" ] && BUILD_JOBS=4
[ -n "${PAHO_C_PATH}" ] && PAHO_C_SWITCH="-DCMAKE_PREFIX_PATH=${PAHO_C_PATH}"

for COMPILER in $COMPILERS; do
    if [ -z "$(which ${COMPILER})" ]; then
        printf "Compiler not found: %s\n" "${COMPILER}"
    else
        printf "===== Testing: %s =====\n\n" "${COMPILER}"
        rm -rf buildtst-build/
        mkdir buildtst-build ; pushd buildtst-build &> /dev/null

        if ! cmake -DCMAKE_CXX_COMPILER=${COMPILER} -DPAHO_WITH_SSL=ON -DPAHO_BUILD_SAMPLES=ON -DPAHO_BUILD_TESTS=ON ${PAHO_C_SWITCH} .. ; then
            printf "\nCMake configuration failed for %s\n" "${COMPILER}"
            exit 1
        fi

        if ! make -j${BUILD_JOBS} ; then
            printf "\nCompilation failed for %s\n" "${COMPILER}"
            exit 2
        fi

        printf "Running Catch2 Unit tests for %s:\n" "${COMPILER}"
        if ! ./test/unit/unit_tests ; then
            printf "\nCatch2 unit test failed for %s\n" "${COMPILER}"
            exit 3
        fi

        printf "Running CppUnit tests for %s:\n" "${COMPILER}"
        if ! ./test/cppunit/paho-mqttpp-test ; then
            printf "\nUnit test failed for %s\n" "${COMPILER}"
            exit 4
        fi

        popd &> /dev/null
    fi
    printf "\n"
done

rm -rf buildtst-build/
printf "\nAll builds completed successfully\n\n"

if ! cppcheck --enable=all --std=c++11 --force --quiet src/*.cpp ; then
    printf "\ncppcheck failed\n"
    exit 5
fi

printf "\n===== All tests completed successfully =====\n\n"
exit 0
