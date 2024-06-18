#!/bin/bash
#
# Runs clang format over the whole project tree
#

find . -iname '*.h' -o -iname '*.cpp' | xargs clang-format -i

