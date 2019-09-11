#!/bin/bash

rm -rf build-qmake-tests
mkdir  build-qmake-tests
cd     build-qmake-tests
qmake ../qmake_tests.pro || exit 1
make -j6 || exit 1

../list_of_tests.sh || exit 1

cd ..

echo
echo
echo ====================================================================================
echo  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
echo ====================================================================================
echo "=         Remove qmake tests...                                                  ="
rm -rf build-qmake-tests
echo "=         qmake OK                                                               ="
echo ====================================================================================
echo  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
echo ====================================================================================
echo
exit 0
