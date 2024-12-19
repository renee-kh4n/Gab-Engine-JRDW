set(CURRENT_CMAKE_LIB gbe_editor)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Editor.cpp"
	"App/EditorApp.cpp" 
	"App/RootWindow.cpp"
	"App/Windows/SceneWindow.cpp"
 "gbe_editor.h")

find_package(wxWidgets CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE wx::core wx::base)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})
