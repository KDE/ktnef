remove_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

set(ktnef_SRCS
  ktnefparser.cpp
  ktnefproperty.cpp
  ktnefattach.cpp
  mapi.cpp
  ktnefpropertyset.cpp
  ktnefmessage.cpp
  ktnefwriter.cpp
  lzfu.cpp
  formatter.cpp
)

add_library(KF5KTnef ${ktnef_SRCS})

generate_export_header(KF5KTnef BASE_NAME ktnef)

add_library(KF5::KTnef ALIAS KF5KTnef)

target_include_directories(KF5KTnef INTERFACE "$<INSTALL_INTERFACE:${INCLUDE_INSTALL_DIR}/ktnef;${Boost_INCLUDE_DIR}>")
target_include_directories(KF5KTnef PUBLIC "$<BUILD_INTERFACE:${KTnef_SOURCE_DIR}/src;${KTnef_BINARY_DIR}/src>")

target_link_libraries(KF5KTnef
  KF5::CalendarCore
  KF5::CalendarUtils
  KF5::KAbc
  KF5::KDE4Support
  KF5::KIOCore
  KF5::PimUtils
)

set_target_properties(KF5KTnef PROPERTIES
    VERSION ${KTNEF_VERSION_STRING}
    SOVERSION ${KTNEF_SOVERSION}
    EXPORT_NAME KTnef
)

install(TARGETS KF5KTnef EXPORT KF5KTnefTargets ${INSTALL_TARGETS_DEFAULT_ARGS})

ecm_generate_headers(KTnef_CamelCase_HEADERS
  HEADER_NAMES
  Formatter
  KTNEFAttach
  KTNEFDefs
  KTNEFMessage
  KTNEFParser
  KTNEFProperty
  KTNEFPropertySet
  KTNEFWriter

  REQUIRED_HEADERS KTnef_HEADERS
  PREFIX ktnef
)

install(FILES
    ${KTnef_CamelCase_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/KTNEF
    COMPONENT Devel
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ktnef_export.h
    ${KTnef_HEADERS}
    DESTINATION ${INCLUDE_INSTALL_DIR}/ktnef COMPONENT Devel
)

#ecm_generate_pri_file(BASE_NAME KTNef LIB_NAME KF5TNef DEPS "CoreAddons KMime" FILENAME_VAR PRI_FILENAME)
#install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})