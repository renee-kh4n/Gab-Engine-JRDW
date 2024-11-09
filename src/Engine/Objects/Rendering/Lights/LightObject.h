#pragma once

#include "../../Object.h"
#include "../Graphics/Light.h"

namespace gbe {

	class LightObject : public Object{
	protected:
		rendering::Light* mLight;
		
		Vector3 old_position;
		Vector3 old_forward;
		bool changed = true;
		
		Vector3 Color;
		float intensity;

	public:
		void Set_Color(Vector3 color);
		float Get_Intensity();
		void Set_Intensity(float intensity);

		bool CheckChanged();

		virtual rendering::Light* GetData() = 0;
	};
}