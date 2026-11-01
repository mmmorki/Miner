cmake_minimum_required(VERSION 3.31)
project(miner LANGUAGES CXX)

set(CMAKE_PREFIX_PATH "C:/Users/ivane/devtool/C++/Qt/6.10.2/msvc2022_64")
set(CMAKE_CXX_STANDART 17)
set(CMAKE_CXX_STANDART_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
        Core
        Gui
        Widgets)

add_executable(miner
        src/main.cpp
        src/MainWindow.cpp
        src/Field.cpp
        src/BasicCell.cpp
        src/MinedCell.cpp
        src/CleanCell.cpp
        include/MainWindow.h
        include/Enums.h
        include/Field.h
        include/Functions.h
        include/BasicCell.h
        include/MinedCell.h
        include/CleanCell.h
        include/Constants.h
        resources/resources.qrc)

target_include_directories(miner PRIVATE
        include)

target_link_libraries(miner PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets)