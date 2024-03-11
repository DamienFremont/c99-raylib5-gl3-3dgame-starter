# this script adds all subprojects to a single build to allow IDEs understand the full project structure.

cmake_minimum_required(VERSION 3.28.3)

project(cpp20-raylib-starter LANGUAGES CXX)

# MS ONLY !
set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "desktop_win64" )

add_subdirectory(./platform   ${CMAKE_BINARY_DIR}/platform)
add_subdirectory(./standalone ${CMAKE_BINARY_DIR}/standalone)
# add_subdirectory(./lib        ${CMAKE_BINARY_DIR}/lib)
