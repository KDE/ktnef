set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include(ECMAddTests)
find_package(Qt6Test ${QT_REQUIRED_VERSION} CONFIG REQUIRED)

configure_file(config-ktnef-tests.h.in ${CMAKE_CURRENT_BINARY_DIR}/config-ktnef-tests.h)

ecm_add_tests( parsertest.cpp NAME_PREFIX "ktnetf-" LINK_LIBRARIES KPim6Tnef Qt::Test)
