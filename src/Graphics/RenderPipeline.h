#pragma once

#include <glm/glm.hpp>
#include <glad/glad.h>
#include <glm/gtx/matrix_decompose.hpp>

#include <vulkan/vulkan.h>

#include <vector>
#include <algorithm>
#include <map>
#include <set>
#include <string>

#include <functional>

#include "Math/gbe_math.h"
#include "Asset/gbe_asset.h"

#include "Data/DrawCall.h"
#include "Data/Skybox.h"
#include "Data/Light.h"
#include "Data/Framebuffer.h"
#include "AssetLoaders/TextureLoader.h"
#include "AssetLoaders/ShaderLoader.h"
#include "AssetLoaders/MeshLoader.h"
#include "AssetLoaders/MaterialLoader.h"
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
		MeshLoader meshloader;
		TextureLoader textureloader;
		MaterialLoader materialloader;

		//VK Init
		Window* window;
		VkInstance vkInst;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkSurfaceKHR vksurface;
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
		void querySwapChainSupport(VkPhysicalDevice pvkdevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities, std::vector<VkSurfaceFormatKHR>& formats, std::vector<VkPresentModeKHR>& presentModes);

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
		static void beginSingleTimeCommands(VkCommandBuffer& buffer);
		static void endSingleTimeCommands(VkCommandBuffer& commandBuffer);
		static void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		static void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		static uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		static void createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		static void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
		static void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		static void createImageView(VkImageView& imageview, VkImage image, VkFormat format);

		RenderPipeline(gbe::Window*, Vector2Int);
		DrawCall* RegisterDrawCall(asset::Mesh* mesh, asset::Material* material);

		void SetCameraShader(asset::Shader* postprocess);
		bool TryPushLight(gfx::Light* data, bool priority = false);

		void SetResolution(Vector2Int newresolution);
		
		void RenderFrame(Matrix4 viewmat, Matrix4 projmat, float& nearclip, float& farclip);

		void CleanUp();
	};
}