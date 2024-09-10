#LIBRARY LOADING
add_library("Editor-ObjectFunctions" STATIC "ObjectFunctions.h" "EarlyUpdate.h" "LateUpdate.h" "Update.h")

set_target_properties("Editor-ObjectFunctions" PROPERTIES LINKER_LANGUAGE CXX)

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-ObjectFunctions" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> ObjectFunctions")