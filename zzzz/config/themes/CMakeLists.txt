
set(kcm_zzzz_themeconfig_SRCS
    themeconfig.cpp
)

kde4_add_plugin(kcm_zzzz_themeconfig ${kcm_zzzz_themeconfig_SRCS})

target_link_libraries(kcm_zzzz_themeconfig ${KDE4_KUTILS_LIBS} ${Grantlee_CORE_LIBRARIES} zzzzprivate)

install(TARGETS kcm_zzzz_themeconfig DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES zzzz_themeconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})
