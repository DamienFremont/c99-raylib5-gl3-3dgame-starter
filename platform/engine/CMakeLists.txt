cmake_minimum_required(VERSION 3.28.3)

project(engine LANGUAGES C)

# ---- Vars ----
set(assets   "../../assets")
set(lib      "../../lib")
set(platform "../../platform")

set(raylib   "${lib}/raylib-5.0_win64_msvc16")

# ---- Sources ----
include_directories(
    src
    include
)

# ---- Create library ---- 
file(GLOB EXEC_SOURCES
     "src/*.c"
     "include/*.h"
)
add_library(${PROJECT_NAME} ${EXEC_SOURCES})

# ---- Dependencies ----
include_directories(${raylib}/include)
