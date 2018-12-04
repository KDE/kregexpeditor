project(kregexpeditor)

############### search packages used by KDE ###############

cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)

find_package(ECM 5.20 REQUIRED NO_MODULE)
set(QT5_REQUIRED_VERSION "5.5.0")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

option(ENABLE_KTEXTWIDGETS "Build KTextWidgets integration" OFF)
option(BUILD_TEST "Build test widget" OFF)

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

find_package(Qt5 ${QT5_REQUIRED_VERSION} CONFIG REQUIRED Core Widgets Xml)
find_package(KF5 REQUIRED
    I18n
    DocTools
    WidgetsAddons
    Crash
    CoreAddons
    )

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)


include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

############### Find the stuff we need ###############

add_definitions( -DQT_NO_CAST_ASCII )
add_definitions( -DQT_DISABLE_DEPRECATED_BEFORE=0x060000 )
# not for now, the code is not fully ready for it
#add_definitions( -DQT_NO_CAST_FROM_ASCII )
add_definitions( -DQT_NO_CAST_TO_ASCII )
add_definitions( -DQT_NO_NARROWING_CONVERSIONS_IN_CONNECT )

add_subdirectory(src)
add_subdirectory( doc )

if (ENABLE_KTEXTWIDGETS)
    add_definitions( -DENABLE_KTEXTWIDGETS )
    find_package(KF5 REQUIRED
        TextWidgets
    )
endif()

if (BUILD_TEST)
    find_package(KF5 REQUIRED
        KIO
    )
    add_subdirectory(tests)
endif()

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
