# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_subdirectory(Datatypes)
add_subdirectory(Editor)
add_subdirectory(Graphics)
add_subdirectory(Window)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})