set(CURRENT_CMAKE_LIB gbe_math)


#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Vector2.cpp"
	"Vector2Int.cpp"
	"Vector3.cpp"
	"Vector4.cpp"
	"Matrix4.cpp"
 "Quaternion.h" "Quaternion.cpp" "TrackedVariable.h")

#COMPILE DEFINITIONS SETUP
target_compile_definitions(${CURRENT_CMAKE_LIB} PUBLIC GLM_ENABLE_EXPERIMENTAL)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PUBLIC glm::glm)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})