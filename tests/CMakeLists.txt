set(regexpeditortest_SRCS
   main.cpp
   regexpeditorwidget.cpp
)

add_executable(regexpeditor_gui ${regexpeditortest_SRCS})
target_link_libraries(regexpeditor_gui
    kregexpeditorcommon
    Qt6::Widgets
    KF6::TextWidgets
    KF6::Service
)
