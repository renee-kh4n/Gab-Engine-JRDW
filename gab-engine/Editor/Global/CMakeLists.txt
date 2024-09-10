#LIBRARY LOADING
add_library("Editor-Global" "Time.cpp" "Time.h")

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-Global" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> Global")