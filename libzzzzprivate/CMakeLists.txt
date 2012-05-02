
set(libzzzzprivate_SRCS
    account.cpp
    accountmanager.cpp
    addaccountwizard.cpp
    addaccountwizardpage1.cpp
    addaccountwizardpage2.cpp
    mediafetcher.cpp
    pluginmanager.cpp
    themeengine.cpp
    typeswrapper.cpp
)

kde4_add_library(zzzzprivate SHARED ${libzzzzprivate_SRCS})
target_link_libraries(zzzzprivate ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${QCA2_LIBRARIES} ${QTOAUTH_LIBRARY} ${Grantlee_CORE_LIBRARIES} zzzz)
set_target_properties(zzzzprivate PROPERTIES VERSION 0.0.1 SOVERSION 0)
install(TARGETS zzzzprivate ${INSTALL_TARGETS_DEFAULT_ARGS})

