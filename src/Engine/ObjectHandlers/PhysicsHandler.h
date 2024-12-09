#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <list>
#include "ObjectHandler.h"
#include "../Objects/Physics/Physics.h"
#include "../Objects/Physics/ForceVolume.h"
#include "../Physics/PhysicsPipeline.h"

namespace gbe {
	class PhysicsHandler : public ObjectHandler<RigidObject>{
	private:
		ObjectHandler<ForceVolume> forcevolume_handler;
		physics::PhysicsPipeline* mPipeline;
	public:

		PhysicsHandler();

		void SetPipeline(physics::PhysicsPipeline*);

		void Update(double duration);

		virtual void OnAdd(RigidObject*) override;
		virtual void OnRemove(RigidObject*) override;
	};
}