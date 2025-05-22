set(CURRENT_CMAKE_LIB gbe_asset)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
 
 "AssetTypes/Texture.cpp"
 "AssetTypes/Shader.cpp"
 "AssetTypes/Mesh.cpp"
 "AssetInjection/AssetSocket.h" 
 "AssetInjection/AssetSocket.cpp" 
 "AssetLoading/AssetDeserializer.h"
 "AssetLoading/AssetLoader.h"
 "Parsing/gbeParser.h" "AssetLoading/AssetDeserializer.cpp"  
 "AssetTypes/Audio.cpp"
 "AssetTypes/Material.cpp"
)

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)

find_package(glaze CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glaze::glaze)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})
