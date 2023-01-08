cmake_minimum_required(VERSION 3.14)

project(Generator_engine LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core DBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core DBus)

add_executable(Generator_interface
    interface.cpp interface.h interface_main.cpp
)

target_link_libraries(Generator_interface PUBLIC
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus

)

add_executable(Generator_engine
  main.cpp generator.h generator.cpp
)


target_link_libraries(Generator_engine
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::DBus
)

install(TARGETS Generator_engine
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
