set(CURRENT_CMAKE_LIB gbe_gui)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"gb_canvas.cpp"
 "gb_rect.cpp" 
 "rect_types/gb_image.h" 
 "gbuiPipeline.h" 
 "gbuiPipeline.cpp"
)

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PUBLIC glm::glm)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glad::glad)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})