set(CURRENT_CMAKE_LIB gbe_window)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Window.cpp"
 "WindowEvents.h" "gbe_window.h")

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB}
    PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})