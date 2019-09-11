#!/bin/bash

./cmake_tests.sh || exit 1
./qmake_tests.sh || exit 1

exit 0
