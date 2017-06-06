cmake_minimum_required(VERSION 3.5)

project(ohjoy)
set(CMAKE_CXX_STANDARD 11)

find_package(Threads REQUIRED)

add_library(${PROJECT_NAME}
    src/device.cpp
    src/ds4.cpp
    src/event.cpp
    )

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED TRUE)

target_include_directories(${PROJECT_NAME}
    PUBLIC include
    PUBLIC $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
    )
target_link_libraries(${PROJECT_NAME}
    PRIVATE Threads::Threads
    )
