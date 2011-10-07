project(zzzz)
cmake_minimum_required(VERSION 2.6)
find_package(KDE4 REQUIRED 4.4)
find_package(QJson REQUIRED)
find_package(QCA2 REQUIRED)
find_package(QtOAuth REQUIRED)

find_package(Grantlee REQUIRED)

include(KDE4Defaults)
add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})
include_directories(${QCA2_INCLUDE_DIR} ${QTOAUTH_INCLUDE_DIR})

include_directories(${Grantlee_INCLUDE_DIRS})

include_directories(
    ${KDE4_INCLUDES}
    ${QT_INCLUDES}
    ${CMAKE_CURRENT_SOURCE_DIR}/libzzzz
    ${CMAKE_CURRENT_SOURCE_DIR}/libzzzzprivate
)

add_subdirectory(libzzzz)
add_subdirectory(libzzzzprivate)
add_subdirectory(zzzz)
add_subdirectory(microblogs)
add_subdirectory(themes)
