cmake_minimum_required(VERSION 3.5)
set(PIM_VERSION "5.16.2")

project(KTnef VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.75.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(ECMQtDeclareLoggingCategory)




set(KTNEF_LIB_VERSION ${PIM_VERSION})
set(CALENDARUTILS_LIB_VERSION "5.16.1")
set(QT_REQUIRED_VERSION "5.13.0")
ecm_setup_version(PROJECT VARIABLE_PREFIX KTNEF
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ktnef_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5TnefConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Tnef")

find_package(Qt5 ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets)
find_package(KF5CalendarCore ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarUtils ${CALENDARUTILS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)

add_definitions(-DQT_DISABLE_DEPRECATED_BEFORE=0x050f00)
add_definitions(-DKF_DISABLE_DEPRECATED_BEFORE_AND_AT=0x054C00)

add_definitions(-DQT_NO_FOREACH)
add_definitions(-DQT_NO_KEYWORDS)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
