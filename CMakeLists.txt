cmake_minimum_required(VERSION 3.10)
project(battleship)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

find_package(Qt6 COMPONENTS Core Gui Widgets REQUIRED)

add_subdirectory(backend)
add_subdirectory(frontend)

add_executable(${PROJECT_NAME} 
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    backend
    frontend
)