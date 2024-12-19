set(CURRENT_CMAKE_LIB gbe_asset)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"AssetLoader.cpp"
 "AssetReference.h" "BaseAsset.cpp")

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})
