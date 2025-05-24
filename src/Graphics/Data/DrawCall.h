#pragma once
#include "Asset/gbe_asset.h"
#include <vulkan/vulkan.h>
#include "../AssetLoaders/ShaderLoader.h"
#include "../AssetLoaders/MeshLoader.h"

namespace gbe {
    namespace gfx {
        class DrawCall {
        public:
            std::unordered_map<void*, Matrix4> calls;

            int order;
            asset::Mesh* m_mesh;
            asset::Material* m_material;


            //VULKAN
			VkDevice* vkdevice;
			ShaderData* shaderdata;
            unsigned int MAX_FRAMES_IN_FLIGHT = 0;
            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;

            VkDescriptorPool descriptorPool;
            std::vector<VkDescriptorSet> descriptorSets;

            DrawCall(asset::Mesh* mesh, asset::Material* material, ShaderData* shaderdata, unsigned int MAX_FRAMES_IN_FLIGHT, VkDevice* vkdevice, int order);
            ~DrawCall();

            void SetBufferMemory(unsigned int index, UniformBufferObject& ubo);
        };
    }
}