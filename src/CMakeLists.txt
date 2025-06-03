set(CURRENT_CMAKE_LIB gbe_engine_src)

#SUBDIRECTORY LOADING
add_subdirectory(Math)
add_subdirectory(Asset)
add_subdirectory(Graphics)
add_subdirectory(GUI)
add_subdirectory(Physics)
add_subdirectory(Window)
add_subdirectory(Audio)
add_subdirectory(Engine)
add_subdirectory(Editor)

add_library(${CURRENT_CMAKE_LIB} INTERFACE)

target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_math)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_asset)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_graphics)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_gui)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_physics)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_window)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_audio)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_editor)
target_link_libraries(${CURRENT_CMAKE_LIB} INTERFACE gbe_engine)


get_target_property(LINKED_LIBS ${CURRENT_CMAKE_LIB} INTERFACE_LINK_LIBRARIES)
#include directory for all libraries above
    foreach(lib ${LINKED_LIBS})
        string(APPEND LINKED_LIBS_STRING " | ${lib} ")
        target_include_directories(${lib} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    endforeach()

message("LINKED LIBRARIES IN GABENGINE INTERFACE: " ${LINKED_LIBS_STRING})


message("[GABENGINE] INCLUDED ROOT DIR TO LIBRARY: " ${CMAKE_CURRENT_SOURCE_DIR})