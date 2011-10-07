set(zzzz_sohu_SRCS
sohumicroblog.cpp
)

kde4_add_plugin(zzzz_sohu ${zzzz_sohu_SRCS})
target_link_libraries(zzzz_sohu ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${QTOAUTH_LIBRARY} zzzz)

install(TARGETS zzzz_sohu DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES zzzz_sohu.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
