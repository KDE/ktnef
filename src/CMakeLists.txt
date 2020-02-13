
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

if (ECM_VERSION VERSION_LESS "5.68.0")
    ecm_qt_declare_logging_category(ktnef_SRCS HEADER ktnef_debug.h IDENTIFIER KTNEF_LOG CATEGORY_NAME org.kde.pim.ktnef)
else()
    ecm_qt_declare_logging_category(ktnef_SRCS HEADER ktnef_debug.h IDENTIFIER KTNEF_LOG CATEGORY_NAME org.kde.pim.ktnef
        OLD_CATEGORY_NAMES log_ktnef
        DESCRIPTION "ktnef (pim lib)" EXPORT KTNEF)
endif()

add_library(KF5Tnef ${ktnef_SRCS})

generate_export_header(KF5Tnef BASE_NAME ktnef)

add_library(KF5::Tnef ALIAS KF5Tnef)

target_include_directories(KF5Tnef INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR_KF5}/KTNEF>")
target_include_directories(KF5Tnef PUBLIC "$<BUILD_INTERFACE:${KTnef_SOURCE_DIR}/src;${KTnef_BINARY_DIR}/src>")

target_link_libraries(KF5Tnef
PUBLIC
  KF5::CalendarCore
PRIVATE
  KF5::CalendarUtils
  KF5::Contacts
  KF5::I18n
)

set_target_properties(KF5Tnef PROPERTIES
    VERSION ${KTNEF_VERSION_STRING}
    SOVERSION ${KTNEF_SOVERSION}
    EXPORT_NAME Tnef
)

install(TARGETS KF5Tnef EXPORT KF5TnefTargets ${KF5_INSTALL_TARGETS_DEFAULT_ARGS})

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
  PREFIX KTNEF
  REQUIRED_HEADERS KTnef_HEADERS
)

install(FILES ${KTnef_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KTNEF/KTNEF/ COMPONENT Devel)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ktnef_export.h
    ${KTnef_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}/KTNEF/ktnef
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KTNef LIB_NAME KF5Tnef DEPS "CalendarCore" FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR_KF5}/KTNEF)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
if (NOT ECM_VERSION VERSION_LESS "5.68.0")
   ecm_qt_install_logging_categories(EXPORT KTNEF FILE ktnef.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})
endif()
