cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.11.40")

project(KTnef VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.59.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(ECMQtDeclareLoggingCategory)




set(KTNEF_LIB_VERSION ${PIM_VERSION})
set(CALENDARCORE_LIB_VERSION "5.11.44")
set(CALENDARUTILS_LIB_VERSION "5.11.40")
set(KCONTACTS_LIB_VERSION "5.11.40")
set(QT_REQUIRED_VERSION "5.11.0")
ecm_setup_version(PROJECT VARIABLE_PREFIX KTNEF
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ktnef_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5TnefConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Tnef")

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets)
find_package(KF5CalendarCore ${CALENDARCORE_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarUtils ${CALENDARUTILS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KCONTACTS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x060000)
add_definitions(-DQT_NO_FOREACH)

configure_package_config_file(
  "${KTnef_SOURCE_DIR}/KF5TnefConfig.cmake.in"
  "${KTnef_BINARY_DIR}/KF5TnefConfig.cmake"
  INSTALL_DESTINATION  ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
  "${KTnef_BINARY_DIR}/KF5TnefConfig.cmake"
  "${KTnef_BINARY_DIR}/KF5TnefConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
  COMPONENT Devel
)

install(EXPORT KF5TnefTargets
        DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
        FILE KF5TnefTargets.cmake
        NAMESPACE KF5::
)

install(FILES
   ${CMAKE_CURRENT_BINARY_DIR}/ktnef_version.h
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF5}
   COMPONENT Devel
)


add_definitions(-DTRANSLATION_DOMAIN=\"libktnef5\")

add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

install(FILES ktnef.renamecategories ktnef.categories  DESTINATION  ${KDE_INSTALL_LOGGINGCATEGORIESDIR})

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
