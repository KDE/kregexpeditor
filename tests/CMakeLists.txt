set(regexpeditortest_SRCS
   main.cpp
   regexpeditorwidget.cpp
)

add_executable(regexpeditor_gui ${regexpeditortest_SRCS})
target_link_libraries(regexpeditor_gui
     Qt5::Widgets
     KF5::TextWidgets
     KF5::Service
)
