set(CURRENT_CMAKE_LIB gbe_engine_src)

#SUBDIRECTORY LOADING
add_subdirectory(Datatypes)
add_subdirectory(Graphics)
add_subdirectory(Window)
add_subdirectory(Editor)

add_library(${CURRENT_CMAKE_LIB} INTERFACE)

target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_datatypes)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_graphics)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_window)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_editor)