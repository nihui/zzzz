
set(kcm_zzzz_accountconfig_SRCS
    accountconfig.cpp
)

kde4_add_plugin(kcm_zzzz_accountconfig ${kcm_zzzz_accountconfig_SRCS})

target_link_libraries(kcm_zzzz_accountconfig ${KDE4_KUTILS_LIBS} zzzzprivate)

install(TARGETS kcm_zzzz_accountconfig DESTINATION ${PLUGIN_INSTALL_DIR})

install(FILES zzzz_accountconfig.desktop DESTINATION ${SERVICES_INSTALL_DIR})
