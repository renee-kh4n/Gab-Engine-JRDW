#define GLM_ENABLE_EXPERIMENTAL
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "RenderPipeline.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

using namespace gbe;

gbe::RenderPipeline* gbe::RenderPipeline::Instance;

gbe::RenderPipeline* gbe::RenderPipeline::Get_Instance() {
	return gbe::RenderPipeline::Instance;
}

void gbe::RenderPipeline::Init() {
	this->Instance = this;
}

gbe::RenderPipeline::RenderPipeline(gbe::Window* window, Vector2Int dimensions)
{
	this->resolution = dimensions;

#pragma region SDL x VULKAN init
    auto implemented_window = static_cast<SDL_Window*>(window->Get_implemented_window());

    //EXTENSIONS
    uint32_t extensionCount;
    const char** extensionNames = 0;
    SDL_Vulkan_GetInstanceExtensions(implemented_window, &extensionCount, nullptr);
    extensionNames = new const char* [extensionCount];
    SDL_Vulkan_GetInstanceExtensions(implemented_window, &extensionCount, extensionNames);
    
    //APP INFO
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
    //VALIDATION LAYERS
    bool validationlayerssupported = true;

    const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
    };
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
    for (const char* layerName : validationLayers) {
        bool layerFound = false;

        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if (!layerFound) {
            validationlayerssupported = false;
        }
    }

    if (enableValidationLayers && !validationlayerssupported) {
        throw std::runtime_error("validation layers requested, but not available!");
    }
    
	//INSTANCE INFO
    VkInstanceCreateInfo instInfo{};
    instInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instInfo.pApplicationInfo = &appInfo;
    instInfo.pNext = nullptr;
    instInfo.flags = 0;
    instInfo.ppEnabledLayerNames = nullptr;
    instInfo.enabledExtensionCount = extensionCount;
    instInfo.ppEnabledExtensionNames = extensionNames;

    if (enableValidationLayers) {
        instInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instInfo.ppEnabledLayerNames = validationLayers.data();
    }
    else {
        instInfo.enabledLayerCount = 0;
    }
    
    //INSTANCE
    if (vkCreateInstance(&instInfo, nullptr, &vkInst) != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(vkInst, &physicalDeviceCount, nullptr);
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(vkInst, &physicalDeviceCount, physicalDevices.data());
    VkPhysicalDevice physicalDevice = physicalDevices[0];

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    //SURFACE
    SDL_Vulkan_CreateSurface(implemented_window, vkInst, &surface);

    uint32_t graphicsQueueIndex = UINT32_MAX;
    uint32_t presentQueueIndex = UINT32_MAX;
    VkBool32 support;
    uint32_t i = 0;
    for (VkQueueFamilyProperties queueFamily : queueFamilies) {
        if (graphicsQueueIndex == UINT32_MAX && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            graphicsQueueIndex = i;
        if (presentQueueIndex == UINT32_MAX) {
            vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &support);
            if (support)
                presentQueueIndex = i;
        }
        ++i;
    }

    float queuePriority = 1.0f;
    VkDeviceQueueCreateInfo queueInfo = {
        VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO, // sType
        nullptr,                                    // pNext
        0,                                          // flags
        graphicsQueueIndex,                         // graphicsQueueIndex
        1,                                          // queueCount
        &queuePriority,                             // pQueuePriorities
    };

    VkPhysicalDeviceFeatures deviceFeatures = {};
    const std::vector<const char*> deviceExtensionNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
    VkDeviceCreateInfo deviceInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // sType
        nullptr,                                            // pNext
        0,                                                  // flags
        1,                                                  // queueCreateInfoCount
        &queueInfo,                                         // pQueueCreateInfos
        0,                                                  // enabledLayerCount
        nullptr,                                            // ppEnabledLayerNames
        static_cast<uint32_t>(deviceExtensionNames.size()), // enabledExtensionCount
        deviceExtensionNames.data(),                        // ppEnabledExtensionNames
        &deviceFeatures,                                    // pEnabledFeatures
    };
    //DEVICE
    vkCreateDevice(physicalDevice, &deviceInfo, nullptr, &vkdevice);

    vkGetDeviceQueue(vkdevice, graphicsQueueIndex, 0, &graphicsQueue);

    vkGetDeviceQueue(vkdevice, presentQueueIndex, 0, &presentQueue);

    SDL_Log("Initialized with errors: %s", SDL_GetError());
#pragma endregion

