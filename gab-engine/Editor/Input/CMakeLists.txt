#LIBRARY LOADING
add_library("Editor-Input" "KeyDefines.h" "InputSystem.h" "InputAction.h" "ActionImplementation.h")
target_sources("Editor-Input" PRIVATE "Implementations/Implementations.h")
target_sources("Editor-Input" PRIVATE "Implementations/KeyPressImplementation.cpp")
target_sources("Editor-Input" PRIVATE "Implementations/KeyPressImplementation.h")
target_sources("Editor-Input" PRIVATE "Implementations/MouseDragImplementation.h")
target_sources("Editor-Input" PRIVATE "Implementations/MouseScrollImplementation.cpp")
target_sources("Editor-Input" PRIVATE "Implementations/MouseScrollImplementation.h")
target_sources("Editor-Input" PRIVATE "Implementations/WasdDeltaImplementation.cpp")
target_sources("Editor-Input" PRIVATE "Implementations/WasdDeltaImplementation.h")
target_sources("Editor-Input" PRIVATE "Action/KeyPress.h")
target_sources("Editor-Input" PRIVATE "Action/MouseDrag.h")
target_sources("Editor-Input" PRIVATE "Action/MouseScroll.h")
target_sources("Editor-Input" PRIVATE "Action/WasdDelta.h")

#INCLUD BASE SRC FOLDER
target_include_directories("Editor-Input" PUBLIC "${PROJECT_SOURCE_DIR}/gab-engine")

#DEBUG
message("[GABENGINE] LOADED: Editor -> Input")