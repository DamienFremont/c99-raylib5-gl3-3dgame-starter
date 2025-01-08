cmake_minimum_required(VERSION 3.28.3)

project(cJSON LANGUAGES C)

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
