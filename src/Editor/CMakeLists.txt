set(CURRENT_CMAKE_LIB gbe_editor)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Editor.cpp" "Gui/EditorGui.h" "Gui/InspectorData.h")

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_graphics)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_window)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_engine)

find_package(imgui CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE imgui::imgui)

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

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})
