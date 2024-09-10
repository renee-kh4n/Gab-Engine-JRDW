#LIBRARY LOADING
add_library(Datatypes "Vectors.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector_base.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector2.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector2.cpp")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector2Int.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector2Int.cpp")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector3.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector3.cpp")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector4.h")
target_sources(Datatypes PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Vector4.cpp")

#SET DEFAULT PROJECT INCLUDES
include_gabengine_directories(Datatypes)

#SET LIBRARY-SPECFIC DEPENDENCIES
#find_package(glm REQUIRED)
#target_link_libraries(Datatypes PUBLIC glm::glm)

#DEBUG
message("[GABENGINE] LOADED: Datatypes")