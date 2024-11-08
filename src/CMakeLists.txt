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

# Add source to this project's executable.
add_executable(${PROJECT_NAME} "gab-engine.h" "gab-engine.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE gbe_engine_src)

#Dependency Loading
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

#DEBUG
get_target_property(LINKED_LIBS ${PROJECT_NAME} LINK_LIBRARIES)
# Construct the compiler string for the include directories.
    foreach(lib ${LINKED_LIBS})
        string(APPEND LINKED_LIBS_STRING " | ${lib} ")
    endforeach()
message("LINKED LIBRARIES: " ${LINKED_LIBS_STRING})
get_target_property(INCLUDED_DIRS ${PROJECT_NAME} INCLUDE_DIRECTORIES)
    # Construct the compiler string for the include directories.
    foreach(dir ${INCLUDED_DIRS})
        string(APPEND INCLUDED_DIRS_STRING " | ${dir} ")
    endforeach()
message("INCLUDED DIRECTORIES: " ${INCLUDED_DIRS_STRING})

#AUTO COPY DLL TO BUILD
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)