# CMakeList.txt : CMake project for gab-engine, include source and define
# project specific logic here.
#

#SUBDIRECTORY LOADING
add_library(gbe_datatypes 
	"Vector2.cpp"
	"Vector2Int.cpp"
	"Vector3.cpp"
	"Vector4.cpp"
)
add_library(gbe::gbe_datatypes ALIAS gbe_datatypes)

find_package(glm CONFIG REQUIRED)
target_link_libraries(gbe_datatypes PRIVATE glm::glm)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})