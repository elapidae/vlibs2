TEMPLATE = subdirs

#   Столь странное подключение проектов сделано, чтобы писать имя микромодуля только раз.
#   Копипастите последнюю пару строк и меняйте PROJECT = <microname>
#   Если были соблюдены все правила именования, тест должен подцепиться.

PROJECT = vcat
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vgit
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vcompiler_traits
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vsignal
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vchrono
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vlog
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vbyte_buffer
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vposix
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vtimer
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vnetwork
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vserial_port
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vsettings
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vcan_socket
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vprofile
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}

PROJECT = vcmdline_parser
SUBDIRS += vlibs/$${PROJECT}/test_$${PROJECT}
