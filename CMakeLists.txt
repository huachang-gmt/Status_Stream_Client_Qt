cmake_minimum_required(VERSION 3.19)
project(Status_Stream_Client_Qt LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(Status_Stream_Client_Qt
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    src/tcp_client.cpp
    include/tcp_client.h
    src/tcp_worker.cpp
    include/tcp_worker.h
)

target_include_directories(Status_Stream_Client_Qt
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Status_Stream_Client_Qt
    PRIVATE
        Qt::Core
        Qt::Widgets
        ws2_32
)

install(TARGETS Status_Stream_Client_Qt
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Status_Stream_Client_Qt
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
