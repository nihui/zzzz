set(zzzz_netease_SRCS
neteasemicroblog.cpp
)

kde4_add_plugin(zzzz_netease ${zzzz_netease_SRCS})
target_link_libraries(zzzz_netease ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${QTOAUTH_LIBRARY} zzzz)

install(TARGETS zzzz_netease DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES zzzz_netease.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
