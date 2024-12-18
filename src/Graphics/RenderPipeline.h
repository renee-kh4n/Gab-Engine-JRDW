#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <vector>
#include <algorithm>
#include "Shader.h"
#include "Texture.h"
#include "DrawCall.h"
#include "Skybox.h"
#include "Light.h"
#include "Framebuffer.h"

#include <functional>

#include "../Datatypes/Vectors.h"
#include "../Datatypes/Matrix4.h"

namespace gbe {
	using namespace rendering;

	class RenderPipeline {
	private:
		Vector2Int resolution;
		Shader* postprocess;

		std::vector<DrawCall*> drawcalls;

		std::list<rendering::Light*> lights_this_frame;
		const int maxlights = 10;

		//--------CACHE--------//
		//SHADERS
		Shader* depthShader;

		//BUFFERS
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;
	public:
		RenderPipeline(void*(*procaddressfunc)(const char*), Vector2Int);
		void RegisterDrawCall(DrawCall*);

		void SetPostProcessing(Shader* postprocess);
		bool TryPushLight(rendering::Light* data, bool priority = false);

		void RenderFrame(Vector3& from, Vector3& forward, Matrix4& frustrum, float& nearclip, float& farclip);

		void CleanUp();
	};
}