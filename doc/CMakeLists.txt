kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${HTML_INSTALL_DIR}/$(KDE_LANG)/KRegExpEditor FILES  )

kde4_footer()



#original Makefile.am contents follow:

#
#KDE_LANG = en
#KDE_DOCS = AUTO
#
#docdir = $(kde_htmldir)/$(KDE_LANG)/KRegExpEditor
