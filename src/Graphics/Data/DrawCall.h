#pragma once
#include "Asset/gbe_asset.h"
#include <vulkan/vulkan.h>
#include "../AssetLoaders/ShaderLoader.h"
#include "../AssetLoaders/MeshLoader.h"

namespace gbe {
    namespace gfx {
        class DrawCall {
            struct CallInstance {
                Matrix4 model;

                std::vector<VkBuffer> uniformBuffers;
                std::vector<VkDeviceMemory> uniformBuffersMemory;
                std::vector<void*> uniformBuffersMapped;

                VkDescriptorPool descriptorPool;
                std::vector<VkDescriptorSet> descriptorSets;
            };

        private:
            std::unordered_map<void*, CallInstance> calls;
            unsigned int callcount = 0;

            int order;
            asset::Mesh* m_mesh;
            asset::Material* m_material;

            //VULKAN
			VkDevice* vkdevice;
			ShaderData* shaderdata;
            unsigned int MAX_FRAMES_IN_FLIGHT = 2;
        public:
            struct GlobalUniforms {
                gbe::Matrix4 proj;
                gbe::Matrix4 view;
            };

            DrawCall(asset::Mesh* mesh, asset::Material* material, ShaderData* shaderdata, unsigned int MAX_FRAMES_IN_FLIGHT, VkDevice* vkdevice, int order);
            ~DrawCall();

            asset::Mesh* get_mesh();
            asset::Material* get_material();

            unsigned int get_call_count();
            unsigned int get_order();

            void BindForRender(VkCommandBuffer curcmdbuffer, unsigned int frame, unsigned int objindex, TransformUBO ubo);

            Matrix4 get_callmatrix(unsigned int iter);
            VkDescriptorSet* get_descriptorset(unsigned int frame, unsigned int objindex);
            
            Matrix4* RegisterCall(void* instance_id, Matrix4 matrix);
            void UnRegisterCall(void* instance_id);

            void UpdateGlobalUniforms(GlobalUniforms& ubo);
        };
    }
}