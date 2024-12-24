set(CURRENT_CMAKE_LIB gbe_graphics)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"RenderPipeline.cpp"
	"Data/Light.cpp"
	"Data/DrawCall.cpp"
	"Data/Material.cpp"
	"Data/Mesh.cpp"
	"Data/Skybox.cpp"
	
	"AssetLoaders/TextureLoader.cpp" "AssetLoaders/ShaderLoader.h" "AssetLoaders/ShaderLoader.cpp")

find_package(Stb REQUIRED)
target_include_directories(${CURRENT_CMAKE_LIB} PRIVATE ${Stb_INCLUDE_DIR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glad::glad)

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})