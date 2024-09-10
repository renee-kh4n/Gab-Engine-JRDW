# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

# Add source to this project's executable.

add_executable (gab-engine "gab-engine.cpp" "gab-engine.h")
message("[GABENGINE] INCLUDING DIRECTORY: " ${DEPENDENCY_DIR})

#SUBDIRECTORY LOADING
add_subdirectory(Editor)
add_subdirectory(Datatypes)
add_subdirectory(Graphics)
add_subdirectory(Window)

#LIBRARY LINKING
link_gabengine_exe(gab-engine)
target_link_libraries(gab-engine PUBLIC Datatypes Editor Graphics Window)

message("[GABENGINE] LOADED: ROOT")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET gab-engine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

#AUTO COPY DLL TO BUILD
add_custom_command(TARGET gab-engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:gab-engine> $<TARGET_RUNTIME_DLLS:gab-engine>
  COMMAND_EXPAND_LISTS
)