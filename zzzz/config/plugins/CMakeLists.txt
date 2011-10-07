
set(kcm_zzzz_pluginconfig_SRCS
    pluginconfig.cpp
)

kde4_add_plugin(kcm_zzzz_pluginconfig ${kcm_zzzz_pluginconfig_SRCS})

target_link_libraries(kcm_zzzz_pluginconfig ${KDE4_KUTILS_LIBS})

install(TARGETS kcm_zzzz_pluginconfig DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES zzzz_pluginconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})
