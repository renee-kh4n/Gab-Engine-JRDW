#pragma once

#include "../../Object.h"
#include "../Physics/ColliderData/ColliderData.h"

namespace gbe {
	class Collider : public Object {
	private:
		
	public:
		virtual physics::ColliderData* GetColliderData() = 0;

		void OnChangeMatrix() override;
	};
}