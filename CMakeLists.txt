cmake_minimum_required(VERSION 3.21.1)

option(LINK_INSIGHT "Link Qt Insight Tracker library" ON)
option(BUILD_QDS_COMPONENTS "Build design studio components" ON)

project(TextMeApp LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)

find_package(Qt6 6.6.0 REQUIRED COMPONENTS Sql Core Gui Qml Quick)


if (Qt6_VERSION VERSION_GREATER_EQUAL 6.3)
    qt_standard_project_setup()
endif()

qt_add_executable(TextMeApp src/main.cpp
    ContactPage.qml
    ConversationPage.qml
    avatar.jpg
    avatar2.jpg
    avatar3.png
    sqlcontactmodel.h sqlcontactmodel.cpp
    sqlconversationmodel.h sqlconversationmodel.cpp
    LoginPage.qml
    sqlloginmodel.h sqlloginmodel.cpp
)

qt_add_resources(TextMeApp "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)

target_link_libraries(TextMeApp PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Qml
    Qt6::Quick
    Qt6::Sql
)

if (BUILD_QDS_COMPONENTS)
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)

if (LINK_INSIGHT)
    include(${CMAKE_CURRENT_SOURCE_DIR}/insight)
endif ()

include(GNUInstallDirs)
install(TARGETS TextMeApp
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)


