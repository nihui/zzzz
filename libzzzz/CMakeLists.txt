
set(libzzzz_SRCS
microblog.cpp
plugin.cpp
tcmicroblog.cpp
utility.cpp
)

kde4_add_library(zzzz SHARED ${libzzzz_SRCS})
target_link_libraries(zzzz ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${QJSON_LIBRARY})
set_target_properties(zzzz PROPERTIES VERSION 0.0.1 SOVERSION 0)
# install(TARGETS zzzz ${INSTALL_TARGETS_DEFAULT_ARGS})
install(FILES zzzzplugin.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})
install(FILES zzzzmicroblog.desktop DESTINATION ${SERVICETYPES_INSTALL_DIR})

