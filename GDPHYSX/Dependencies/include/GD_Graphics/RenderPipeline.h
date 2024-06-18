#pragma once

#include <vector>
#include "util.h"
#include "Shader.h"
#include "Texture.h"
#include "DrawCall.h"
#include "Window.h"
#include "Skybox.h"
#include "Light.h"
#include "Framebuffer.h"

namespace gde {
	using namespace rendering;

	class RenderPipeline {
	private:
		Shader* depthShader;
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;

		double lastFrameT = 0.0f;
		double currentFrameT = 0.0f;
		double deltaTime = 0.0f;

		glm::vec3 from;
		glm::mat4 viewMat;
		glm::mat4 projMat;
		Shader* postprocess;

		std::vector<DrawCall*> drawcalls;

		std::list<rendering::Light*> lights_this_frame;
		int maxlights = 10;

		Skybox* mSkybox;
	public:
		RenderPipeline(glm::vec2);
		void RegisterDrawCall(DrawCall*);

		void SetMaximumLights(int maxlights);
		void SetView(glm::vec3 from, glm::mat4 viewMat, glm::mat4 projMat);
		void SetPostProcessing(Shader* postprocess);
		bool TryPushLight(rendering::Light* data, bool priority = false);

		void RenderFrame();

		void CleanUp();
	};
}