cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

project(kregexpeditor)

############### search packages used by KDE ###############


set(KF_MIN_VERSION "6.0.0")
set(QT_MIN_VERSION "6.5")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(ECMGenerateHeaders)
include(ECMGenerateExportHeader)
include(ECMInstallIcons)
include(ECMAddAppIcon)
include(ECMMarkNonGuiExecutable)
include(ECMOptionalAddSubdirectory)
include(ECMSetupVersion)
include(FeatureSummary)
include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(KDEClangFormat)
include(KDEGitCommitHooks)
include(ECMDeprecationSettings)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets Xml)
find_package(KF6 ${KF_MIN_VERSION} REQUIRED
    I18n
    TextWidgets
    CoreAddons
    ConfigWidgets
    DocTools
    IconThemes
    WidgetsAddons
    Crash
    XmlGui
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

############### Find the stuff we need ###############
ecm_set_disabled_deprecation_versions(QT 6.3
    KF 6.1.0
    )

add_subdirectory(src)

ki18n_install(po)
if(KF6DocTools_FOUND)
    kdoctools_install(po)
    add_subdirectory(doc)
endif()


feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