#pragma region swapchain init
    VkSurfaceCapabilitiesKHR capabilities = {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, presentModes.data());
    }

    //format selection
    for (const auto& availableFormat : formats) {
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            this->chosenSurfaceFormat = availableFormat;
            break;
        }
    }

    this->chosenSurfaceFormat = formats[0];

    //presentation mode selection
	VkPresentModeKHR chosenPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    for (const auto& availablePresentMode : presentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            chosenPresentMode = availablePresentMode;
        }
    }

	//swapchain extent selection
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
        this->swapchainExtent = capabilities.currentExtent;
    }
    else {
        VkExtent2D actualExtent = {
            static_cast<uint32_t>(dimensions.x),
            static_cast<uint32_t>(dimensions.y)
        };

        actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        this->swapchainExtent = actualExtent;
    }

    uint32_t imageCount = capabilities.minImageCount + 1;
    if (capabilities.maxImageCount > 0 && imageCount > capabilities.maxImageCount) {
        imageCount = capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR swapchainInfo{};
    swapchainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchainInfo.surface = surface;

    swapchainInfo.minImageCount = imageCount;
    swapchainInfo.imageFormat = this->chosenSurfaceFormat.format;
    swapchainInfo.imageColorSpace = this->chosenSurfaceFormat.colorSpace;
    swapchainInfo.imageExtent = this->swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    uint32_t queueFamilyIndices[] = { graphicsQueueIndex, presentQueueIndex };

    if (graphicsQueueIndex != presentQueueIndex) {
        swapchainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchainInfo.queueFamilyIndexCount = 2;
        swapchainInfo.pQueueFamilyIndices = queueFamilyIndices;
    }
    else {
        swapchainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        swapchainInfo.queueFamilyIndexCount = 0; // Optional
        swapchainInfo.pQueueFamilyIndices = nullptr; // Optional
    }

    swapchainInfo.preTransform = capabilities.currentTransform;
    swapchainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    swapchainInfo.presentMode = chosenPresentMode;
    swapchainInfo.clipped = VK_TRUE;
    swapchainInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(vkdevice, &swapchainInfo, nullptr, &swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    //Retrieving the swap chain images
    vkGetSwapchainImagesKHR(vkdevice, this->swapChain, &imageCount, nullptr);
    this->swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(vkdevice, this->swapChain, &imageCount, swapChainImages.data());

    //Image views
    this->swapChainImageViews.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        VkImageViewCreateInfo imageViewInfo{};
        imageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        imageViewInfo.image = swapChainImages[i];

        imageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        imageViewInfo.format = this->chosenSurfaceFormat.format;

        imageViewInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        imageViewInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        imageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        imageViewInfo.subresourceRange.baseMipLevel = 0;
        imageViewInfo.subresourceRange.levelCount = 1;
        imageViewInfo.subresourceRange.baseArrayLayer = 0;
        imageViewInfo.subresourceRange.layerCount = 1;

        if (vkCreateImageView(vkdevice, &imageViewInfo, nullptr, &swapChainImageViews[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create image views!");
        }
    }
#pragma endregion

#pragma region Render pass
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = this->chosenSurfaceFormat.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    if (vkCreateRenderPass(this->vkdevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
#pragma endregion

#pragma region Framebuffer
	this->swapChainFramebuffers.resize(this->swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = attachments;
        framebufferInfo.width = this->swapchainExtent.width;
        framebufferInfo.height = this->swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(this->vkdevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
#pragma endregion

	//Asset Loaders
	this->textureloader.AssignSelfAsLoader();
    this->shaderloader.PassDependencies(&this->vkdevice, &this->swapchainExtent, &this->renderPass);
	this->shaderloader.AssignSelfAsLoader();
}

void RenderPipeline::SetCameraShader(asset::Shader* camshader) {
	
}
bool RenderPipeline::TryPushLight(gfx::Light* data, bool priority) {

	if (this->lights_this_frame.size() == this->maxlights)
		return false;

	if (priority) {
		this->lights_this_frame.push_front(data);
		return true;
	}

	this->lights_this_frame.push_back(data);
	return true;
}

void gbe::RenderPipeline::SetResolution(Vector2Int newresolution) {
	this->resolution = newresolution;

    throw new std::runtime_error("Not implemented yet");
}

void gbe::RenderPipeline::RenderFrame(Vector3& from, const Vector3& forward, Matrix4& _frustrum, float& nearclip, float& farclip)
{

}

unsigned int gbe::RenderPipeline::Get_mainbufferId()
{
	throw new std::runtime_error("Not implemented yet");
	return 0;
}

void gbe::RenderPipeline::RegisterDrawCall(DrawCall* drawcall)
{
	if (this->drawcalls.find(drawcall->order) == this->drawcalls.end()) {
		this->drawcalls.insert_or_assign(drawcall->order, std::vector<DrawCall*>{ drawcall });
	}
	else {
		this->drawcalls[drawcall->order].push_back(drawcall);
	}
}

void gbe::RenderPipeline::CleanUp()
{
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(this->vkdevice, framebuffer, nullptr);
    }

    vkDestroyRenderPass(vkdevice, renderPass, nullptr);

    vkDestroySurfaceKHR(vkInst, surface, nullptr);
    vkDestroyInstance(vkInst, nullptr);

    vkDestroySwapchainKHR(vkdevice, swapChain, nullptr);

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(vkdevice, imageView, nullptr);
    }

	for (auto it = drawcalls.begin(); it != drawcalls.end(); it++) {
		auto drawcallbatch = it->second;

		for (size_t o_i = 0; o_i < drawcallbatch.size(); o_i++)
		{
			glDeleteVertexArrays(1, &drawcallbatch[o_i]->m_mesh->VAO);
			glDeleteBuffers(1, &drawcallbatch[o_i]->m_mesh->VBO);
		}
	}
}
