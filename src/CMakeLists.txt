include_directories(${QT_INCLUDES})
include_directories(${MALIIT_PLUGINS_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

include_directories(
    ${FCITX4_FCITX_INCLUDE_DIRS}
    ${FCITX4_FCITX_UTILS_INCLUDE_DIRS}
    ${FCITX4_FCITX_CONFIG_INCLUDE_DIRS}
    )

link_directories(
    ${FCITX4_FCITX_LIBRARY_DIRS}
    ${FCITX4_FCITX_UTILS_LIBRARY_DIRS}
    ${FCITX4_FCITX_CONFIG_LIBRARY_DIRS}
    )

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


QT4_ADD_DBUS_INTERFACE(fcitx_maliit_SRCS
    org.fcitx.Fcitx.InputContext.xml
    org.fcitx.Fcitx.InputContext
)

QT4_ADD_DBUS_INTERFACE(fcitx_maliit_SRCS
    org.fcitx.Fcitx.InputMethod.xml
    org.fcitx.Fcitx.InputMethod
)

QT4_ADD_DBUS_INTERFACE(fcitx_maliit_SRCS
    org.freedesktop.DBus.xml
    org.freedesktop.DBus
)

qt4_wrap_cpp(fcitx_maliit_MOCS ${fcitx_maliit_MOC_SRCS})
add_library(fcitx-maliit MODULE ${fcitx_maliit_SRCS} ${fcitx_maliit_MOCS})
target_link_libraries(fcitx-maliit
    ${QT_QTCORE_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTDBUS_LIBRARY}
    ${QT_QTDECLARATIVE_LIBRARY}
    ${MALIIT_PLUGINS_LIBRARIES}
    ${FCITX4_FCITX_UTILS_LIBRARIES}
    ${FCITX4_FCITX_CONFIG_LIBRARIES}
    ${FCITX4_FCITX_LIBRARIES}
)

install(TARGETS fcitx-maliit DESTINATION ${MALIIT_PLUGINS_INSTALL_DIR})
install(DIRECTORY qml DESTINATION share/fcitx/maliit)