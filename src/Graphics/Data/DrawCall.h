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

                struct UniformBlockBuffer {
                    std::string block_name;

                    std::vector<VkBuffer> uboPerFrame;
                    std::vector<VkDeviceMemory> uboMemoryPerFrame;
                    std::vector<void*> uboMappedPerFrame;
                };

                struct UniformTexture {
					std::string texture_name;
					VkImageView imageView;
					VkSampler sampler;
                };

				std::vector<UniformBlockBuffer> uniformBuffers;
				std::vector<UniformTexture> uniformTextures;
                VkDescriptorPool descriptorPool;
                std::vector<std::vector<VkDescriptorSet>> allocdescriptorSets;

                bool GetBlock(std::string name, gbe::gfx::DrawCall::CallInstance::UniformBlockBuffer& out_block);
            };

        private:
            std::unordered_map<void*, CallInstance> calls;
            std::unordered_map<std::string, std::vector<int>> overrideHandledList;

            unsigned int callcount = 0;

            int order;
            asset::Mesh* m_mesh;
            asset::Material* m_material;

            //VULKAN
			VkDevice* vkdevice;
			ShaderData* shaderdata;
            unsigned int MAX_FRAMES_IN_FLIGHT = 2;
        public:

            DrawCall(asset::Mesh* mesh, asset::Material* material, ShaderData* shaderdata, unsigned int MAX_FRAMES_IN_FLIGHT, VkDevice* vkdevice, int order);
            ~DrawCall();

            asset::Mesh* get_mesh();
            asset::Material* get_material();

            CallInstance& get_call_instance(unsigned int index);
            unsigned int get_call_count();
            unsigned int get_order();

            bool SyncMaterialData(unsigned int frameindex);

            template<typename T>
            bool ApplyOverride(const T& valueref, std::string target, unsigned int frameindex, CallInstance& callinst) {
                ShaderData::ShaderBlock blockinfo;
                ShaderData::ShaderField fieldinfo;

                if (shaderdata->FindUniformField(target, fieldinfo, blockinfo) == false)
                    return false;

                CallInstance::UniformBlockBuffer blockbuffer;

                if(callinst.GetBlock(blockinfo.name, blockbuffer) == false)
					return false;

                const auto& blockaddr = reinterpret_cast<char*>(blockbuffer.uboMappedPerFrame[frameindex]);
                const auto& finaladdr = blockaddr + fieldinfo.offset;

                if (sizeof(valueref) != fieldinfo.size)
                    throw std::runtime_error("Data size mismatch. Can Result in unexpected behaviour.");

                memcpy(finaladdr, &valueref, fieldinfo.size);

                return true;
            }

            Matrix4* RegisterCall(void* instance_id, Matrix4 matrix);
            void UnRegisterCall(void* instance_id);
        };
    }
}