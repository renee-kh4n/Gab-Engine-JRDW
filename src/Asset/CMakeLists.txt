set(CURRENT_CMAKE_LIB gbe_asset)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
 
 "AssetTypes/Texture.cpp"
 "AssetTypes/Shader.cpp"
 "AssetTypes/Mesh.cpp"
 "AssetSocket.h" "AssetSocket.cpp" "AssetDeserializer.h")

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})
