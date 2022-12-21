cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/)

set(SOURCE_FILES 
  app/src/main.cpp
  app/src/shared/input.hpp
  app/src/shared/common.hpp
  app/src/renderer/renderer.hpp
  app/src/world/systems/player_system.hpp
  app/src/world/systems/map_system.hpp
  app/src/world/systems/province_system.hpp
  app/src/world/systems/movement_system.hpp
  app/src/world/systems/animation_system.hpp
  app/src/world/systems/selection_system.hpp
  app/src/world/systems/spawn_system.hpp
  app/src/world/systems/actor_system.hpp
  app/src/world/components/actor.hpp
  app/src/world/components/animated.hpp
  app/src/world/components/province.hpp
  app/src/world/components/selected.hpp
  app/src/world/components/sight.hpp
  app/src/world/components/tile.hpp
  app/src/world/components/unit.hpp
  app/src/world/components/event.hpp
  )

if (UNIX)
     set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
     project(game)

     find_package(raylib 4.2.0 REQUIRED) # QUIET or REQUIRED

     add_executable(${PROJECT_NAME} ${SOURCE_FILES})
     # For compiler errors/warnings
     # target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
     target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
     target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
endif(UNIX)


if (WIN32)
  set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

  project(game LANGUAGES CXX)


  add_executable(${PROJECT_NAME} ${SOURCE_FILES})

  add_library(raylib STATIC IMPORTED)
  set_target_properties(
        raylib PROPERTIES
        IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
  )

  find_package(nlohmann_json 3.11.2 REQUIRED)

  target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
  target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    nlohmann_json::nlohmann_json 
    raylib 
    opengl32 
    gdi32 
    winmm 
    -static 
    )
  target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
     
endif(WIN32)


  # target_link_libraries(${PROJECT_NAME} raylib GameNetworkingSockets)
  # target_link_libraries(server GameNetworkingSockets)
  # target_link_libraries(client GameNetworkingSockets)

  # target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libraylib.so.400 m)
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  # target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/entt/)


# if (NOT raylib_FOUND) # If there's none, fetch and build raylib
#     set(CUSTOMIZE_BUILD ON)
#     set(SUPPORT_FILEFORMAT_BMP ON)
#   include(FetchContent)
#   FetchContent_Declare(
#     raylib
#     URL https://github.com/raysan5/raylib/archive/refs/tags/4.0.0.tar.gz
#   )
#   FetchContent_GetProperties(raylib)
#   if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
#     set(FETCHCONTENT_QUIET NO)
#     FetchContent_Populate(raylib)
#     set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
#     add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
#   endif()
# endif()


# add_executable(
#   server net/server/server.cpp
# )

# add_executable(
#   client net/client/client.cpp
# )

