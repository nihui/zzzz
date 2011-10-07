
set(libzzzzprivate_SRCS
    account.cpp
    accountmanager.cpp
    addaccountwizard.cpp
    addaccountwizardpage1.cpp
    addaccountwizardpage2.cpp
#     avatarmanager.cpp
    pluginmanager.cpp
)

kde4_add_library(zzzzprivate SHARED ${libzzzzprivate_SRCS})
target_link_libraries(zzzzprivate ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${KDE4_KUTILS_LIBS} ${QCA2_LIBRARIES} ${QTOAUTH_LIBRARY} zzzz)
set_target_properties(zzzzprivate PROPERTIES VERSION 0.0.1 SOVERSION 0)
install(TARGETS zzzzprivate ${INSTALL_TARGETS_DEFAULT_ARGS})

