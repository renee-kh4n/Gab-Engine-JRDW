# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_library(gbe_editor 
	"Editor.cpp"
)
add_library(gbe::gbe_editor ALIAS gbe_editor)

target_link_libraries(gbe_editor PRIVATE gbe::gbe_graphics)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})