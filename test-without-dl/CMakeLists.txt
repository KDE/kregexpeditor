



########### next target ###############
if(KDE4_BUILD_TESTS)
set(regexptest_SRCS main.cpp )

kde4_automoc(${regexptest_SRCS})

kde4_add_executable(regexptest ${regexptest_SRCS})

target_link_libraries(regexptest  kregexpeditorgui ${KDE4_KIO_LIBS} )

endif(KDE4_BUILD_TESTS)
