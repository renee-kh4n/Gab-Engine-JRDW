#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

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

namespace gde {
	using namespace rendering;

	class RenderPipeline {
	private:
		Shader* depthShader;
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;

		glm::vec3 from;
		glm::mat4 viewMat;
		glm::mat4 projMat;
		Shader* postprocess;

		std::vector<DrawCall*> drawcalls;

		std::list<rendering::Light*> lights_this_frame;
		int maxlights = 10;

		Skybox* mSkybox;

		void SetRequiredAttribs();
	public:
		RenderPipeline(glm::vec2);
		void RegisterDrawCall(DrawCall*);

		void SetMaximumLights(int maxlights);
		void SetView(glm::vec3 from, glm::mat4 viewMat, glm::mat4 projMat);
		void SetPostProcessing(Shader* postprocess);
		bool TryPushLight(rendering::Light* data, bool priority = false);

		void RenderFrame();

		glm::mat4 GetProjMat();

		void CleanUp();
	};
}