#LIBRARY LOADING
add_library(Window "Window.cpp" "Window.h")

#LIBRARY-SPECIFIC DEPENDENCY LOADING
find_package(SDL2 REQUIRED)
target_link_libraries(Window SDL2::SDL2)

#SET DEFAULT PROJECT INCLUDES
include_gabengine_directories(Window)

#DEBUG
message("[GABENGINE] LOADED: Window")