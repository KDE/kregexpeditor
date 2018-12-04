BISON_TARGET(qregexpparser qregexpparser.y ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.cc COMPILE_FLAGS "-d -p qregexp")
FLEX_TARGET(qregexplexer qregexpparser.l  ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexplexer.cpp COMPILE_FLAGS "-Pqregexp")
ADD_FLEX_BISON_DEPENDENCY( qregexplexer qregexpparser )

set_property(SOURCE ${BISON_qregexpparser_OUTPUTS} PROPERTY SKIP_AUTOMOC ON)
set_property(SOURCE ${FLEX_qregexplexer_OUTPUTS} PROPERTY SKIP_AUTOMOC ON)

add_subdirectory( predefined )
add_subdirectory( pics )

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
    verifier.cpp
    verifybuttons.cpp
    widgetfactory.cpp
    zerowidgets.cpp
)

add_library(kregexpeditorcommon SHARED ${kregexpeditorcommon_SRCS})

target_link_libraries(kregexpeditorcommon
    KF5::TextWidgets
    KF5::CoreAddons
    KF5::WidgetsAddons
    Qt5::Widgets
    Qt5::Xml
    )

set_target_properties(kregexpeditorcommon PROPERTIES VERSION 1.0.0 SOVERSION 1)
install(TARGETS kregexpeditorcommon ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

########### next target ###############

set(kregexpeditorgui_PART_SRCS kregexpeditorfactory.cpp )

add_library(kregexpeditorgui MODULE ${kregexpeditorgui_PART_SRCS})

target_link_libraries(kregexpeditorgui  kregexpeditorcommon)

install(TARGETS kregexpeditorgui DESTINATION ${KDE_INSTALL_PLUGINDIR})

########### next target ###############

set(kregexpeditor_SRCS main.cpp)

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kregexpeditor.png")
ecm_add_app_icon(kregexpeditor_SRCS ICONS ${ICONS_SRCS})

add_executable(kregexpeditor ${kregexpeditor_SRCS})

target_link_libraries(kregexpeditor  Qt5::Widgets kregexpeditorcommon KF5::Crash)

install(TARGETS kregexpeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES org.kde.kregexpeditor.desktop  DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES kregexpeditorgui.desktop  DESTINATION ${KDE_INSTALL_KSERVICES5DIR})

install(FILES
    KMultiFormListBox/kmultiformlistboxentry.h
    KMultiFormListBox/kmultiformlistbox.h
    KMultiFormListBox/kmultiformlistboxfactory.h
    DESTINATION ${KDE_INSTALL_INCLUDEDIR}
)

install(FILES KWidgetStreamer/kwidgetstreamer.h DESTINATION ${KDE_INSTALL_INCLUDEDIR})

ecm_install_icons(ICONS
    128-apps-kregexpeditor.png
    16-apps-kregexpeditor.png
    22-apps-kregexpeditor.png
    32-apps-kregexpeditor.png
    48-apps-kregexpeditor.png
    64-apps-kregexpeditor.png
    DESTINATION ${KDE_INSTALL_ICONDIR}
    THEME hicolor
)
