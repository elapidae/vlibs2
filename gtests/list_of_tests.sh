#!/bin/bash

echo ====================================================================================
PROJECT=vcat
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vgit
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vcompiler_traits
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vsignal
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vchrono
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vlog
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vbyte_buffer
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vposix
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vthread
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================



#vlibs/vgio/keyfile/test_keyfile/test_keyfile            || exit 1
#echo ====================================================================================
#vlibs/vcrc/test_vcrc/test_vcrc                          || exit 1
#echo ====================================================================================
#vlibs/vgeometry/test_vgeometry/test_vgeometry           || exit 1
#echo ====================================================================================
#vlibs/vapplication/test_vapplication/test_vapplication  || exit 1
#echo ====================================================================================
#vlibs/vhdlc/test_vhdlc/test_vhdlc                       || exit 1
#echo ====================================================================================

exit 0
