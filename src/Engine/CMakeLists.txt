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
	"Objects/Controllers/GenericController.cpp"
	
	
	"Objects/Input/InputPlayer.cpp"

	"Objects/Physics/ForceVolume.cpp"
	"Objects/Rendering/Lights/DirectionalLight.cpp"
	"Objects/Rendering/Lights/LightObject.cpp"
	"Objects/Rendering/Renderer/LineRenderer.cpp"
	"Objects/Rendering/Camera.cpp"
	"Objects/Rendering/RenderObject.cpp"

	"Objects/Physics/RigidObject.cpp"
	"Objects/Physics/Collider/Collider.cpp"
	"Objects/Physics/Collider/SphereCollider.cpp"
	"Objects/Physics/Collider/BoxCollider.cpp"
	"Objects/Physics/PhysicsObject.cpp"
	"Objects/Physics/TriggerRigidObject.cpp"

 "Objects/GenericObject.cpp" "Objects/Controllers/TPCameraController.cpp" "Objects/Systems/ParticleSystem.cpp" "Objects/Physics/Collider/MeshCollider.cpp" "Input/Implementations/MouseDeltaImplementation.h")

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_graphics)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_math)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_window)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_gui)

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})