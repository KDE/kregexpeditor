



########### next target ###############
set(regexptest_SRCS main.cpp )


kde4_add_test_executable(regexptest ${regexptest_SRCS})

target_link_libraries(regexptest  kregexpeditorgui ${KDE4_KIO_LIBS} )
