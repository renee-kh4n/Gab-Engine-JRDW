set(CURRENT_CMAKE_LIB gbe_engine)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} STATIC
	"Engine.cpp"

	"Component/Transform.cpp"

	"Global/Time.cpp"

	"Input/Implementations/KeyPressImplementation.cpp"
	"Input/Implementations/MouseScrollImplementation.cpp"
	"Input/Implementations/WasdDeltaImplementation.cpp"

	"ObjectHandlers/PhysicsHandler.cpp"

	"Objects/Object.cpp"
	"Objects/Root.cpp"
	"Objects/Controllers/ControllerBase.cpp"
	"Objects/Controllers/FlyingCameraControl.cpp"
	"Objects/Controllers/OrbitalControl.cpp"
	
	"Objects/Controllers/VariableSwitcher.cpp"
	"Objects/Input/InputPlayer.cpp"
	"Objects/Physics/ForceVolume.cpp"
	
	"Objects/Physics/RigidObject.cpp"
	"Objects/Physics/Collider/Collider.cpp"
	"Objects/Physics/Joints/AnchorSpring.cpp"
	"Objects/Physics/Joints/Bungee.cpp"
	"Objects/Physics/Joints/ChainJoint.cpp"
	"Objects/Physics/Joints/Spinner.cpp"
	"Objects/Rendering/Lights/DirectionalLight.cpp"
	"Objects/Rendering/Lights/LightObject.cpp"
	"Objects/Rendering/Renderer/LineRenderer.cpp"
	"Objects/Rendering/Camera.cpp"
	"Objects/Rendering/RenderObject.cpp"
)

target_include_directories(${CURRENT_CMAKE_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message("[GABENGINE] INCLUDED ROOT DIR TO EDITOR SRCS: " ${CMAKE_CURRENT_SOURCE_DIR})

find_package(glm CONFIG REQUIRED)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE glm::glm)

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_graphics)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})