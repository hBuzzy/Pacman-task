cmake_minimum_required(VERSION 3.26)
project(pacman)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "D:/Code/C++/qt6/qt6-static/lib/cmake")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

include_directories(src)

add_executable(pacman main.cpp
        src/mainwindow/mainwindow.hpp
        src/mainwindow/mainwindow.ui
        src/scene/scene.hpp
        src/scene/objects/wall.hpp
        src/figures_widget/figures_list.hpp
        src/figures_widget/figure.hpp
        src/scene/objects/scene_object.hpp
        src/scene/objects/pacman.hpp
        src/scene/objects/coin.hpp)

target_link_libraries(pacman
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

file(COPY
        src/figures_widget/figures_list.hpp
        DESTINATION ${PROJECT_NAME}_autogen/include)
