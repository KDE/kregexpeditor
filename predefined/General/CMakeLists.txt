kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kregexpeditor/predefined/general FILES spaces.regexp anything.regexp )

kde4_footer()



#original Makefile.am contents follow:

#pkg_DATA = spaces.regexp anything.regexp
# 
#pkgdir = $(kde_datadir)/kregexpeditor/predefined/general
# 
#EXTRA_DIST = $(pkg_DATA) 
