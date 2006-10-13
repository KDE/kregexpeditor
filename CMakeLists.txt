project(kregexpeditor)

ADD_DEFINITIONS( -DQT_NO_CAST_ASCII )

add_subdirectory( KWidgetStreamer ) 
add_subdirectory( KMultiFormListBox ) 
add_subdirectory( pics ) 
add_subdirectory( predefined ) 

include_directories(
	${CMAKE_SOURCE_DIR}/kregexpeditor/KWidgetStreamer/ 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/ )


########### next target ###############
set(libkmultiformlistbox_SRCS 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/ccp.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/kmultiformlistboxentry.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/kmultiformlistbox.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/kmultiformlistbox-multivisible.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/kmultiformlistboxfactory.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/indexWindow.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/kmultiformlistbox-windowed.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/widgetwindow.cpp 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KMultiFormListBox/windowlistboxitem.cpp
)

set(libkwidgetstreamer_SRCS 
	${CMAKE_SOURCE_DIR}/kregexpeditor/KWidgetStreamer/kwidgetstreamer.cpp
	)

set(kregexpeditorcommon_LIB_SRCS ${libkmultiformlistbox_SRCS} ${libkwidgetstreamer_SRCS}
   kregexpeditorgui.cpp 
   editorwindow.cpp 
   regexpwidget.cpp 
   drag.cpp 
   concwidget.cpp 
   zerowidgets.cpp 
   dragaccepter.cpp 
   repeatwidget.cpp 
   characterswidget.cpp 
   altnwidget.cpp 
   limitedcharlineedit.cpp 
   textregexp.cpp 
   textrangeregexp.cpp 
   concregexp.cpp 
   altnregexp.cpp 
   positionregexp.cpp 
   dotregexp.cpp 
   repeatregexp.cpp 
   textwidget.cpp 
   selectablelineedit.cpp 
   multicontainerwidget.cpp 
   scrollededitorwindow.cpp 
   compoundwidget.cpp 
   singlecontainerwidget.cpp 
   widgetfactory.cpp 
   dcbutton.cpp 
   gen_qregexplexer.cpp 
   gen_qregexpparser.cc 
   charselector.cpp 
   lookaheadregexp.cpp 
   lookaheadwidget.cpp 
   regexp.cpp 
   infopage.cpp 
   kregexpeditorprivate.cpp 
   compoundregexp.cpp 
   auxbuttons.cpp 
   myfontmetrics.cpp 
   regexpbuttons.cpp 
   userdefinedregexps.cpp 
   verifier.cpp 
   verifybuttons.cpp 
   errormap.cpp 
   qtregexphighlighter.cpp 
   regexpconverter.cpp 
   qtregexpconverter.cpp 
   emacsregexpconverter.cpp 
   regexphighlighter.cpp 
   util.cpp )

kde4_automoc(kregexpeditorcommon ${kregexpeditorcommon_LIB_SRCS})

kde4_add_library(kregexpeditorcommon SHARED ${kregexpeditorcommon_LIB_SRCS})

target_link_libraries(kregexpeditorcommon  ${KDE4_KIO_LIBS}   )

set_target_properties(kregexpeditorcommon PROPERTIES VERSION 1.0.0 SOVERSION 1 )
install(TARGETS kregexpeditorcommon  DESTINATION ${LIB_INSTALL_DIR})


########### next target ###############

set(kregexpeditorgui_PART_SRCS  ${kregexpeditorcommon_LIB_SRCS})

kde4_automoc(kregexpeditorgui ${kregexpeditorgui_PART_SRCS})

kde4_add_plugin(kregexpeditorgui WITH_PREFIX ${kregexpeditorgui_PART_SRCS})

kde4_install_libtool_file( ${PLUGIN_INSTALL_DIR} kregexpeditorgui )

target_link_libraries(kregexpeditorgui  ${KDE4_KIO_LIBS} )

install(TARGETS kregexpeditorgui  DESTINATION ${PLUGIN_INSTALL_DIR})


########### next target ###############

set(kregexpeditor_SRCS main.cpp ${kregexpeditorcommon_LIB_SRCS})

kde4_automoc(kregexpeditor ${kregexpeditor_SRCS})

kde4_add_executable(kregexpeditor ${kregexpeditor_SRCS})

target_link_libraries(kregexpeditor  ${KDE4_KIO_LIBS} )

install(TARGETS kregexpeditor  DESTINATION ${BIN_INSTALL_DIR} )


########### install files ###############

install( FILES kregexpeditor.desktop  DESTINATION ${XDG_APPS_DIR})
install( FILES kregexpeditorgui.desktop  DESTINATION ${SERVICES_INSTALL_DIR})

kde4_install_icons( ${ICON_INSTALL_DIR}   )

