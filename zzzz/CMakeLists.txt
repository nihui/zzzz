
set(zzzz_SRCS
    composerwidget.cpp
    zzzzapp.cpp
    main.cpp
    mainwindow.cpp
    navbuttonswidget.cpp
    timelinedelegate.cpp
    timelinewidget.cpp
)

kde4_add_ui_files(zzzz_SRCS
    composerwidget.ui
)

kde4_add_executable(zzzz_bin ${zzzz_SRCS})
target_link_libraries(zzzz_bin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${KDE4_KIO_LIBS}
    zzzz
    zzzzprivate
)

set_target_properties(zzzz_bin PROPERTIES OUTPUT_NAME zzzz)

install(FILES zzzzui.rc DESTINATION ${DATA_INSTALL_DIR}/zzzz)

add_subdirectory(config)
