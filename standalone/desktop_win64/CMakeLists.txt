cmake_minimum_required(VERSION 3.28.3)

project(desktop_win64 LANGUAGES CXX)

# ---- Vars ----
set(ASSETS   ../../assets)
set(LIB      ../../lib)
set(PLATFORM ../../platform)
set(CORE ${PLATFORM}/core)
set(RAYLIB ${LIB}/raylib-4.0.0_win64_msvc16)
set(RAYLIB_LIB  "C:/Users/damien/git/cpp20-raylib-starter/lib/raylib-4.0.0_win64_msvc16/lib/raylib.lib")
set(RAYLIB_BIN "${RAYLIB}/lib/raylib.dll")

# ---- Sources ----
include_directories(
  src
  ${CORE}/include
  ${CORE}/src
  ${RAYLIB}/include
)

file(GLOB SOURCES
  "src/*.cpp"
  "${CORE}/include/*.h"
  "${CORE}/src/*.c"
)

# ---- Create standalone executable ---- 
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  WIN32_EXECUTABLE ON 
  CXX_STANDARD 20 
  OUTPUT_NAME ${PROJECT_NAME}
)

# ---- Dependencies ----
target_link_libraries(${PROJECT_NAME} 
  ${RAYLIB_LIB}
  "C:/Users/damien/git/cpp20-raylib-starter/lib/winmm-Win64-10.0.22621.0/lib/WinMM.Lib"
)

# ---- Bundle ----
file(COPY ${ASSETS}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${ASSETS}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
