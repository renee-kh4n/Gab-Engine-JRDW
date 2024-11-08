set(CURRENT_CMAKE_LIB gbe_datatypes)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Vector2.cpp"
	"Vector2Int.cpp"
	"Vector3.cpp"
	"Vector4.cpp"
	"Matrix4.cpp"
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glm::glm)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})