include_directories(${QT_INCLUDES})
include_directories(${MALIIT_PLUGINS_INCLUDE_DIRS})

set(fcitx_maliit_SRCS
    fcitxplugin.cpp
    fcitxhost.cpp
    fcitxsettings.cpp
)
qt4_automoc(${fcitx_maliit_SRCS})
add_library(fcitx-maliit MODULE ${fcitx_maliit_SRCS})
target_link_libraries(fcitx-maliit ${QT_QTCORE_LIBRARY})
