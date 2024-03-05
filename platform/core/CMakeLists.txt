cmake_minimum_required(VERSION 3.28.3)

project(core LANGUAGES C)

# ---- Vars ----
set(ASSETS   ../../assets)
set(LIB      ../../lib)
set(PLATFORM ../../platform)

set(RAYLIB ${LIB}/raylib-4.0.0_win64_msvc16)

# ---- Sources ----
include_directories(include)
add_library(${PROJECT_NAME} src/shaders_postprocessing.c)

# ---- Dependencies ----
include_directories(${RAYLIB}/include)
