project(kregexpeditor)

############### search packages used by KDE ###############

cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(KF_MIN_VERSION "5.240.0")
set(QT_MIN_VERSION "6.5")

find_package(ECM ${KF_MIN_VERSION} REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

include(ECMGenerateHeaders)
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

find_package(Qt6 ${QT5_REQUIRED_VERSION} CONFIG REQUIRED Core Widgets Xml)
find_package(KF6 REQUIRED
    I18n
    TextWidgets
    CoreAddons
    ConfigWidgets
    DocTools
    IconThemes
    Service
    WidgetsAddons
    Crash
    XmlGui
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

############### Find the stuff we need ###############

add_subdirectory(src)
add_subdirectory( doc )

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)

file(GLOB_RECURSE ALL_CLANG_FORMAT_SOURCE_FILES src/*.cpp src/*.h)
kde_clang_format(${ALL_CLANG_FORMAT_SOURCE_FILES})

kde_configure_git_pre_commit_hook(CHECKS CLANG_FORMAT)
