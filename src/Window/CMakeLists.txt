set(CURRENT_CMAKE_LIB gbe_window)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} SHARED
	"Window.cpp"
)
add_library(gbe::${CURRENT_CMAKE_LIB} ALIAS ${CURRENT_CMAKE_LIB})

find_package(SDL2 CONFIG REQUIRED)
    target_link_libraries(${CURRENT_CMAKE_LIB}
        PRIVATE
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    )

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_datatypes)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})