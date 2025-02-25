cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include)

set(SOURCE_FILES 
  src/main.cpp
  src/shared/common.hpp
  src/renderer/renderer.hpp
  )

if (UNIX)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    project(game)

    # find_package(raylib 4.2.0 REQUIRED) # QUIET or REQUIRED 

    add_executable(${PROJECT_NAME} ${SOURCE_FILES})

    add_library(raylib SHARED IMPORTED)
    set_target_properties(
      raylib PROPERTIES
      #IMPORTED_LOCATION "${INCLUDE_DIRS}/raylib/lib/libraylib.so"
      IMPORTED_LOCATION "${INCLUDE_DIRS}/raylib/lib/libraylib.dylib"
      INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/raylib"
    )

    add_library(steam_api SHARED IMPORTED)
    set_target_properties(
      steam_api PROPERTIES
      #IMPORTED_LOCATION "${INCLUDE_DIRS}/steam/lib/libsteam_api.so"
      IMPORTED_LOCATION "${INCLUDE_DIRS}/steam/lib/libsteam_api.dylib"
      INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/steam"
    )

  # For compiler errors/warnings
  # target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
  target_link_libraries(${PROJECT_NAME} PRIVATE raylib steam_api)
  target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})

  configure_file("steam_appid.txt" "steam_appid.txt")
  execute_process(COMMAND ${CMAKE_COMMAND} -E copy
  "${CMAKE_CURRENT_SOURCE_DIR}/include/steam/lib/libsteam_api.dylib" "${CMAKE_CURRENT_BINARY_DIR}/libsteam_api.dylib")

  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory 
  "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets")

  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory 
  "${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/data")
endif(UNIX)


#if (WIN32)
#  set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")
#
#  project(game)
#
#  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#
#  add_library(raylib STATIC IMPORTED)
#  set_target_properties(
#        raylib PROPERTIES
#        IMPORTED_LOCATION "${INCLUDE_DIRS}/raylib/lib/raylib.lib"
#        INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/raylib"
#  )
#  add_library(steam_api STATIC IMPORTED)
#  set_target_properties(
#          steam_api PROPERTIES
#          IMPORTED_LOCATION "${INCLUDE_DIRS}/steam/lib/steam_api.lib"
#          INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}/steam"
#  )
#
#
#  target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
#  target_link_libraries(${PROJECT_NAME} PRIVATE raylib steam_api)
#  target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIRS})
#
#  configure_file("steam_appid.txt" "steam_appid.txt")
#
#  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory 
#  "${CMAKE_CURRENT_SOURCE_DIR}/assets" "${CMAKE_CURRENT_BINARY_DIR}/assets")
#
#  execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory 
#  "${CMAKE_CURRENT_SOURCE_DIR}/data" "${CMAKE_CURRENT_BINARY_DIR}/data")
#
#endif(WIN32)


