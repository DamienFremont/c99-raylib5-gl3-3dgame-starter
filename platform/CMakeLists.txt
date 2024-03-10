cmake_minimum_required(VERSION 3.28.3)

project(modules LANGUAGES CXX)

add_subdirectory(core   ${CMAKE_BINARY_DIR}/core)
add_subdirectory(mygame ${CMAKE_BINARY_DIR}/mygame)
