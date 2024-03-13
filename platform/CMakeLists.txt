cmake_minimum_required(VERSION 3.28.3)

project(modules LANGUAGES CXX)

add_subdirectory(engine ${CMAKE_BINARY_DIR}/engine)
add_subdirectory(mygame ${CMAKE_BINARY_DIR}/mygame)

add_subdirectory(demos ${CMAKE_BINARY_DIR}/demos)
