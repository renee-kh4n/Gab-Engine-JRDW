#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <vector>
#include <algorithm>

#include <functional>

#include "Math/gbe_math.h"
#include "Asset/gbe_asset.h"

#include "Data/DrawCall.h"
#include "Data/Skybox.h"
#include "Data/Light.h"
#include "Data/Framebuffer.h"
#include "AssetLoaders/TextureLoader.h"
#include "AssetLoaders/ShaderLoader.h"

namespace gbe {
	using namespace gfx;

	class RenderPipeline {
	private:
		static RenderPipeline* Instance;

		Vector2Int resolution;
		asset::AssetReference<asset::Shader> camera_shader;

		std::vector<DrawCall*> drawcalls;

		std::list<gfx::Light*> lights_this_frame;
		const int maxlights = 10;

		//--------CACHE--------//
		//Loaders
		ShaderLoader shaderloader;
		TextureLoader textureloader;
		
		//SHADERS
		asset::AssetReference<asset::Shader> depth_shader;

		//BUFFERS
		Framebuffer* mFrameBuffer;
		Framebuffer* mDepthFrameBuffer;
	public:
		static RenderPipeline* Get_Instance();

		RenderPipeline(void*(*procaddressfunc)(const char*), Vector2Int);
		void RegisterDrawCall(DrawCall*);

		void SetCameraShader(asset::Shader* postprocess);
		bool TryPushLight(gfx::Light* data, bool priority = false);

		void SetResolution(Vector2Int newresolution);
		void RenderFrame(Vector3& from, const Vector3& forward, Matrix4& frustrum, float& nearclip, float& farclip);

		void CleanUp();
		void Init();
	};
}