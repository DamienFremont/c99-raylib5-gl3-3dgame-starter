cmake_minimum_required(VERSION 3.28.3)

project(modules LANGUAGES CXX)

add_subdirectory(engine ${CMAKE_BINARY_DIR}/engine)
add_subdirectory(game ${CMAKE_BINARY_DIR}/game)

add_subdirectory(demos ${CMAKE_BINARY_DIR}/demos)
