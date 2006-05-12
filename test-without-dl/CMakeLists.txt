



########### next target ###############

set(regexptest_SRCS main.cpp )

kde4_automoc(${regexptest_SRCS})

if(KDE4_BUILD_TESTS)

kde4_add_executable(regexptest ${regexptest_SRCS})

target_link_libraries(regexptest  ${KDE4_KDECORE_LIBS} kregexpeditorgui kio )

endif(KDE4_BUILD_TESTS)

########### install files ###############





#original Makefile.am contents follow:

#check_PROGRAMS = regexptest
#regexptest_SOURCES = main.cpp
#regexptest_LDFLAGS = $(all_libraries)
#regexptest_LDADD =  ../libkregexpeditorgui.la $(LIB_KSYCOCA) 
#INCLUDES =  $(all_includes)
#KDE_CXXFLAGS = -g
#
#METASOURCES = AUTO
