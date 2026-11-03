cmake_minimum_required(VERSION 3.20.0)

list(APPEND BOARD_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
find_package(Zephyr REQUIRED)
project(my_zephyr_app)

target_sources(app PRIVATE src/main.cpp)
add_subdirectory(
    "${PROJECT_SOURCE_DIR}/../drivers/rajesh_driver"
    "${PROJECT_BINARY_DIR}/drivers/rajesh_driver"
)