set(zzzz_sina_SRCS
sinamicroblog.cpp
)

kde4_add_plugin(zzzz_sina ${zzzz_sina_SRCS})
target_link_libraries(zzzz_sina ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${QTOAUTH_LIBRARY} zzzz)

install(TARGETS zzzz_sina DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES zzzz_sina.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
