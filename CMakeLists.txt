project(kregexpeditor)

############### search packages used by KDE ###############

cmake_minimum_required (VERSION 2.8.12 FATAL_ERROR)

find_package(ECM 1.3.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

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


find_package(Qt5 ${QT_MIN_VERSION} CONFIG REQUIRED Core Widgets)
find_package(KF5 REQUIRED
    ConfigWidgets
    DocTools
    IconThemes
    KDELibs4Support
    KIO
    TextWidgets
    WidgetsAddons
    )

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(qregexpparser qregexpparser.y ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.cc COMPILE_FLAGS "-d -p qregexp")
FLEX_TARGET(qregexplexer qregexpparser.l  ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexplexer.cpp COMPILE_FLAGS "-Pqregexp")
ADD_FLEX_BISON_DEPENDENCY( qregexplexer qregexpparser )

include_directories(${CMAKE_CURRENT_BINARY_DIR})


############### Needed commands before building anything ###############

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} )

############### Find the stuff we need ###############

add_definitions( -DQT_NO_CAST_ASCII )

add_subdirectory( predefined )
add_subdirectory( pics )
add_subdirectory( doc )

include_directories(
    KWidgetStreamer/
    KMultiFormListBox/ )

########### next target ###############

set(kregexpeditorcommon_SRCS
   ${BISON_qregexpparser_OUTPUTS}
   ${FLEX_qregexplexer_OUTPUTS}
    altnregexp.cpp
    altnwidget.cpp
    auxbuttons.cpp
    characterswidget.cpp
    charselector.cpp
    compoundregexp.cpp
    compoundwidget.cpp
    concregexp.cpp
    concwidget.cpp
    dcbutton.cpp
    dotregexp.cpp
    dragaccepter.cpp
    regexpeditorwindow.cpp
    emacsregexpconverter.cpp
    errormap.cpp
    infopage.cpp
    KMultiFormListBox/ccp.cpp
    KMultiFormListBox/kmultiformlistboxentry.cpp
    KMultiFormListBox/kmultiformlistbox.cpp
    KMultiFormListBox/kmultiformlistbox-multivisible.cpp
    KMultiFormListBox/kmultiformlistboxfactory.cpp
    KMultiFormListBox/indexWindow.cpp
    KMultiFormListBox/kmultiformlistbox-windowed.cpp
    KMultiFormListBox/widgetwindow.cpp
    KMultiFormListBox/windowlistboxitem.cpp
    kregexpeditorgui.cpp
    kregexpeditorguidialog.cpp
    kregexpeditorprivate.cpp
    KWidgetStreamer/kwidgetstreamer.cpp
    limitedcharlineedit.cpp
    lookaheadregexp.cpp
    lookaheadwidget.cpp
    multicontainerwidget.cpp
    myfontmetrics.cpp
    positionregexp.cpp
    qtregexpconverter.cpp
    qtregexphighlighter.cpp
    regexpbuttons.cpp
    regexpconverter.cpp
    regexp.cpp
    regexphighlighter.cpp
    regexpwidget.cpp
    repeatregexp.cpp
    repeatwidget.cpp
    scrollededitorwindow.cpp
    selectablelineedit.cpp
    singlecontainerwidget.cpp
    textrangeregexp.cpp
    textregexp.cpp
    textwidget.cpp
    userdefinedregexps.cpp
    util.cpp
    verifier.cpp
    verifybuttons.cpp
    widgetfactory.cpp
    zerowidgets.cpp
)

add_library(kregexpeditorcommon SHARED ${kregexpeditorcommon_SRCS})

target_link_libraries(kregexpeditorcommon
    KF5::ConfigWidgets
    KF5::IconThemes
    KF5::KIOCore
    KF5::TextWidgets
    KF5::WidgetsAddons
    Qt5::Widgets
    )

set_target_properties(kregexpeditorcommon PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})
install(TARGETS kregexpeditorcommon ${INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

set(kregexpeditorgui_PART_SRCS kregexpeditorfactory.h )

add_library(kregexpeditorgui MODULE ${kregexpeditorgui_PART_SRCS})

target_link_libraries(kregexpeditorgui  KF5::KIOCore kregexpeditorcommon)

install(TARGETS kregexpeditorgui DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

set(kregexpeditor_SRCS main.cpp)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kregexpeditor.png")
ecm_add_app_icon(kregexpeditor_SRCS ICONS ${ICONS_SRCS})

add_executable(kregexpeditor ${kregexpeditor_SRCS})

target_link_libraries(kregexpeditor  KF5::KIOCore Qt5::Widgets kregexpeditorcommon)

install(TARGETS kregexpeditor ${INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES kregexpeditor.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES kregexpeditorgui.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

install(FILES
    KMultiFormListBox/kmultiformlistboxentry.h
    KMultiFormListBox/kmultiformlistbox.h
    KMultiFormListBox/kmultiformlistboxfactory.h
    DESTINATION ${INCLUDE_INSTALL_DIR}
)

install(FILES KWidgetStreamer/kwidgetstreamer.h DESTINATION ${INCLUDE_INSTALL_DIR})

ecm_install_icons(ICONS
    128-apps-kregexpeditor.png
    16-apps-kregexpeditor.png
    22-apps-kregexpeditor.png
    32-apps-kregexpeditor.png
    48-apps-kregexpeditor.png
    64-apps-kregexpeditor.png
    DESTINATION ${ICON_INSTALL_DIR}
    THEME hicolor
)
