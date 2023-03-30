project(lsp VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets QuickControls2 Concurrent)

set(PROJECT_SOURCES
        main.cpp
        services.h
        services.cpp
        upload.h
        upload.cpp
        githubapi.h
        githubapi.cpp
        main.qrc
        favicon.rc
)

add_executable(${PROJECT_NAME}
  ${PROJECT_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt::Widgets Qt::QuickControls2 Qt::Concurrent)

install(TARGETS ${PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(${PROJECT_NAME})
    qt_finalize_executable(${PROJECT_NAME})
endif()
