project(kregexpeditor)

############### search packages used by KDE ###############

set (QT_MIN_VERSION "4.6.0")
find_package(KDE4 4.4.1 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(qregexpparser qregexpparser.y ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexpparser.cc COMPILE_FLAGS "-d -p qregexp")
FLEX_TARGET(qregexplexer qregexpparser.l  ${CMAKE_CURRENT_BINARY_DIR}/gen_qregexplexer.cpp COMPILE_FLAGS "-Pqregexp")
ADD_FLEX_BISON_DEPENDENCY( qregexplexer qregexpparser )

include_directories(${CMAKE_CURRENT_BINARY_DIR})


############### Needed commands before building anything ###############

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

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

kde4_add_library(kregexpeditorcommon SHARED ${kregexpeditorcommon_SRCS})

target_link_libraries(kregexpeditorcommon ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

set_target_properties(kregexpeditorcommon PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})
install(TARGETS kregexpeditorcommon RUNTIME DESTINATION bin
                       LIBRARY DESTINATION lib
                       ARCHIVE DESTINATION lib)

########### next target ###############

set(kregexpeditorgui_PART_SRCS kregexpeditorfactory.cpp)

kde4_add_plugin(kregexpeditorgui ${kregexpeditorgui_PART_SRCS})

target_link_libraries(kregexpeditorgui ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kregexpeditorcommon)

install(TARGETS kregexpeditorgui DESTINATION ${PLUGIN_INSTALL_DIR})

########### next target ###############

set(kregexpeditor_SRCS main.cpp)

kde4_add_app_icon(kregexpeditor_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/hi*-app-kregexpeditor.png")

kde4_add_executable(kregexpeditor ${kregexpeditor_SRCS})

target_link_libraries(kregexpeditor ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} kregexpeditorcommon)

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

kde4_install_icons(${ICON_INSTALL_DIR})
