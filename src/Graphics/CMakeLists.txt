set(CURRENT_CMAKE_LIB gbe_graphics)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} SHARED
	"RenderPipeline.cpp"
	"Light.cpp"
	"DrawCall.cpp"
	"Material.cpp"
	"Mesh.cpp"
	"Shader.cpp"
	"Skybox.cpp"
	"Texture.cpp"
)
add_library(gbe::${CURRENT_CMAKE_LIB} ALIAS ${CURRENT_CMAKE_LIB})

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_datatypes)

find_package(Stb REQUIRED)
target_include_directories(${CURRENT_CMAKE_LIB} PRIVATE ${Stb_INCLUDE_DIR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glad::glad)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})