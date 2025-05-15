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
		Window* window;
		VkInstance vkInst;
		VkSurfaceKHR surface;
		VkDevice vkdevice;
		VkPhysicalDevice vkphysicalDevice;
		uint32_t graphicsQueueIndex = UINT32_MAX;
		uint32_t presentQueueIndex = UINT32_MAX;
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

		//Renderpass
		VkRenderPass renderPass;
		
		void InitializePipelineObjects();
		void RefreshPipelineObjects();
		void CleanPipelineObjects();
		
		//COMMANDPOOL
		const int MAX_FRAMES_IN_FLIGHT = 2;
		uint32_t currentFrame = 0;
		VkCommandPool commandPool;
		std::vector<VkCommandBuffer> commandBuffers;
		//Synchronization
		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		
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