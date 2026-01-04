cmake_minimum_required(VERSION 3.16)
project(ShortVideoFollow LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt 路径（改成你本地安装路径）
set(CMAKE_PREFIX_PATH "E:/Qt/6.10.1/mingw_64")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(ShortVideoFollow
    src/main.cpp
    src/mainwindow.cpp
    src/userdetailwindow.cpp
    src/model/seqlist.cpp
    src/model/linkedlist.cpp
    ui/mainwindow.ui
)

target_include_directories(ShortVideoFollow PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

set_target_properties(ShortVideoFollow PROPERTIES
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Ensure AUTOUIC searches the ui/ directory for .ui files
set_target_properties(ShortVideoFollow PROPERTIES AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui")

target_link_libraries(ShortVideoFollow PRIVATE Qt6::Widgets)
