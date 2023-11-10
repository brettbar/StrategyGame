cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/)

set(SOURCE_FILES 
  app/src/main.cpp
  app/src/shared/common.hpp
  app/src/renderer/renderer.hpp
  app/src/world/systems/map_system.hpp
  app/src/world/systems/province_system.hpp
  app/src/world/systems/movement_system.hpp
  app/src/world/systems/animation_system.hpp
  app/src/world/systems/selection_system.hpp
  app/src/world/systems/actor_system.hpp
  app/src/world/components/actor.hpp
  app/src/world/components/animated.hpp
  app/src/world/components/province.hpp
  app/src/world/components/selected.hpp
  app/src/world/components/sight.hpp
  app/src/world/components/tile.hpp
  )

if (UNIX)
     set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
     project(game)

     find_package(raylib 4.2.0 REQUIRED) # QUIET or REQUIRED

     add_executable(${PROJECT_NAME} ${SOURCE_FILES})
 
     add_library(steam_api SHARED IMPORTED)
     set_target_properties(
       steam_api PROPERTIES
       IMPORTED_LOCATION "${INCLUDE_DIRS}/steam/lib/libsteam_api.so"
       INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/steam"
     )

     # For compiler errors/warnings
     # target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
     target_link_libraries(${PROJECT_NAME} PRIVATE raylib steam_api)
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
   add_library(steam_api SHARED IMPORTED)
   set_target_properties(
           steam_api PROPERTIES
           IMPORTED_LOCATION "${INCLUDE_DIRS}/steam/lib/libsteam_api.so"
           INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/steam"
   )

   find_package(nlohmann_json 3.11.2 REQUIRED)

   target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
   target_link_libraries(
     ${PROJECT_NAME}
     PRIVATE
     nlohmann_json::nlohmann_json
     raylib
     steam_api
     opengl32
     gdi32
     winmm
     -static
     )
   target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

 endif(WIN32)


