cmake_minimum_required(VERSION 3.28.3)

project(core LANGUAGES C)

# ---- Vars ----
set(assets   "../../assets")
set(lib      "../../lib")
set(platform "../../platform")

set(core       "${platform}/core")
set(raylib   "${lib}/raylib-5.0_win64_msvc16")

# ---- Sources ----
include_directories(include)
add_library(${PROJECT_NAME} src/mygame.c)

# ---- Dependencies ----
include_directories(${raylib}/include)
include_directories(${core}/include)
