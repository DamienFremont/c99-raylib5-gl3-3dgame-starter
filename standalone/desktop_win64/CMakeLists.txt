cmake_minimum_required(VERSION 3.28.3)

project(desktop_win64 LANGUAGES CXX)

# ---- Vars ----
set(assets     "../../assets")
set(lib        "../../lib")
set(platform   "../../platform")

set(game      "${platform}/game")
set(engine      "${platform}/engine")
set(raylib      "${lib}/raylib-5.0_win64_msvc16")
set(raylib_lib  "${raylib}/lib/raylib")
set(raylib_bin  "${raylib}/lib/raylib.dll")
set(raylibextra "${lib}/raylib-5.0-extra")
set(winmm_lib  "${lib}/winmm-Win64-10.0.22621.0/lib/WinMM")

set(demos      "${platform}/demos")

# ---- Sources ----
include_directories(
  src
  ${game}/include
  ${game}/src
  ${engine}/include
  ${engine}/src
  ${raylib}/include
  ${raylibextra}/include

  ${demos}/include
  ${demos}/src
)

file(GLOB EXEC_SOURCES
  "src/*.cpp"
  "${game}/include/*.h"
  "${game}/src/*.c"
  "${engine}/include/*.h"
  "${engine}/src/*.c"

  "${demos}/include/*.h"
  "${demos}/src/*.c"
)

# ---- Create standalone executable ---- 
add_executable(${PROJECT_NAME} WIN32 ${EXEC_SOURCES})
set_target_properties(${PROJECT_NAME} PROPERTIES
  WIN32_EXECUTABLE ON 
  CXX_STANDARD 20 
  OUTPUT_NAME ${PROJECT_NAME}
)

# ---- Dependencies ----
target_link_libraries(${PROJECT_NAME} 
  ${raylib_lib}
  ${winmm_lib}
)

# ---- Bundle (first build only) ----
file(REMOVE_RECURSE                       ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${assets}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)

file(REMOVE_RECURSE                       ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY ${assets}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
