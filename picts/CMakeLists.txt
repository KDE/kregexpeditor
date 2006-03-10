kde4_header()

include_directories( ${KDE4_INCLUDE_DIR} ${QT_INCLUDES} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}  )


########### install files ###############

install_files( ${DATA_INSTALL_DIR}/kregexpeditor/pics FILES altn.png begline.png compound.png neglookahead.png poslookahead.png select.png wordboundary.png anychar.png characters.png endline.png nonwordboundary.png repeat.png text.png error.png autoverify.png verify.png )

kde4_footer()



#original Makefile.am contents follow:

#pics_DATA= altn.png begline.png compound.png neglookahead.png poslookahead.png select.png wordboundary.png anychar.png characters.png endline.png nonwordboundary.png repeat.png text.png error.png autoverify.png verify.png
#picsdir = $(kde_datadir)/kregexpeditor/pics
