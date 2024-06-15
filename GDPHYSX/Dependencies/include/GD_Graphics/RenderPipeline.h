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
	class RenderPipeline {
	private:
		Shader* depthShader;
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;

		double lastFrameT = 0.0f;
		double currentFrameT = 0.0f;
		double deltaTime = 0.0f;

		std::vector<DrawCall*> drawcalls;

		Skybox* mSkybox;
	public:
		RenderPipeline(glm::vec2);
		void RegisterDrawCall(DrawCall*);
		void RenderFrame(glm::vec3 from, glm::mat4 viewMat, glm::mat4 projMat, Shader* postprocess);
		void CleanUp();
	};
}