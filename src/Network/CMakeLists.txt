set(CURRENT_CMAKE_LIB gbe_network)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Network.cpp"
)

#LINKING DEPENDENCIES

target_link_libraries(${CURRENT_CMAKE_LIB} PUBLIC gbe_math)
target_link_libraries(${CURRENT_CMAKE_LIB} PUBLIC gbe_asset)

#RECURSIVE
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_engine)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})