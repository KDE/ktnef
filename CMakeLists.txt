cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
set(PIM_VERSION "5.21.2")

project(KTnef VERSION ${PIM_VERSION})

set(KF5_MIN_VERSION "5.95.0")

find_package(ECM ${KF5_MIN_VERSION} CONFIG REQUIRED)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH})

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)

include(GenerateExportHeader)
include(ECMGenerateHeaders)
include(ECMGeneratePriFile)

include(ECMSetupVersion)
include(FeatureSummary)
include(KDEGitCommitHooks)
include(KDEClangFormat)
file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES *.cpp *.h *.c)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})
include(ECMQtDeclareLoggingCategory)
include(ECMDeprecationSettings)




set(KTNEF_LIB_VERSION ${PIM_VERSION})
set(CALENDARUTILS_LIB_VERSION "5.21.1")
set(QT_REQUIRED_VERSION "5.15.2")
ecm_setup_version(PROJECT VARIABLE_PREFIX KTNEF
                        VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/ktnef_version.h"
                        PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5TnefConfigVersion.cmake"
                        SOVERSION 5
)

########### Find packages ###########
set(CMAKECONFIG_INSTALL_DIR "${KDE_INSTALL_CMAKEPACKAGEDIR}/KF5Tnef")

find_package(Qt${QT_MAJOR_VERSION} ${QT_REQUIRED_VERSION} CONFIG REQUIRED Widgets)
find_package(KF5CalendarCore ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5CalendarUtils ${CALENDARUTILS_LIB_VERSION} CONFIG REQUIRED)
find_package(KF5Contacts ${KF5_MIN_VERSION} CONFIG REQUIRED)
find_package(KF5I18n ${KF5_MIN_VERSION} CONFIG REQUIRED)

ecm_set_disabled_deprecation_versions(QT 5.15.2 KF 5.96.0)



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
   DESTINATION ${KDE_INSTALL_INCLUDEDIR_KF}/KTNEF/
   COMPONENT Devel
)


add_definitions(-DTRANSLATION_DOMAIN=\"libktnef5\")
option(USE_UNITY_CMAKE_SUPPORT "Use UNITY cmake support (speedup compile time)" OFF)

set(COMPILE_WITH_UNITY_CMAKE_SUPPORT OFF)
if (USE_UNITY_CMAKE_SUPPORT)
    set(COMPILE_WITH_UNITY_CMAKE_SUPPORT ON)
endif()
add_subdirectory(src)

if(BUILD_TESTING)
    add_subdirectory(autotests)
endif()

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
ki18n_install(po)
feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
