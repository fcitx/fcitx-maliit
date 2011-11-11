include_directories(${QT_INCLUDES})
include_directories(${MALIIT_PLUGINS_INCLUDE_DIRS})

set(fcitx_maliit_SRCS
    fcitxplugin.cpp
    fcitxhost.cpp
    fcitxsettings.cpp
)


set(fcitx_maliit_MOC_SRCS
    fcitxhost.h
    fcitxplugin.h
    fcitxsettings.h
    keyboardwidget.h
)

qt4_wrap_cpp(fcitx_maliit_MOCS ${fcitx_maliit_MOC_SRCS})
add_library(fcitx-maliit MODULE ${fcitx_maliit_SRCS} ${fcitx_maliit_MOCS})
target_link_libraries(fcitx-maliit
                      ${QT_QTCORE_LIBRARY}
                      ${QT_QTGUI_LIBRARY}
                      ${QT_QTDECLARATIVE_LIBRARY}
                      ${MALIIT_PLUGINS_LIBRARIES}
)

install(TARGETS fcitx-maliit DESTINATION ${MALIIT_PLUGINS_INSTALL_DIR})
install(DIRECTORY qml DESTINATION share/fcitx/maliit)