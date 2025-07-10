//PUT ODR DEFINES HERE AS THIS FILE IS THE FIRST FILE TO BE COMPILED IN THE RENDER PIPELINE
#define TINYOBJLOADER_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#include "RenderPipeline.h"

#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#include "Editor/gbe_editor.h"

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

void gbe::RenderPipeline::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(Instance->vkdevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(Instance->vkdevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = Instance->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Instance->vkdevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    vkBindBufferMemory(Instance->vkdevice, buffer, bufferMemory, 0);
}

void gbe::RenderPipeline::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer;
	beginSingleTimeCommands(commandBuffer);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    endSingleTimeCommands(commandBuffer);
}

uint32_t gbe::RenderPipeline::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(Instance->vkphysicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}

void gbe::RenderPipeline::createImage(uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    imageInfo.extent.depth = 1;
    imageInfo.mipLevels = 1;
    imageInfo.arrayLayers = 1;
    imageInfo.format = format;
    imageInfo.tiling = tiling;
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageInfo.usage = usage;
    imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateImage(Instance->vkdevice, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(Instance->vkdevice, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(Instance->vkdevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(Instance->vkdevice, image, imageMemory, 0);
}

void gbe::RenderPipeline::transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
{
    VkCommandBuffer commandBuffer;
    beginSingleTimeCommands(commandBuffer);

    VkImageMemoryBarrier barrier{};
    barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout = oldLayout;
    barrier.newLayout = newLayout;

    barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    barrier.image = image;
    barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel = 0;
    barrier.subresourceRange.levelCount = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;

        if (format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        sourceStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_PRESENT_SRC_KHR) {
        barrier.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

        sourceStage = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
        destinationStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(
        commandBuffer,
        sourceStage, destinationStage,
        0,
        0, nullptr,
        0, nullptr,
        1, &barrier
    );



    endSingleTimeCommands(commandBuffer);
}

void gbe::RenderPipeline::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer;
    beginSingleTimeCommands(commandBuffer);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(commandBuffer);
}

void gbe::RenderPipeline::copyImageToBuffer(VkImage image, VkFormat imageformat, VkImageLayout imagelayout, VkBuffer buffer, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer;
    beginSingleTimeCommands(commandBuffer);

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;

    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    transitionImageLayout(image, imageformat, imagelayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);

    vkCmdCopyImageToBuffer(
        commandBuffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        buffer,
        1,
        &region
    );

    //transitionImageLayout(image, imageformat, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, imagelayout);

    endSingleTimeCommands(commandBuffer);
}

void gbe::RenderPipeline::createImageView(VkImageView& imageview, VkImage image, VkFormat format, VkImageAspectFlags aspectflags)
{
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectflags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    if (vkCreateImageView(Instance->vkdevice, &viewInfo, nullptr, &imageview) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }
}

void gbe::RenderPipeline::beginSingleTimeCommands(VkCommandBuffer& _commandBuffer)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = Instance->commandPool;
    allocInfo.commandBufferCount = 1;

    vkAllocateCommandBuffers(Instance->vkdevice, &allocInfo, &_commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(_commandBuffer, &beginInfo);
}

void gbe::RenderPipeline::endSingleTimeCommands(VkCommandBuffer& commandBuffer)
{
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(Instance->graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(Instance->graphicsQueue);

    vkFreeCommandBuffers(Instance->vkdevice, Instance->commandPool, 1, &commandBuffer);
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    void* pUserData) {

    if (messageSeverity > VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
        std::cerr << "validation ERROR: " << pCallbackData->pMessage << std::endl;
		throw std::runtime_error("Vulkan validation layer error encountered: " + std::string(pCallbackData->pMessage));
    }
    else {
        std::cerr << "validation log: " << pCallbackData->pMessage << std::endl;
    }

    return VK_FALSE;
}

gbe::RenderPipeline::RenderPipeline(gbe::Window* window, Vector2Int dimensions)
{
    if(this->Instance != nullptr)
		throw std::runtime_error("RenderPipeline instance already exists!");

    this->Instance = this;

	this->window = window;
	this->resolution = dimensions;

#pragma region SDL x VULKAN init
    auto implemented_window = static_cast<SDL_Window*>(window->Get_implemented_window());
    
    //EXTENSIONS
    uint32_t SDLextensionCount;
    const char** SDLextensionNames = 0;
    SDL_Vulkan_GetInstanceExtensions(implemented_window, &SDLextensionCount, nullptr);
    SDLextensionNames = new const char* [SDLextensionCount];
    SDL_Vulkan_GetInstanceExtensions(implemented_window, &SDLextensionCount, SDLextensionNames);
    std::vector<const char*> allextensions(SDLextensionNames, SDLextensionNames + SDLextensionCount);
    if (enableValidationLayers) {
        allextensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    //allextensions.push_back(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);

    //APP INFO
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    
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
    instInfo.enabledExtensionCount = static_cast<uint32_t>(allextensions.size());;
    instInfo.ppEnabledExtensionNames = allextensions.data();

    //DEBUG
    VkDebugUtilsMessengerCreateInfoEXT debug_messenger_create_info{};
    if (enableValidationLayers) {
        instInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
        instInfo.ppEnabledLayerNames = validationLayers.data();

        debug_messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        debug_messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        debug_messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        debug_messenger_create_info.pfnUserCallback = debugCallback;
        debug_messenger_create_info.pUserData = nullptr; // Optional

        instInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debug_messenger_create_info;
    }
    else {
        instInfo.enabledLayerCount = 0;

		instInfo.pNext = nullptr;
    }
    
    //INSTANCE
    auto instresult = vkCreateInstance(&instInfo, nullptr, &vkInst);
    if (instresult != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance!");
    }

    //SDL SURFACE
    SDL_Vulkan_CreateSurface(implemented_window, vkInst, &vksurface);

    //DEBUG
    if (enableValidationLayers) {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkInst, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(vkInst, &debug_messenger_create_info, nullptr, &this->debugMessenger);
        }
        else {
            throw std::runtime_error("failed to set up debug messenger!");
        }
    }

    //===================DEVICE SET UP===================//
    //EXTENSIONS
    const std::vector<const char*> deviceExtensionNames = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        VK_KHR_MAINTENANCE1_EXTENSION_NAME
    };

    bool founddevice = false;
    uint32_t physicalDeviceCount;
    vkEnumeratePhysicalDevices(vkInst, &physicalDeviceCount, nullptr);
    if (physicalDeviceCount == 0) {
        throw std::runtime_error("failed to find GPUs with Vulkan support!");
    }
    std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
    vkEnumeratePhysicalDevices(vkInst, &physicalDeviceCount, physicalDevices.data());
    //TEST DEVICE SUITABLE
    const auto isDeviceSuitable = [=](VkPhysicalDevice vkpdevice) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(vkpdevice, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(vkpdevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensionNames.begin(), deviceExtensionNames.end());

        for (const auto& extension : availableExtensions) {
            requiredExtensions.erase(extension.extensionName);
        }
		bool extensionsSupported = requiredExtensions.empty();

        bool swapChainAdequate = false;
        if (extensionsSupported) {
            VkSurfaceCapabilitiesKHR capabilities = {};
            std::vector<VkSurfaceFormatKHR> formats;
            std::vector<VkPresentModeKHR> presentModes;
            querySwapChainSupport(vkpdevice, this->vksurface, capabilities, formats, presentModes);
            swapChainAdequate = !formats.empty() && !presentModes.empty();
        }

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(vkpdevice, &supportedFeatures);

        return extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
        };
    for (const auto& vkpdevice : physicalDevices) {
        if (isDeviceSuitable(vkpdevice)) {
            this->vkphysicalDevice = vkpdevice;
            founddevice = true;
            break;
        }
    }

    if (founddevice == false) {
        throw std::runtime_error("failed to find a suitable GPU!");
    }

    uint32_t queueFamilyCount;
    vkGetPhysicalDeviceQueueFamilyProperties(this->vkphysicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(this->vkphysicalDevice, &queueFamilyCount, queueFamilies.data());

    VkBool32 support;
    uint32_t i = 0;
    for (const VkQueueFamilyProperties& queueFamily : queueFamilies) {
        if (graphicsQueueIndex == UINT32_MAX && queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
            graphicsQueueIndex = i;
        if (presentQueueIndex == UINT32_MAX) {
            vkGetPhysicalDeviceSurfaceSupportKHR(this->vkphysicalDevice, i, vksurface, &support);
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

    //FEATURES SETUP
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.fillModeNonSolid = VK_TRUE;

    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features = {};
    shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
    shader_draw_parameters_features.pNext = nullptr;
    shader_draw_parameters_features.shaderDrawParameters = VK_TRUE;

    VkDeviceCreateInfo deviceInfo = {
        VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,               // sType
        &shader_draw_parameters_features,                   // pNext
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
    vkCreateDevice(this->vkphysicalDevice, &deviceInfo, nullptr, &vkdevice);

    vkGetDeviceQueue(vkdevice, graphicsQueueIndex, 0, &graphicsQueue);
    vkGetDeviceQueue(vkdevice, presentQueueIndex, 0, &presentQueue);

    if (graphicsQueueIndex == UINT32_MAX || presentQueueIndex == UINT32_MAX) {
		throw std::runtime_error("failed to find a suitable queue family!");
    }
#pragma endregion

    this->InitializePipelineObjects();

#pragma region command pool
    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = graphicsQueueIndex;

    if (vkCreateCommandPool(this->vkdevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }

    commandBuffers.resize(this->MAX_FRAMES_IN_FLIGHT);

    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    if (vkAllocateCommandBuffers(this->vkdevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
#pragma endregion

    this->CreateDepthResources();
    this->InitializeFramebuffers();

#pragma region synchronization
    this->imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    this->inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(this->vkdevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(this->vkdevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(this->vkdevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {

            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
#pragma endregion

	//Asset Loaders
    this->shaderloader.AssignSelfAsLoader();
    this->shaderloader.PassDependencies(&this->vkdevice, &this->swapchainExtent, &this->renderPass);
	
    this->meshloader.AssignSelfAsLoader();
	this->meshloader.PassDependencies(&this->vkdevice, &this->vkphysicalDevice);
	
    this->materialloader.AssignSelfAsLoader();
    this->materialloader.PassDependencies(&this->shaderloader);

    this->textureloader.AssignSelfAsLoader();
    this->textureloader.PassDependencies(&this->vkdevice, &this->vkphysicalDevice);

    //Assigning pipeline specific variables
    this->PipelineVariables.insert_or_assign("VkInstance", &vkInst);
    this->PipelineVariables.insert_or_assign("VkSurfaceKHR", &vksurface);
    this->PipelineVariables.insert_or_assign("VkDevice", &vkdevice);
    this->PipelineVariables.insert_or_assign("VkPhysicalDevice", &vkphysicalDevice);
    this->PipelineVariables.insert_or_assign("VkRenderPass", &renderPass);
    this->PipelineVariables.insert_or_assign("VkQueue_graphics", &graphicsQueue);
    this->PipelineVariables.insert_or_assign("VkQueue_present", &presentQueue);
}

void gbe::RenderPipeline::querySwapChainSupport(VkPhysicalDevice pvkdevice, VkSurfaceKHR surface, VkSurfaceCapabilitiesKHR& capabilities, std::vector<VkSurfaceFormatKHR>& formats, std::vector<VkPresentModeKHR>& presentModes) {
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pvkdevice, surface, &capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(pvkdevice, surface, &formatCount, nullptr);

    if (formatCount != 0) {
        formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(pvkdevice, surface, &formatCount, formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(pvkdevice, surface, &presentModeCount, nullptr);

    if (presentModeCount != 0) {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(pvkdevice, surface, &presentModeCount, presentModes.data());
    }
}

void gbe::RenderPipeline::insertImageMemoryBarrier(VkCommandBuffer cmdbuffer, VkImage image, VkAccessFlags srcAccessMask, VkAccessFlags dstAccessMask, VkImageLayout oldImageLayout, VkImageLayout newImageLayout, VkPipelineStageFlags srcStageMask, VkPipelineStageFlags dstStageMask, VkImageSubresourceRange subresourceRange)
{
    VkImageMemoryBarrier imageMemoryBarrier{};
    imageMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;

    imageMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    imageMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

    imageMemoryBarrier.srcAccessMask = srcAccessMask;
    imageMemoryBarrier.dstAccessMask = dstAccessMask;
    imageMemoryBarrier.oldLayout = oldImageLayout;
    imageMemoryBarrier.newLayout = newImageLayout;
    imageMemoryBarrier.image = image;
    imageMemoryBarrier.subresourceRange = subresourceRange;

    vkCmdPipelineBarrier(
        cmdbuffer,
        srcStageMask,
        dstStageMask,
        0,
        0, nullptr,
        0, nullptr,
        1, &imageMemoryBarrier);
}

void gbe::RenderPipeline::InitializePipelineObjects() {

#pragma region swapchain init
    VkSurfaceCapabilitiesKHR capabilities = {};
    std::vector<VkSurfaceFormatKHR> formats;
    std::vector<VkPresentModeKHR> presentModes;

	querySwapChainSupport(this->vkphysicalDevice, vksurface, capabilities, formats, presentModes);

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
            static_cast<uint32_t>(this->resolution.x),
            static_cast<uint32_t>(this->resolution.y)
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
    swapchainInfo.surface = vksurface;

    swapchainInfo.minImageCount = imageCount;
    swapchainInfo.imageFormat = this->chosenSurfaceFormat.format;
    swapchainInfo.imageColorSpace = this->chosenSurfaceFormat.colorSpace;
    swapchainInfo.imageExtent = this->swapchainExtent;
    swapchainInfo.imageArrayLayers = 1;
    swapchainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

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
    swapChainImageViews.resize(swapChainImages.size());

    for (uint32_t i = 0; i < swapChainImages.size(); i++) {
        createImageView(swapChainImageViews[i], swapChainImages[i], chosenSurfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
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

    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
    depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;

    //Subpass dependencies
    VkSubpassDependency dependency{};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;

    dependency.srcAccessMask = 0;

    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

    //Renderpass
    std::array<VkAttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;

    if (vkCreateRenderPass(this->vkdevice, &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }
#pragma endregion
}

void gbe::RenderPipeline::AssignEditor(Editor* _editor)
{
    this->editor = _editor;
}

void RenderPipeline::SetCameraShader(asset::Shader* camshader) {
	
}

void gbe::RenderPipeline::SetResolution(Vector2Int newresolution) {
	this->resolution = newresolution;
}


//========== RUNTIME THINGS ==========//
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

void gbe::RenderPipeline::RenderFrame(Matrix4 viewmat, Matrix4 projmat, float& nearclip, float& farclip)
{
	//Syncronization
    vkWaitForFences(this->vkdevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;
    VkResult aquireResult = vkAcquireNextImageKHR(this->vkdevice, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (aquireResult == VK_ERROR_OUT_OF_DATE_KHR) {
        this->RefreshPipelineObjects();
        return;
    }
    else if (aquireResult != VK_SUCCESS && aquireResult != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }

    vkResetFences(this->vkdevice, 1, &inFlightFences[currentFrame]);

	auto currentCommandBuffer = commandBuffers[currentFrame];
    vkResetCommandBuffer(currentCommandBuffer, 0);

#pragma region command buffer recording
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = 0; // Optional
    beginInfo.pInheritanceInfo = nullptr; // Optional

    if (vkBeginCommandBuffer(currentCommandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }

    //Start render pass
    VkRenderPassBeginInfo renderPassBeginInfo{};
    renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassBeginInfo.renderPass = renderPass;
    renderPassBeginInfo.framebuffer = swapChainFramebuffers[imageIndex];

    renderPassBeginInfo.renderArea.offset = { 0, 0 };
    renderPassBeginInfo.renderArea.extent = this->swapchainExtent;

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = { {0.5f, 0.5f, 0.5f, 1.0f} };
    clearValues[1].depthStencil = { 1.0f, 0 };

    renderPassBeginInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassBeginInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(currentCommandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

    //Render an object
    projmat[1][1] = -projmat[1][1]; //Flip Y axis for Vulkan

    for (const auto &pair : this->drawcalls)
    {
		const auto& draworder = pair.first;
		const auto& drawcallbatch = pair.second;

        for (const auto& drawcall : drawcallbatch)
        {
            //SYNC FIRST
            drawcall->SyncMaterialData(currentFrame);

            //USE SHADER
            auto shaderasset = drawcall->get_material()->getShader();
            const auto& currentshaderdata = shaderloader.GetAssetData(shaderasset);
            vkCmdBindPipeline(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentshaderdata.pipeline);

            VkViewport viewport{};
            viewport.width = static_cast<float>(this->swapchainExtent.width);
            viewport.height = static_cast<float>(this->swapchainExtent.height);
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            vkCmdSetViewport(currentCommandBuffer, 0, 1, &viewport);

            VkRect2D scissor{};
            scissor.offset = { 0, 0 };
            scissor.extent = this->swapchainExtent;
            vkCmdSetScissor(currentCommandBuffer, 0, 1, &scissor);

            //RENDER MESH
            const auto& curmesh = this->meshloader.GetAssetData(drawcall->get_mesh());
			
            //UPDATE GLOBAL UBO
            for (int dc_i = 0; dc_i < drawcall->get_call_count(); dc_i++) {
                auto& callinstance = drawcall->get_call_instance(dc_i);

                drawcall->ApplyOverride<Matrix4>(callinstance.model, "model", currentFrame, callinstance);
                drawcall->ApplyOverride<Matrix4>(projmat, "proj", currentFrame, callinstance);
                drawcall->ApplyOverride<Matrix4>(viewmat, "view", currentFrame, callinstance);

                VkBuffer vertexBuffers[] = { curmesh.vertexBuffer };
                VkDeviceSize offsets[] = { 0 };
                vkCmdBindVertexBuffers(currentCommandBuffer, 0, 1, vertexBuffers, offsets);


                std::vector<VkDescriptorSet> bindingsets;
                for (auto& set : callinstance.allocdescriptorSets)
                {
                    bindingsets.push_back(set[currentFrame]);
                }

                vkCmdBindDescriptorSets(currentCommandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, currentshaderdata.pipelineLayout, 0, bindingsets.size(), bindingsets.data(), 0, nullptr);

                vkCmdBindIndexBuffer(currentCommandBuffer, curmesh.indexBuffer, 0, VK_INDEX_TYPE_UINT16);
                vkCmdDrawIndexed(currentCommandBuffer, static_cast<uint32_t>(curmesh.loaddata->indices.size()), 1, 0, 0, 0);
            }
        }
    }

    //RECORDING
    if (recording) {
        const auto frame_data = this->ScreenShot();

        // Decode the image from the buffer into a cv::Mat
        cv::Mat image = cv::imdecode(frame_data, cv::IMREAD_COLOR);

        // Check if the image was successfully decoded
        if (image.empty()) {
            std::runtime_error("Failed to decode image!");
        }

        video_frames.push_back(image);
    }

    //EDITOR/GUI PASS
    this->editor->RenderPass(currentCommandBuffer);

    vkCmdEndRenderPass(currentCommandBuffer);

    if (vkEndCommandBuffer(currentCommandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to record command buffer!");
    }
#pragma endregion

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    presentInfo.pResults = nullptr; // Optional

    VkResult presentResult = vkQueuePresentKHR(presentQueue, &presentInfo);

    if (presentResult == VK_ERROR_OUT_OF_DATE_KHR || presentResult == VK_SUBOPTIMAL_KHR) {
        this->RefreshPipelineObjects();
    }
    else if (presentResult != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }

    this->currentFrame = (this->currentFrame + 1) % this->MAX_FRAMES_IN_FLIGHT;
}

std::vector<unsigned char> gbe::RenderPipeline::ScreenShot(bool write_file) {
    // Source for the copy is the last rendered swapchain image
    VkImage srcImage = swapChainImages[currentFrame];

    // Create the linear tiled destination image to copy to and to read the memory from
    VkImageCreateInfo imageCreateCI{};
	imageCreateCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    imageCreateCI.imageType = VK_IMAGE_TYPE_2D;
    // Note that vkCmdBlitImage (if supported) will also do format conversions if the swapchain color format would differ
    imageCreateCI.format = VK_FORMAT_R8G8B8A8_UNORM;
    imageCreateCI.extent.width = this->resolution.x;
    imageCreateCI.extent.height = this->resolution.y;
    imageCreateCI.extent.depth = 1;
    imageCreateCI.arrayLayers = 1;
    imageCreateCI.mipLevels = 1;
    imageCreateCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    imageCreateCI.samples = VK_SAMPLE_COUNT_1_BIT;
    imageCreateCI.tiling = VK_IMAGE_TILING_LINEAR;
    imageCreateCI.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    // Create the image
    VkImage dstImage;
    if (vkCreateImage(this->vkdevice, &imageCreateCI, nullptr, &dstImage) != VK_SUCCESS){
		throw new std::runtime_error("failed to create image for screenshot!");
    }
    // Create memory to back up the image
    VkMemoryRequirements memRequirements{};
    VkMemoryAllocateInfo memAllocInfo{};
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkDeviceMemory dstImageMemory;
    vkGetImageMemoryRequirements(this->vkdevice, dstImage, &memRequirements);
    memAllocInfo.allocationSize = memRequirements.size;
    // Memory must be host visible to copy from
    memAllocInfo.memoryTypeIndex = this->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	if (vkAllocateMemory(this->vkdevice, &memAllocInfo, nullptr, &dstImageMemory) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate memory for screenshot image!");
	}
    if (vkBindImageMemory(this->vkdevice, dstImage, dstImageMemory, 0) != VK_SUCCESS) {
        throw std::runtime_error("failed to bind image memory for screenshot image!");
    }

    // Do the actual blit from the swapchain image to our host visible destination image
    VkCommandBuffer copyCmd;
	beginSingleTimeCommands(copyCmd);

    // Transition destination image to transfer destination layout
    this->insertImageMemoryBarrier(
        copyCmd,
        dstImage,
        0,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_IMAGE_LAYOUT_UNDEFINED,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

    // Transition swapchain image from present to transfer source layout
    this->insertImageMemoryBarrier(
        copyCmd,
        srcImage,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_ACCESS_TRANSFER_READ_BIT,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

    
    // Otherwise use image copy (requires us to manually flip components)
    VkImageCopy imageCopyRegion{};
    imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.srcSubresource.layerCount = 1;
    imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    imageCopyRegion.dstSubresource.layerCount = 1;
    imageCopyRegion.extent.width = this->resolution.x;
    imageCopyRegion.extent.height = this->resolution.y;
    imageCopyRegion.extent.depth = 1;

    // Issue the copy command
    vkCmdCopyImage(
        copyCmd,
        srcImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &imageCopyRegion);

    // Transition destination image to general layout, which is the required layout for mapping the image memory later on
    this->insertImageMemoryBarrier(
        copyCmd,
        dstImage,
        VK_ACCESS_TRANSFER_WRITE_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        VK_IMAGE_LAYOUT_GENERAL,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

    // Transition back the swap chain image after the blit is done
    this->insertImageMemoryBarrier(
        copyCmd,
        srcImage,
        VK_ACCESS_TRANSFER_READ_BIT,
        VK_ACCESS_MEMORY_READ_BIT,
        VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
        VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VK_PIPELINE_STAGE_TRANSFER_BIT,
        VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

    endSingleTimeCommands(copyCmd);

    // Get layout of the image (including row pitch)
    VkImageSubresource subResource{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 0 };
    VkSubresourceLayout subResourceLayout;
    vkGetImageSubresourceLayout(this->vkdevice, dstImage, &subResource, &subResourceLayout);

    // Map image memory so we can start copying from it
    const char* data;
    vkMapMemory(this->vkdevice, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&data);
    data += subResourceLayout.offset;

    std::stringstream s_out = std::stringstream();

    // ppm header
    s_out << "P6\n" << this->resolution.x << "\n" << this->resolution.y << "\n" << 255 << "\n";

    // If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'll have to manually swizzle color components
    bool colorSwizzle = false;
    // Check if source is BGR
    // Note: Not complete, only contains most common and basic BGR surface formats for demonstration purposes
    
    std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
    colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), chosenSurfaceFormat.format) != formatsBGR.end());
    

        // ppm binary pixel data
    for (uint32_t y = 0; y < this->resolution.y; y++)
    {
        unsigned int* row = (unsigned int*)data;
        for (uint32_t x = 0; x < this->resolution.x; x++)
        {
            if (colorSwizzle)
            {
                s_out.write((char*)row + 2, 1);
                s_out.write((char*)row + 1, 1);
                s_out.write((char*)row, 1);
            }
            else
            {
                s_out.write((char*)row, 3);
            }
            row++;
        }
        data += subResourceLayout.rowPitch;
    }

    auto out_string = s_out.str();
    auto out_data = std::vector<unsigned char>(out_string.begin(), out_string.end());

    // Clean up resources
    vkUnmapMemory(this->vkdevice, dstImageMemory);
    vkFreeMemory(this->vkdevice, dstImageMemory, nullptr);
    vkDestroyImage(this->vkdevice, dstImage, nullptr);

    if (write_file) {
        auto file = std::ofstream("ss.ppm", std::ios::out | std::ios::binary);
        file << out_string;
        file.close();
    }

    return out_data;
}

void gbe::RenderPipeline::StartRecording()
{
    this->recording = true;
}

void gbe::RenderPipeline::StopRecording()
{
    //Export File
    if (this->recording) {
        std::string codecstr = "avc1";
        const unsigned int codec = (((((codecstr[3] << 8) | codecstr[2]) << 8) | codecstr[1]) << 8) | codecstr[0];
        std::string outputFileName = "latest_recording.mp4";
        double fps = 24.0;
        cv::Size frameSize(this->resolution.x, this->resolution.y);

        // Initialize VideoWriter
        cv::VideoWriter outputVideo;
        outputVideo.open(outputFileName, codec, fps, frameSize, true);

        // Write frames to video
        for (const auto& frame : this->video_frames) {
            // Create a destination Mat for the flipped image
            cv::Mat mirrored_image;

            cv::flip(frame, mirrored_image, 0);
            outputVideo.write(mirrored_image);
        }

        // Release the VideoWriter
        outputVideo.release();

        std::cout << "Video saved successfully to " << outputFileName << std::endl;
    }

    this->recording = false;
}

void gbe::RenderPipeline::ToggleRecording()
{
    if (this->recording)
        this->StopRecording();
    else
        this->StartRecording();
}

gbe::gfx::DrawCall* gbe::RenderPipeline::RegisterDrawCall(asset::Mesh* mesh, asset::Material* material)
{
	auto newdrawcall = new DrawCall(mesh, material, &shaderloader.GetAssetData(material->getShader()), this->MAX_FRAMES_IN_FLIGHT, &this->vkdevice, 0);

	if (this->drawcalls.find(newdrawcall->get_order()) == this->drawcalls.end()) {
		this->drawcalls.insert_or_assign(newdrawcall->get_order(), std::vector<DrawCall*>{ newdrawcall });
	}
	else {
		this->drawcalls[newdrawcall->get_order()].push_back(newdrawcall);
	}

    return newdrawcall;
}

DrawCall* gbe::RenderPipeline::RegisterDefaultDrawCall(asset::Mesh* mesh, asset::Material* material)
{
    this->default_drawcall = RegisterDrawCall(mesh, material);

    return this->default_drawcall;
}

DrawCall* gbe::RenderPipeline::GetDefaultDrawCall()
{
    return this->default_drawcall;
}

void* gbe::RenderPipeline::GetPipelineVariable(std::string id)
{
    if (this->PipelineVariables.find(id) == this->PipelineVariables.end()) {
        throw std::runtime_error("Variable does not exist.");
    }

    return this->PipelineVariables.at(id);
}

void gbe::RenderPipeline::RefreshPipelineObjects() {
	if (this->window->isMinimized())
		return;

    this->CleanPipelineObjects();
	this->InitializePipelineObjects();
    this->CreateDepthResources();
    this->InitializeFramebuffers();
}

void gbe::RenderPipeline::InitializeFramebuffers()
{
    this->swapChainFramebuffers.resize(this->swapChainImageViews.size());
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 2> attachments = {
            swapChainImageViews[i],
            depthImageView
        };

        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass;
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
        framebufferInfo.pAttachments = attachments.data();
        framebufferInfo.width = swapchainExtent.width;
        framebufferInfo.height = swapchainExtent.height;
        framebufferInfo.layers = 1;

        if (vkCreateFramebuffer(this->vkdevice, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

void gbe::RenderPipeline::CleanPipelineObjects() {
    vkDeviceWaitIdle(this->vkdevice);

    vkDestroyImageView(this->vkdevice, depthImageView, nullptr);
    vkDestroyImage(this->vkdevice, depthImage, nullptr);
    vkFreeMemory(this->vkdevice, depthImageMemory, nullptr);

    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(this->vkdevice, framebuffer, nullptr);
    }

    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(vkdevice, imageView, nullptr);
    }

    vkDestroySwapchainKHR(vkdevice, swapChain, nullptr);
}

void gbe::RenderPipeline::CreateDepthResources()
{
    auto depthformat = findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );

    createImage(this->resolution.x, this->resolution.y, depthformat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    createImageView(depthImageView, depthImage, depthformat, VK_IMAGE_ASPECT_DEPTH_BIT);

    transitionImageLayout(depthImage, depthformat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);

}

VkFormat gbe::RenderPipeline::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
{
    for (VkFormat format : candidates) {
        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(this->vkphysicalDevice, format, &props);

        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }

    throw std::runtime_error("failed to find supported format!");
}

void gbe::RenderPipeline::CleanUp()
{
    if (enableValidationLayers) {
        auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(this->vkInst, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr) {
            func(this->vkInst, this->debugMessenger, nullptr);
        }
    }

    vkDeviceWaitIdle(this->vkdevice);

    this->CleanPipelineObjects();

    //Insert Shader Program disposal here
    
    vkDestroyRenderPass(vkdevice, renderPass, nullptr);
   
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(this->vkdevice, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(this->vkdevice, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(this->vkdevice, inFlightFences[i], nullptr);
    }
    vkDestroyCommandPool(this->vkdevice, commandPool, nullptr);
    
	for (auto it = drawcalls.begin(); it != drawcalls.end(); it++) {
		const auto& drawcallbatch = it->second;

		for (const auto& drawcall : drawcallbatch)
		{
            delete drawcall;
		}
	}

    vkDestroySurfaceKHR(vkInst, vksurface, nullptr);
    vkDestroyDevice(this->vkdevice, nullptr);
    vkDestroyInstance(vkInst, nullptr);
}
