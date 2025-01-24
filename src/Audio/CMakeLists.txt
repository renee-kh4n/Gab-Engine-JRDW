set(CURRENT_CMAKE_LIB gbe_audio)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"AudioPipeline.cpp"
 "AudioLoader.cpp")

find_package(SDL2 CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB}
    PUBLIC
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
)
find_package(SDL2_mixer CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PUBLIC ${SDL2IMAGE_LIBRARIES})

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})