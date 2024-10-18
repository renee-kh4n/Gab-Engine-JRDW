# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_library(gbe_graphics 
	"RenderPipeline.cpp"
)
add_library(gbe::gbe_graphics ALIAS gbe_graphics)

find_package(glm CONFIG REQUIRED)
target_link_libraries(gbe_graphics PRIVATE glm::glm)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})