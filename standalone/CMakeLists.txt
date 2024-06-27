cmake_minimum_required(VERSION 3.28.3)

project(standalone LANGUAGES CXX)

add_subdirectory(desktop_win64 ${CMAKE_BINARY_DIR}/desktop_win64)
add_subdirectory(steamdeck_win64 ${CMAKE_BINARY_DIR}/steamdeck_win64)
