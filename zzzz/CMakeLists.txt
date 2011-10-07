
set(zzzz_SRCS
    composerwidget.cpp
    zzzzapp.cpp
    main.cpp
    mainwindow.cpp
    navarranger.cpp
    navbuttonswidget.cpp
    timelinewidget.cpp
    typeswrapper.cpp
)

kde4_add_executable(zzzz_bin ${zzzz_SRCS})
target_link_libraries(zzzz_bin
    ${KDE4_KDEUI_LIBS}
    ${KDE4_KUTILS_LIBS}
    ${KDE4_KIO_LIBS}
    ${KDE4_KHTML_LIBS}
    ${KDE4_KDEWEBKIT_LIBS}
    ${QT_QTWEBKIT_LIBRARY}
    ${Grantlee_CORE_LIBRARIES}
    zzzz
    zzzzprivate
)

set_target_properties(zzzz_bin PROPERTIES OUTPUT_NAME zzzz)

install(FILES zzzzui.rc DESTINATION ${DATA_INSTALL_DIR}/zzzz)

add_subdirectory(config)
