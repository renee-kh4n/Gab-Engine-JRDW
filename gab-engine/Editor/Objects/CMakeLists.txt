#LIBRARY LOADING
add_library("Editor-Objects" "Object.h" "Object.cpp")
target_sources("Editor-Objects" PRIVATE "Root.cpp")
target_sources("Editor-Objects" PRIVATE "Root.h")

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-Objects" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> Objects")