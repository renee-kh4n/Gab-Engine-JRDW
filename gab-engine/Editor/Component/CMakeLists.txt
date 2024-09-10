#LIBRARY LOADING
add_library("Editor-Component" "Transform.cpp" "Transform.h")

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-Component" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> Component")