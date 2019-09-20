#!/bin/bash

echo ====================================================================================
vlibs/vcat/test_vcat/test_vcat                                              || exit 1
echo ====================================================================================
vlibs/vgit/test_vgit/test_vgit                                              || exit 1
echo ====================================================================================
vlibs/vcompiler_traits/test_vcompiler_traits/test_vcompiler_traits          || exit 1
echo ====================================================================================
#vlibs/vgio/keyfile/test_keyfile/test_keyfile            || exit 1
#echo ====================================================================================
#vlibs/vcrc/test_vcrc/test_vcrc                          || exit 1
#echo ====================================================================================
#vlibs/vstring/test_vstring/test_vstring                 || exit 1
#echo ====================================================================================
#vlibs/vgeometry/test_vgeometry/test_vgeometry           || exit 1
#echo ====================================================================================
#vlibs/vapplication/test_vapplication/test_vapplication  || exit 1
#echo ====================================================================================
#vlibs/vhdlc/test_vhdlc/test_vhdlc                       || exit 1
#echo ====================================================================================
#vlibs/vchrono/test_vchrono/test_vchrono                 || exit 1
#echo ====================================================================================
#vlibs/vlog/test_vlog/test_vlog                          || exit 1
#echo ====================================================================================

exit 0
