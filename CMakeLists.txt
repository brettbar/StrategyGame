cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
# set(CMAKE_MAKE_PROGRAM(mingw32-make))
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game)

add_executable(
  ${PROJECT_NAME} src/main.cpp
  include/raygui.h
  src/ui/ui.hpp
  src/ui/guilib.hpp
  src/renderer/renderer.hpp
  src/editor/editor.hpp
  src/world/player.hpp
  src/world/systems/map/map.hpp
  src/world/systems/map/provinces.hpp
  src/world/systems/map/terrain.hpp
  src/world/systems/movement.hpp
  src/world/systems/animation.hpp
  src/world/systems/selection.hpp
  src/world/systems/spawn.hpp
  src/world/entities/archetypes.hpp
  src/world/components/actor.hpp
  src/world/systems/actors.hpp
  src/input.hpp
  src/common.hpp
)


if(UNIX)
  target_link_libraries(${PROJECT_NAME} raylib m)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/entt/)
endif(UNIX)


if(WIN32)
  add_library(raylib STATIC IMPORTED)
  set_target_properties(
    raylib PROPERTIES
    IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  )
  target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
  #    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  #    target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIRS}/include)
endif(WIN32)
