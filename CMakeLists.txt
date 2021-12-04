cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
# set(CMAKE_MAKE_PROGRAM(mingw32-make))
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game)

if (WIN32)
  add_library(raylib STATIC IMPORTED)
  set_target_properties(raylib PROPERTIES
      IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
      INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  )
endif (WIN32)

set(SOURCES src/main.cpp src/map.cpp src/actors.cpp src/ui/ui.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

if (WIN32)
  target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
endif (WIN32)

if (UNIX)
  target_link_libraries(${PROJECT_NAME}  raylib m)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  target_include_directories(${PROJECT_NAME} PRIVATE ${raylib_SOURCE_DIRS}/include)
endif (UNIX)


