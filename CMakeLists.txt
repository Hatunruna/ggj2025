cmake_minimum_required(VERSION 3.10)

set(GAME bubble-express)

project(${GAME}
  VERSION 0.1.0
)

include(GNUInstallDirs)

if (CMAKE_TOOLCHAIN_FILE MATCHES "vcpkg.cmake$")
  find_package(unofficial-chipmunk CONFIG REQUIRED)
  set(CHIPMUNK_LIBS unofficial::chipmunk::chipmunk)
else ()
  find_library(CHIPMUNK_LIBS NAMES libchipmunk, chipmunk)
  if (CHIPMUNK_LIBS)
    message(STATUS "Chipmunk2D found: ${CHIPMUNK_LIBS}")
  else ()
    message(FATAL_ERROR "Chipmunk2D not found")
  endif ()
endif ()

set(BUILD_TESTING OFF CACHE BOOL "Enable GF tests" FORCE)
set(GF_BUILD_EXAMPLES OFF CACHE BOOL "Enable GF examples" FORCE)
set(GF_BUILD_DOCUMENTATION OFF CACHE BOOL "Enable GF documentation" FORCE)
add_subdirectory(engine EXCLUDE_FROM_ALL)
set_target_properties(gfcore0 PROPERTIES UNITY_BUILD ON)
set_target_properties(gf0 PROPERTIES UNITY_BUILD ON)
set_target_properties(gfnet0 PROPERTIES UNITY_BUILD ON)

if (WIN32)
  target_compile_options(gfcore0 PRIVATE /wd4251)
  target_compile_options(gf0 PRIVATE /wd4251)
  target_compile_options(gfnet0 PRIVATE /wd4251)
endif ()

# if (UNIX)
#   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address")
# endif ()

if (WIN32)
  set(GAME_DATADIR "./data" CACHE STRING "Path to game data")
else ()
  set(GAME_DATADIR "${CMAKE_INSTALL_PREFIX}/share/games/${GAME}" CACHE STRING "Path to game data")
endif()
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/code/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h @ONLY)

add_executable(${GAME}
  code/main.cc
  code/bits/BubbleEntity.cc
  code/bits/BubbleState.cc
  code/bits/CityEntity.cc
  code/bits/CityState.cc
  code/bits/ContractScene.cc
  code/bits/ContractSelectionEntity.cc
  code/bits/ContractState.cc
  code/bits/FeedbackEntity.cc
  code/bits/GameData.cc
  code/bits/GameHub.cc
  code/bits/GameState.cc
  code/bits/HeroEntity.cc
  code/bits/HeroHudEntity.cc
  code/bits/HeroReportEntity.cc
  code/bits/HeroState.cc
  code/bits/LoreEndEntity.cc
  code/bits/LoreEntity.cc
  code/bits/LoreScene.cc
  code/bits/MapEntity.cc
  code/bits/MapSettings.cc
  code/bits/MapState.cc
  code/bits/MountainsEntity.cc
  code/bits/Namegen.cc
  code/bits/PhysicsModel.cc
  code/bits/PhysicsDebug.cc
  code/bits/ProcGen.cc
  code/bits/Spot.cc
  code/bits/StartScene.cc
  code/bits/Support.cc
  code/bits/TitleEntity.cc
  code/bits/WorldScene.cc
)

if (WIN32)
  target_compile_options(${GAME} PRIVATE /wd4251)
else ()
  target_compile_options(${GAME} PRIVATE -Wall -Wextra)
endif ()

target_include_directories(${GAME}
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/code
    ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${GAME}
  PRIVATE
    gf0
    ${CHIPMUNK_LIBS}
)

add_executable(generate-raw-tileset
  code/generate-raw-tileset.cc
)

target_link_libraries(generate-raw-tileset
  PRIVATE
    gf0
)

if (WIN32)
  install(
    FILES
      $<TARGET_FILE:gfcore0>
      $<TARGET_FILE:gf0>
    DESTINATION .
  )

  install(
    DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/bin/
    DESTINATION .
    CONFIGURATIONS Release
    PATTERN "*.pdb" EXCLUDE
  )

  install(
    DIRECTORY ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-windows/debug/bin/
    DESTINATION .
    CONFIGURATIONS Debug
    PATTERN "*.pdb" EXCLUDE
  )

  install(TARGETS ${GAME} DESTINATION . COMPONENT G)
else ()
  install(TARGETS ${GAME} RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
endif ()

if (WIN32)
  install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/${GAME}/"
    DESTINATION "./data/"
  )
else ()
  install(
    DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/data/${GAME}"
    DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/games"
  )
endif ()

if (WIN32)
  set(CPACK_GENERATOR "ZIP")
  include(CPack)
endif()
