# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_library(gbe_window
	"Window.cpp"
)
add_library(gbe::gbe_window ALIAS gbe_window)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(gbe_window PRIVATE SDL2::SDL2)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})