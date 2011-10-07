set(zzzz_tencent_SRCS
tencentmicroblog.cpp
)

kde4_add_plugin(zzzz_tencent ${zzzz_tencent_SRCS})
target_link_libraries(zzzz_tencent ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QJSON_LIBRARY} ${QTOAUTH_LIBRARY} zzzz)

install(TARGETS zzzz_tencent DESTINATION ${PLUGIN_INSTALL_DIR})
install(FILES zzzz_tencent.desktop DESTINATION ${SERVICES_INSTALL_DIR})

add_subdirectory(icons)
