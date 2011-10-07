set(zzzz_statusnet_SRCS
statusnetmicroblog.cpp
)

kde4_add_plugin(zzzz_statusnet ${zzzz_statusnet_SRCS})
target_link_libraries(zzzz_statusnet ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${QTOAUTH_LIBRARY} zzzz)

install(TARGETS zzzz_statusnet DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES zzzz_statusnet.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
