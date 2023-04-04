add_library(KPim6Tnef)
add_library(KPim6::Tnef ALIAS KPim6Tnef)

target_sources(KPim6Tnef PRIVATE
  ktnefparser.cpp
  ktnefproperty.cpp
  ktnefattach.cpp
  mapi.cpp
  ktnefpropertyset.cpp
  ktnefmessage.cpp
  ktnefwriter.cpp
  lzfu.cpp
  formatter.cpp
  ktnefpropertyset.h
  ktnefparser.h
  mapi.h
  ktnefattach.h
  lzfu.h
  ktnefmessage.h
  ktnefdefs.h
  formatter.h
  ktnefwriter.h
  ktnefproperty.h
)

ecm_qt_declare_logging_category(KPim6Tnef HEADER ktnef_debug.h IDENTIFIER KTNEF_LOG CATEGORY_NAME org.kde.pim.ktnef
        OLD_CATEGORY_NAMES log_ktnef
        DESCRIPTION "ktnef (pim lib)" EXPORT KTNEF)

if (COMPILE_WITH_UNITY_CMAKE_SUPPORT)
    set_target_properties(KPim6Tnef PROPERTIES UNITY_BUILD ON)
endif()
generate_export_header(KPim6Tnef BASE_NAME ktnef)



target_include_directories(KPim6Tnef INTERFACE "$<INSTALL_INTERFACE:${KDE_INSTALL_INCLUDEDIR}/KPim6/KTNEF>")
target_include_directories(KPim6Tnef PUBLIC "$<BUILD_INTERFACE:${KTnef_SOURCE_DIR}/src;${KTnef_BINARY_DIR}/src>")

target_link_libraries(KPim6Tnef
PUBLIC
  KF6::CalendarCore
PRIVATE
  KPim6::CalendarUtils
  KF6::Contacts
  KF6::I18n
)

set_target_properties(KPim6Tnef PROPERTIES
    VERSION ${KTNEF_VERSION}
    SOVERSION ${KTNEF_SOVERSION}
    EXPORT_NAME Tnef
)

install(TARGETS KPim6Tnef EXPORT KPim6TnefTargets ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

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

install(FILES ${KTnef_CamelCase_HEADERS} DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KTNEF/KTNEF/ COMPONENT Devel)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ktnef_export.h
    ${KTnef_HEADERS}
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}/KPim6/KTNEF/ktnef
    COMPONENT Devel
)

ecm_generate_pri_file(BASE_NAME KTNef LIB_NAME KPim6Tnef DEPS "KCalendarCore"
    FILENAME_VAR PRI_FILENAME INCLUDE_INSTALL_DIR ${KDE_INSTALL_INCLUDEDIR}/KPim6/KTNEF)
install(FILES ${PRI_FILENAME} DESTINATION ${ECM_MKSPECS_INSTALL_DIR})
ecm_qt_install_logging_categories(EXPORT KTNEF FILE ktnef.categories DESTINATION ${KDE_INSTALL_LOGGINGCATEGORIESDIR})


if (BUILD_QCH)
    ecm_add_qch(
        KPim6Tnef_QCH
        NAME KTnef
        BASE_NAME KPim6Tnef
        VERSION ${PIM_VERSION}
        ORG_DOMAIN org.kde
        SOURCES # using only public headers, to cover only public API
            ${KTnef_HEADERS}
        #MD_MAINPAGE "${CMAKE_SOURCE_DIR}/README.md"
        #IMAGE_DIRS "${CMAKE_SOURCE_DIR}/docs/pics"
        LINK_QCHS
            Qt6Core_QCH
        INCLUDE_DIRS
            ${CMAKE_CURRENT_BINARY_DIR}
        BLANK_MACROS
            KTNEF_EXPORT
        TAGFILE_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        QCH_INSTALL_DESTINATION ${KDE_INSTALL_QTQCHDIR}
        COMPONENT Devel
    )
endif()
