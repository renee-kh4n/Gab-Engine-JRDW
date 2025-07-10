#pragma once

#include <glm/gtx/quaternion.hpp>

#include <list>

#include "Physics/gbe_physics.h"
#include "ObjectHandler.h"
#include "Engine/Objects/Physics/RigidObject.h"
#include "Engine/Objects/Physics/ForceVolume.h"
#include <unordered_map>
#include <functional>

namespace gbe {
	class ColliderHandler : public ObjectHandler<Collider> {
	private:
		physics::PhysicsPipeline* mPipeline;
	public:

		ColliderHandler(physics::PhysicsPipeline*);

		virtual void OnAdd(Collider*) override;
		virtual void OnRemove(Collider*) override;
	};
}