cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
# set(CMAKE_MAKE_PROGRAM(mingw32-make))
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/)

set(SOURCE_FILES 
  src/main.cpp
  src/renderer/renderer.hpp
  src/systems/player_system.hpp
  # src/systems/ui/irongui/irongui.hpp
  # src/systems/ui/modal_menu.hpp
  # src/systems/ui/game_ui_system.hpp
  src/systems/map_system.hpp
  src/systems/province_system.hpp
  src/systems/movement_system.hpp
  src/systems/animation_system.hpp
  src/systems/selection_system.hpp
  src/systems/spawn_system.hpp
  src/components/actor.hpp
  src/components/animated.hpp
  src/components/province.hpp
  src/components/selected.hpp
  src/components/sight.hpp
  src/components/tile.hpp
  src/components/unit.hpp
  src/systems/actor_system.hpp
  src/input.hpp
  src/common.hpp
  src/components/event.hpp)



project(game)

find_package(raylib 4.2.0 REQUIRED) # QUIET or REQUIRED

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

#if(UNIX)

#  add_library(raylib SHARED IMPORTED)
#  set_target_properties(raylib PROPERTIES
#    IMPORTED_LOCATION_NOCONFIG /usr/local/lib/libraylib.so)



add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# For compiler errors/warnings
# target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})




  # target_link_libraries(${PROJECT_NAME} raylib GameNetworkingSockets)
  # target_link_libraries(server GameNetworkingSockets)
  # target_link_libraries(client GameNetworkingSockets)

  # target_link_libraries(${PROJECT_NAME} ${CMAKE_SOURCE_DIR}/lib/libraylib.so.400 m)
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
  # target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/entt/)
#endif(UNIX)


#if(WIN32)
#  # add_library(raylib STATIC IMPORTED)
#  # set_target_properties(
#  #   raylib PROPERTIES
#  #   IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/libraylib.a"
#  #   INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_SOURCE_DIR}/include/raylib"
#  # )
#  target_link_libraries(${PROJECT_NAME} raylib opengl32 gdi32 winmm)
#  #    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
#  #    target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIRS}/include)
#endif(WIN32)
