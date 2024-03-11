cmake_minimum_required(VERSION 3.28.3)

project(desktop_win64 LANGUAGES CXX)

# ---- Vars ----
set(assets     "../../assets")
set(lib        "../../lib")
set(platform   "../../platform")

set(mygame     "${platform}/mygame")
set(core       "${platform}/core")
set(raylib     "${lib}/raylib-5.0_win64_msvc16")
set(raylib_lib "C:/Users/damien/git/cpp20-raylib-starter/lib/raylib-5.0_win64_msvc16/lib/raylib.lib")
set(raylib_bin "${raylib}/lib/raylib.dll")

# ---- Sources ----
include_directories(
  src
  ${mygame}/include
  ${mygame}/src
  ${core}/include
  ${core}/src
  ${raylib}/include
)

file(GLOB EXEC_SOURCES
  "src/*.cpp"
  "${mygame}/include/*.h"
  "${mygame}/src/*.c"
  "${core}/include/*.h"
  "${core}/src/*.c"
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
  "C:/Users/damien/git/cpp20-raylib-starter/lib/winmm-Win64-10.0.22621.0/lib/WinMM.Lib"
)

# ---- Bundle (first build only) ----
file(REMOVE_RECURSE                       ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(COPY ${assets}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Debug)
file(REMOVE_RECURSE                       ${CMAKE_CURRENT_BINARY_DIR}/Release)
file(COPY ${assets}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Release)
