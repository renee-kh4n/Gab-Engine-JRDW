# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_library(interface_gbe_engine_src INTERFACE)

add_subdirectory(Datatypes)
add_subdirectory(Graphics)
add_subdirectory(Window)
add_subdirectory(Editor)

target_link_libraries(interface_gbe_engine_src INTERFACE
    gbe::gbe_datatypes
    gbe::gbe_graphics
    gbe::gbe_window
    gbe::gbe_editor
)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})