set(CURRENT_CMAKE_LIB gbe_editor)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Editor.cpp"
 "Editor.h")

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glm::glm)

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_graphics)

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE wx::core wx::base)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})