set(CURRENT_CMAKE_LIB gbe_editor)

#SUBDIRECTORY LOADING
add_library(${CURRENT_CMAKE_LIB} SHARED
	"Editor.cpp"

	"Component/Transform.cpp"

	"Global/Time.cpp"

	"Input/Implementations/KeyPressImplementation.cpp"
	"Input/Implementations/MouseScrollImplementation.cpp"
	"Input/Implementations/WasdDeltaImplementation.cpp"

	"ObjectHandlers/PhysicsHandler.cpp"
	"ObjectHandlers/Members/ContactResolver.cpp"

	"Objects/Object.cpp"
	"Objects/Root.cpp"
	"Objects/Controllers/ControllerBase.cpp"
	#"Objects/Controllers/FlyingCameraControl.cpp"
	#"Objects/Controllers/OrbitalControl.cpp"
	#"Objects/Controllers/SpacebarHitter.cpp"
	#"Objects/Controllers/SpacebarPauser.cpp"
	#"Objects/Controllers/VariableSwitcher.cpp"
	"Objects/Input/InputPlayer.cpp"
	"Objects/Physics/ForceVolume.cpp"
	"Objects/Physics/ParticleSystem.cpp"
	"Objects/Physics/RigidObject.cpp"
	"Objects/Physics/Collider/Collider.cpp"
	"Objects/Physics/Collision/Contact.cpp"
	"Objects/Physics/Joints/AnchorSpring.cpp"
	"Objects/Physics/Joints/Bungee.cpp"
	"Objects/Physics/Joints/ChainJoint.cpp"
	"Objects/Physics/Joints/Spinner.cpp"
	"Objects/Physics/Linking/RigidObjectLink.cpp"
	"Objects/Physics/Linking/Rod.cpp"
	"Objects/Physics/Particle/ParticleObject.cpp"
	"Objects/Physics/ParticleSystem/FloatParameter.cpp"
	"Objects/Physics/ParticleSystem/VectorParameter.cpp"
)
add_library(gbe::${CURRENT_CMAKE_LIB} ALIAS ${CURRENT_CMAKE_LIB})

target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_datatypes)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_graphics)
target_link_libraries(${CURRENT_CMAKE_LIB} PRIVATE gbe_window)

target_include_directories(${CURRENT_CMAKE_LIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
message("[GABENGINE] INCLUDED ROOT DIR TO EDITOR SRCS: " ${CMAKE_CURRENT_SOURCE_DIR})

message("[GABENGINE] LOADED: " ${CMAKE_CURRENT_SOURCE_DIR})