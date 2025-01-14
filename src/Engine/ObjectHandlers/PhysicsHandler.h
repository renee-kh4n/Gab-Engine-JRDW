#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <list>

#include "Physics/gbe_physics.h"
#include "ObjectHandler.h"
#include "Engine/Objects/Physics/RigidObject.h"
#include "Engine/Objects/Physics/ForceVolume.h"

namespace gbe {
	class PhysicsHandler : public ObjectHandler<RigidObject>{
	private:
		ObjectHandler<ForceVolume> forcevolume_handler;
		physics::PhysicsPipeline* mPipeline;
	public:

		PhysicsHandler(physics::PhysicsPipeline*);

		void Update();

		virtual void OnAdd(RigidObject*) override;
		virtual void OnRemove(RigidObject*) override;
	};
}