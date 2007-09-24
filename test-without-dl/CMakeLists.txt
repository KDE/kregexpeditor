set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )




########### next target ###############
set(regexptest_SRCS main.cpp )


kde4_add_executable(regexptest TEST ${regexptest_SRCS})

target_link_libraries(regexptest  kregexpeditorgui ${KDE4_KIO_LIBS} )
