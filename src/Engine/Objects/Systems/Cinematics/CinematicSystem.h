#pragma once

#include "../../Object.h"
#include "../../Rendering/Camera.h"
#include "../../../ObjectFunctions/Update.h"

#include "Math/gbe_math.h"
#include "Graphics/gbe_graphics.h"

#include <vector>

namespace gbe {
	class CinematicSystem : public Object, public Update{
	private:
		struct CameraSequenceNode {
			Vector3 position;
			Quaternion rotation;
			float duration; // Duration in seconds for this node
		};;

		bool is_playing = false;

		Camera* cur_cam;
		float sequence_t = 0;
		std::vector<CameraSequenceNode> path;

		//DRAWCALLS
		gbe::DrawCall* node_dracall;
	public:
		CinematicSystem();

		void StartCameraMovement();
		void SpawnNode();
		void InvokeUpdate(float deltatime) override;
	};
}