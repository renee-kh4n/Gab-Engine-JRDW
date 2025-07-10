set(CURRENT_CMAKE_LIB gbe_graphics)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"RenderPipeline.cpp"
	"Data/Light.cpp"
	"Data/DrawCall.cpp"
	
	"Data/Skybox.cpp"
	
	"AssetLoaders/TextureLoader.cpp" "AssetLoaders/ShaderLoader.h"
	"AssetLoaders/ShaderLoader.cpp"
	"AssetLoaders/MeshLoader.cpp"
	"AssetLoaders/MaterialLoader.cpp"
	)


find_package(Stb REQUIRED)
target_include_directories(${CURRENT_CMAKE_LIB} PRIVATE ${Stb_INCLUDE_DIR})

find_package(glad CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glad::glad)

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_editor)

find_package(Vulkan REQUIRED)
if (Vulkan_FOUND)
	target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE ${Vulkan_LIBRARIES})
	target_include_directories(${CURRENT_CMAKE_LIB} PRIVATE ${Vulkan_INCLUDE_DIRS})
	target_include_directories(${CURRENT_CMAKE_LIB} PRIVATE ${VULKANSDK_DIR})
	message("[GABENGINE] INCLUDED: " ${Vulkan_INCLUDE_DIRS})
	message("[GABENGINE] VULKAN SDK AT: " ${VULKANSDK_DIR})
else()
	message(FATAL_ERROR "[GABENGINE] VULKAN NOT FOUND")
endif()

IF (WIN32)
	set(OpenCV_DIR "${VCPKG_INSTALLED_DIR}/x64-windows/share/opencv2")
	message("[GABENGINE WIN32] FOUND: ${OpenCV_DIR}")
ELSE()
# set stuff for other systems
ENDIF()

find_package(OpenCV REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE opencv_core opencv_highgui)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})