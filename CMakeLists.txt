#PER MACHINE SYSTEM VARIABLES
set(VCPKG_TOOLCHAIN_DIR "C:/Program Files/Microsoft Visual Studio/2022/Community/VC/vcpkg/scripts/buildsystems/vcpkg.cmake")

# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.22)

set(CMAKE_CXX_STANDARD 23)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#set(CMAKE_TOOLCHAIN_FILE ${VCPKG_TOOLCHAIN_DIR})
message("INCLUDING VCPKG AT: " "${CMAKE_TOOLCHAIN_FILE}")

project ("gab-engine" LANGUAGES CXX)
#SUBDIRECTORY LOADING
add_subdirectory(src)

# Add source to this project's executable.
add_executable(${PROJECT_NAME} "src/gab-engine.h" "src/gab-engine.cpp")
target_link_libraries(${PROJECT_NAME} PRIVATE gbe_engine_src)

#Dependency Loading
#WINDOW CORE LIBRARY
find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(SDL2_mixer CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)




#SHADER COMPILATION
set(SHADERS_FILES "")
function(COMPILE_SHADER SRCPATH)
  add_custom_command(
    OUTPUT "${SRCPATH}.spv"
    MAIN_DEPENDENCY "${SRCPATH}"
    COMMAND "${PACKAGE_PREFIX_DIR}/tools/shaderc/glslc.exe" "${SRCPATH}" -o "${SRCPATH}.spv")
  list(APPEND SHADERS_FILES "${SRCPATH}.spv")
  set(SHADERS_FILES "${SHADERS_FILES}" PARENT_SCOPE)
endfunction()

function(DECOMPILE_SHADER SRCPATH)
  add_custom_command(
    OUTPUT "${SRCPATH}.json"
    MAIN_DEPENDENCY "${SRCPATH}.spv"
    COMMAND "${PACKAGE_PREFIX_DIR}/tools/spirv-cross/spirv-cross.exe" --output "${SRCPATH}.json" "${SRCPATH}.spv" --reflect)
  list(APPEND SHADERS_FILES "${SRCPATH}.json")
  set(SHADERS_FILES "${SHADERS_FILES}" PARENT_SCOPE)
endfunction()

file(GLOB verts "${CMAKE_CURRENT_LIST_DIR}/DefaultAssets/shaders/*.vert")
foreach(shader ${verts})
  COMPILE_SHADER(${shader})
  DECOMPILE_SHADER(${shader})
endforeach()
file(GLOB frags "${CMAKE_CURRENT_LIST_DIR}/DefaultAssets/shaders/*.frag")
foreach(shader ${frags})
  COMPILE_SHADER(${shader})
  DECOMPILE_SHADER(${shader})
endforeach()
  

add_custom_target(compile_shaders ALL DEPENDS ${SHADERS_FILES})
add_dependencies(${PROJECT_NAME} compile_shaders)

message("[GABENGINE] COMPILED SHADERS: ${SHADER_OUTPUT}")

#COPY ASSETS TO BUILD TARGET COMMAND
add_custom_target(COPY_ASSETS_COMMAND
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_LIST_DIR}/DefaultAssets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/DefaultAssets"
)

add_dependencies(${PROJECT_NAME} COPY_ASSETS_COMMAND)

set(generatatedfile_i 0)
foreach(generatedfile ${SHADERS_FILES})
    set(TARGET_NAME DELETE_${generatatedfile_i})
    add_custom_target(${TARGET_NAME}
    COMMAND ${CMAKE_COMMAND} -E rm -f ${generatedfile}
    )

    add_dependencies(${TARGET_NAME} COPY_ASSETS_COMMAND)
    add_dependencies(${PROJECT_NAME} ${TARGET_NAME})

    MATH(EXPR generatatedfile_i "${generatatedfile_i}+1")
    message("[GABENGINE] Queued to delete ${generatatedfile_i}: ${generatedfile}")
endforeach()

#COPY DLL TO BUILD
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
  COMMAND_EXPAND_LISTS
)

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