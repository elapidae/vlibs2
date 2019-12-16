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
PROJECT=vtimer
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vnetwork
vlibs/$PROJECT/test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================



exit 0
