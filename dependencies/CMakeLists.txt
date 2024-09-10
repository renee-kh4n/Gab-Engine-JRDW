set(DEPENDENCY_DIR "${CMAKE_SOURCE_DIR}/dependencies")
set(DEPENDENCY_DIR ${DEPENDENCY_DIR} PARENT_SCOPE)

#SDL
set(SDL2_DIR "${DEPENDENCY_DIR}/SDL2-2.30.7/cmake")
set(SDL2_DIR "${SDL2_DIR}" PARENT_SCOPE)
#GLM
set(glm_DIR "${CMAKE_SOURCE_DIR}/vcpkg_installed/vcpkg/pkgs/glm_x64-windows/share/glm")
set(glm_DIR "${glm_DIR}" PARENT_SCOPE)
#GLAD
add_library(GLAD INTERFACE)
target_include_directories(GLAD INTERFACE glad)
set_target_properties(GLAD PROPERTIES LINKER_LANGUAGE CXX)
#KHR
add_library(KHR INTERFACE)
target_include_directories(KHR INTERFACE KHR)
set_target_properties(KHR PROPERTIES LINKER_LANGUAGE CXX)
#stbi
add_library(STBI INTERFACE)
target_include_directories(STBI INTERFACE stbi)
set_target_properties(STBI PROPERTIES LINKER_LANGUAGE CXX)

function(link_gabengine_exe ARG_TARGET)
	find_package(SDL2 REQUIRED)
	find_package(glm CONFIG REQUIRED)
	target_link_libraries(${ARG_TARGET} PRIVATE glm::glm)
	target_link_libraries(${ARG_TARGET} PUBLIC SDL2::SDL2 SDL2::SDL2main)
	target_link_libraries(${ARG_TARGET} INTERFACE GLAD)
	target_link_libraries(${ARG_TARGET} INTERFACE KHR)
	target_link_libraries(${ARG_TARGET} INTERFACE STBI)

	message("[GABENGINE] LINKED DEPENDENCIES -> ${ARG_TARGET}")
endfunction()

function(include_gabengine_directories ARG_TARGET)
	target_include_directories(${ARG_TARGET} PUBLIC ${DEPENDENCY_DIR})
	target_include_directories(${ARG_TARGET} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/SDL2-2.30.7/include")

	message("[GABENGINE] INCLUDED DEPENDENCY DIRECTORIES -> ${ARG_TARGET}")
endfunction()

message("[GABENGINE] LOADED: DEPENDENCIES")