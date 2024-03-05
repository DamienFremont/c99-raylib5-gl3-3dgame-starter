# this script adds all subprojects to a single build to allow IDEs understand the full project structure.

cmake_minimum_required(VERSION 3.28.3)

project(cpp20-raylib-starter LANGUAGES CXX)

# add_subdirectory(./lib        ${CMAKE_BINARY_DIR}/lib)
add_subdirectory(./platform   ${CMAKE_BINARY_DIR}/platform)
add_subdirectory(./standalone ${CMAKE_BINARY_DIR}/standalone)
