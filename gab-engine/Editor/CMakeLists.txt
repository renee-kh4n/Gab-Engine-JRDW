
#SUBDIRECTORY INCLUDING (TODO)
add_subdirectory("Component")
add_subdirectory("Global")
add_subdirectory("Input")
add_subdirectory("ObjectFunctions")
add_subdirectory("ObjectHandlers")
add_subdirectory("Objects")

#LIBRARY LOADING
add_library(Editor "Editor.cpp" "Editor.h")
target_link_libraries(Editor PUBLIC
	"Editor-Component"
	"Editor-Global"
	"Editor-Input"
	"Editor-ObjectFunctions"
	"Editor-ObjectHandlers"
	"Editor-Objects"
)

#INCLUDE DEFAULT DEPENDENCIES
include_gabengine_directories(Editor)
#INCLUD BASE SRC FOLDER
target_include_directories(Editor PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor")