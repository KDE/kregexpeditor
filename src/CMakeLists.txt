BISON_TARGET(qregexpparser qregexpparser.y ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.cc COMPILE_FLAGS "-d -p qregexp")
FLEX_TARGET(qregexplexer qregexpparser.l  ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexplexer.cpp COMPILE_FLAGS "-Pqregexp")

set_property(
    SOURCE
        ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexplexer.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.cc
        ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.hh
    PROPERTY SKIP_AUTOMOC ON
)
ADD_FLEX_BISON_DEPENDENCY( qregexplexer qregexpparser )


add_subdirectory( predefined )
add_subdirectory( pics )

include_directories(
    KWidgetStreamer/
    KMultiFormListBox/ )

########### next target ###############

add_library(kregexpeditorcommon SHARED
    ${BISON_qregexpparser_OUTPUTS}
    ${FLEX_qregexplexer_OUTPUTS}
    altnregexp.cpp
    altnwidget.cpp
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
    kregexpeditorwindow.h
    kregexpeditorwindow.cpp
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
    widgetfactory.cpp
    zerowidgets.cpp

    altnregexp.h
    altnwidget.h
    characterswidget.h
    charselector.h
    compoundregexp.h
    compoundwidget.h
    concregexp.h
    concwidget.h
    dcbutton.h
    dotregexp.h
    dragaccepter.h
    regexpeditorwindow.h
    emacsregexpconverter.h
    errormap.h
    infopage.h
    KMultiFormListBox/ccp.h
    KMultiFormListBox/kmultiformlistboxentry.h
    KMultiFormListBox/kmultiformlistbox.h
    KMultiFormListBox/kmultiformlistbox-multivisible.h
    KMultiFormListBox/kmultiformlistboxfactory.h
    KMultiFormListBox/indexWindow.h
    KMultiFormListBox/kmultiformlistbox-windowed.h
    KMultiFormListBox/widgetwindow.h
    KMultiFormListBox/windowlistboxitem.h
    kregexpeditorwindow.h
    kregexpeditorwindow.h
    kregexpeditorprivate.h
    KWidgetStreamer/kwidgetstreamer.h
    limitedcharlineedit.h
    lookaheadregexp.h
    lookaheadwidget.h
    multicontainerwidget.h
    myfontmetrics.h
    positionregexp.h
    qtregexpconverter.h
    qtregexphighlighter.h
    regexpbuttons.h
    regexpconverter.h
    regexp.h
    regexphighlighter.h
    regexpwidget.h
    repeatregexp.h
    repeatwidget.h
    scrollededitorwindow.h
    selectablelineedit.h
    singlecontainerwidget.h
    textrangeregexp.h
    textregexp.h
    textwidget.h
    userdefinedregexps.h
    verifier.h
    widgetfactory.h
    zerowidgets.h

    kregexpeditor.qrc
)

target_link_libraries(kregexpeditorcommon
    KF6::ConfigWidgets
    KF6::CoreAddons
    KF6::IconThemes
    KF6::WidgetsAddons
    KF6::TextWidgets
    KF6::I18n
    KF6::XmlGui
    Qt::Widgets
    Qt::Xml
)

set_target_properties(kregexpeditorcommon PROPERTIES VERSION 1.0.0 SOVERSION 1)
generate_export_header(kregexpeditorcommon)
install(TARGETS kregexpeditorcommon ${KDE_INSTALL_TARGETS_DEFAULT_ARGS} LIBRARY NAMELINK_SKIP)

########### next target ###############

file(GLOB ICONS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*-apps-kregexpeditor.png")
ecm_add_app_icon(kregexpeditor_SRCS ICONS ${ICONS_SRCS})

add_executable(kregexpeditor main.cpp)

target_link_libraries(kregexpeditor Qt::Widgets kregexpeditorcommon KF6::Crash)

install(TARGETS kregexpeditor ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})

########### install files ###############

install(FILES org.kde.kregexpeditor.desktop DESTINATION ${KDE_INSTALL_APPDIR})

install(FILES
    KMultiFormListBox/kmultiformlistboxentry.h
    KMultiFormListBox/kmultiformlistbox.h
    KMultiFormListBox/kmultiformlistboxfactory.h
    ${CMAKE_CURRENT_BINARY_DIR}/kregexpeditorcommon_export.h
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
