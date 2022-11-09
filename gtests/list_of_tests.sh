#!/bin/bash

echo ====================================================================================
PROJECT=vcat
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vgit
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vcompiler_traits
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vsignal
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vchrono
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vlog
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vbyte_buffer
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vposix
#test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vtimer
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vnetwork
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vserial_port
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vsettings
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vcan_socket
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vprofile
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================
PROJECT=vcmdline_parser
test_$PROJECT/test_$PROJECT      || exit 1

echo ====================================================================================


exit 0
