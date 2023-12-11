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

find_package(Qt6 ${QT5_REQUIRED_VERSION} CONFIG REQUIRED Core Widgets Xml)
find_package(KF6 REQUIRED
    CoreAddons
    ConfigWidgets
    DocTools
    IconThemes
    Service
    TextWidgets
    WidgetsAddons
    Crash
)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

############### Find the stuff we need ###############

add_subdirectory(src)
add_subdirectory( doc )

add_subdirectory(tests)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
