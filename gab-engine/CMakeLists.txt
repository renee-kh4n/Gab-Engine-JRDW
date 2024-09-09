# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_DIR_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/)

set(SDL2_DIR ${CMAKE_DIR_PREFIX}SDL2-2.30.7/cmake)
find_package(SDL2 REQUIRED)

add_subdirectory(${CMAKE_DIR_PREFIX}glm)

# Add source to this project's executable.

add_executable (gab-engine "gab-engine.cpp" "gab-engine.h")
target_sources(gab-engine PRIVATE "Engine/Engine.cpp" "Engine/Engine.h")

target_include_directories(gab-engine SYSTEM PRIVATE ${CMAKE_DIR_PREFIX}glm)
include_directories(SYSTEM ${CMAKE_DIR_PREFIX}glad)
include_directories(SYSTEM ${CMAKE_DIR_PREFIX}KHR)

target_link_libraries(gab-engine SDL2::SDL2 SDL2::SDL2main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gab-engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

add_custom_command(TARGET gab-engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:gab-engine> $<TARGET_RUNTIME_DLLS:gab-engine>
  COMMAND_EXPAND_LISTS
)