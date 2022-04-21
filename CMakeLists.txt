cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
# set(CMAKE_MAKE_PROGRAM(mingw32-make))
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(game)

find_package(raylib 4.0.0 QUIET) # QUIET or REQUIRED
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
  include(FetchContent)
  FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
  )
  FetchContent_GetProperties(raylib)
  if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
    set(FETCHCONTENT_QUIET NO)
    FetchContent_Populate(raylib)
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
  endif()
endif()

add_executable(
  ${PROJECT_NAME} src/main.cpp
  src/guilib/gui_system.hpp
  src/renderer/renderer.hpp
  src/world/systems/player_system.hpp
  src/world/systems/ui/ui_system.hpp
  src/world/systems/map/map_system.hpp
  src/world/systems/map/province_system.hpp
  src/world/systems/map/terrain_system.hpp
  src/world/systems/movement_system.hpp
  src/world/systems/animation_system.hpp
  src/world/systems/selection_system.hpp
  src/world/systems/spawn_system.hpp
  src/world/entities/archetypes.hpp
  src/world/components/actor.hpp
  src/world/components/animated.hpp
  src/world/components/province.hpp
  src/world/components/selected.hpp
  src/world/components/sight.hpp
  src/world/components/tile.hpp
  src/world/components/unit.hpp
  src/world/systems/actor_system.hpp
  src/input.hpp
  src/common.hpp
  src/world/components/event.hpp)

add_executable(
  server net/server/server.cpp
)


if(UNIX)
  target_link_libraries(${PROJECT_NAME} raylib GameNetworkingSockets)
  target_link_libraries(server GameNetworkingSockets)
  # target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libraylib.so.400 m)
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  # target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/entt/)
endif(UNIX)


if(WIN32)
  # add_library(raylib STATIC IMPORTED)
  # set_target_properties(
  #   raylib PROPERTIES
  #   IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
  #   INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  # )
  target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
  #    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  #    target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIRS}/include)
endif(WIN32)
