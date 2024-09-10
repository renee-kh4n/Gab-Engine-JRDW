#LIBRARY LOADING
add_library("Editor-ObjectHandlers" "InputHandler.h" "ObjectHandler.h" "ObjectHandlers.h" "PhysicsHandler.cpp" "PhysicsHandler.h")
target_sources("Editor-ObjectHandlers" PRIVATE "Members/ContactResolver.cpp")
target_sources("Editor-ObjectHandlers" PRIVATE "Members/ContactResolver.h")

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-ObjectHandlers" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> ObjectHandlers")