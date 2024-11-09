#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <vector>
#include "Shader.h"
#include "Texture.h"
#include "DrawCall.h"
#include "Skybox.h"
#include "Light.h"
#include "Framebuffer.h"

#include <functional>

#include "../Datatypes/Vector3.h"
#include "../Datatypes/Matrix4.h"

namespace gbe {
	using namespace rendering;

	class RenderPipeline {
	private:
		Shader* depthShader;
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;

		Vector3 from;
		Matrix4 viewMat;
		Matrix4 projMat;
		Shader* postprocess;

		std::vector<DrawCall*> drawcalls;

		std::list<rendering::Light*> lights_this_frame;
		int maxlights = 10;

		Skybox* mSkybox;

		void SetRequiredAttribs();
	public:
		RenderPipeline(void*(*procaddressfunc)(const char*), glm::vec2);
		void RegisterDrawCall(DrawCall*);

		void SetMaximumLights(int maxlights);
		void SetView(glm::vec3 from, glm::mat4 viewMat, glm::mat4 projMat);
		void SetPostProcessing(Shader* postprocess);
		bool TryPushLight(rendering::Light* data, bool priority = false);

		void RenderFrame();

		Matrix4 GetProjMat();

		void CleanUp();
	};
}