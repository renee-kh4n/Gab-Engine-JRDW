#LIBRARY LOADING
add_library(Graphics "RenderPipeline.cpp" "RenderPipeline.h")
target_sources(Graphics PRIVATE "DrawCall.cpp" "DrawCall.h")
target_sources(Graphics PRIVATE "Framebuffer.h")
target_sources(Graphics PRIVATE "Light.cpp" "Light.h")
target_sources(Graphics PRIVATE "Lines.h")
target_sources(Graphics PRIVATE "Mesh.cpp" "Mesh.h")
target_sources(Graphics PRIVATE "Shader.cpp" "Shader.h")
target_sources(Graphics PRIVATE "Skybox.cpp" "Skybox.h")
target_sources(Graphics PRIVATE "Texture.cpp" "Texture.h")
target_sources(Graphics PRIVATE "tiny_obj_loader.h")

target_link_directories(Graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

#SET DEFAULT PROJECT INCLUDES
include_gabengine_directories(Graphics)

#DEBUG
message("[GABENGINE] LOADED: Graphics")