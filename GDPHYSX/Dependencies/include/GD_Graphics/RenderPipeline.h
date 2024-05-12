#pragma once

#include <vector>
#include "util.h"
#include "Shader.h"
#include "Texture.h"
#include "DrawCall.h"
#include "Window.h"
#include "Camera.h"
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

		Camera* activeCamera;
		Skybox* mSkybox;
	public:
		RenderPipeline(glm::vec2, Camera*);
		void RegisterDrawCall(DrawCall*);
		void RenderFrame();
		void CleanUp();
	};
}