#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <algorithm>
#include <map>

#include <functional>

#include "Math/gbe_math.h"
#include "Asset/gbe_asset.h"

#include "Data/DrawCall.h"
#include "Data/Skybox.h"
#include "Data/Light.h"
#include "Data/Framebuffer.h"
#include "AssetLoaders/TextureLoader.h"
#include "AssetLoaders/ShaderLoader.h"
#include "Window/gbe_window.h"

namespace gbe {
	using namespace gfx;

	class RenderPipeline {
	private:
		static RenderPipeline* Instance;

		Vector2Int resolution;

		std::map<int, std::vector<DrawCall*>> drawcalls;

		std::list<gfx::Light*> lights_this_frame;
		const int maxlights = 10;

		//--------CACHE--------//
		//Loaders
		ShaderLoader shaderloader;
		TextureLoader textureloader;

		//VK Init
		VkInstance vkInst;
		VkSurfaceKHR surface;
		VkDevice vkdevice;
		VkQueue graphicsQueue;
		VkQueue presentQueue;

		//SWAPCHAINS
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		VkSurfaceFormatKHR chosenSurfaceFormat;
		VkExtent2D swapchainExtent;
		std::vector<VkImageView> swapChainImageViews;
		//SWAPCHAIN FRAMEBUFFERS
		std::vector<VkFramebuffer> swapChainFramebuffers;

		//COMMANDPOOL
		VkCommandPool commandPool;
		VkCommandBuffer commandBuffer;
		void recordCommandBuffer(VkCommandBuffer commandBuffer, unsigned int imageIndex);

		//Renderpass
		VkRenderPass renderPass;
	public:
		static RenderPipeline* Get_Instance();

		RenderPipeline(gbe::Window*, Vector2Int);
		void RegisterDrawCall(DrawCall*);

		void SetCameraShader(asset::Shader* postprocess);
		bool TryPushLight(gfx::Light* data, bool priority = false);

		void SetResolution(Vector2Int newresolution);
		
		void RenderFrame(Vector3& from, const Vector3& forward, Matrix4& frustrum, float& nearclip, float& farclip);
		unsigned int Get_mainbufferId();

		void CleanUp();
		void Init();
	};
}