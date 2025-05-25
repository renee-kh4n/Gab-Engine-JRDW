#include "DrawCall.h"

#include "../RenderPipeline.h"

namespace gbe {
    using namespace gfx;

    DrawCall::DrawCall(asset::Mesh* mesh, asset::Material* material, ShaderData* _shaderdata, unsigned int MAX_FRAMES_IN_FLIGHT, VkDevice* _vkdevice, int order)
    {
        this->vkdevice = _vkdevice;
        this->shaderdata = _shaderdata;
        this->order = order;
        this->m_mesh = mesh;
        this->m_material = material;
        this->MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
    }
    gfx::DrawCall::~DrawCall()
    {
        for (const auto& pair : this->calls)
        {
			const auto& callinst = pair.second;

            vkDestroyDescriptorPool(*this->vkdevice, callinst.descriptorPool, nullptr);
            
            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                vkDestroyBuffer(*this->vkdevice, callinst.uniformBuffers[i], nullptr);
                vkFreeMemory(*this->vkdevice, callinst.uniformBuffersMemory[i], nullptr);
            }
        }
    }

    asset::Mesh* gfx::DrawCall::get_mesh()
    {
        return this->m_mesh;
    }

    asset::Material* gfx::DrawCall::get_material()
    {
        return this->m_material;
    }

    unsigned int gfx::DrawCall::get_call_count()
    {
        return this->callcount;
    }

    unsigned int gfx::DrawCall::get_order()
    {
        return this->order;
    }

    void gfx::DrawCall::BindForRender(VkCommandBuffer curcmdbuffer, unsigned int frame, unsigned int objindex, TransformUBO ubo)
    {
    }

    Matrix4 gfx::DrawCall::get_callmatrix(unsigned int iter)
    {
        auto it_call = this->calls.begin();

        for (int iters = 0; iters < iter; iters++)
        {
            it_call++;
        }

        return it_call->second.model;
    }

    VkDescriptorSet* gfx::DrawCall::get_descriptorset(unsigned int frame, unsigned int objindex)
    {
        auto it_call = this->calls.begin();

        for (int iters = 0; iters < objindex; iters++)
        {
            it_call++;
        }

        return &it_call->second.descriptorSets[frame];
    }

    gbe::Matrix4* gfx::DrawCall::RegisterCall(void* instance_id, Matrix4 matrix)
    {
        bool exists = this->calls.find(instance_id) != this->calls.end();
        if(!exists)
            callcount++;

        CallInstance newinst{};

        //BUFFERS
        VkDeviceSize bufferSize = sizeof(TransformUBO);

        newinst.uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
        newinst.uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
        newinst.uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            RenderPipeline::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, newinst.uniformBuffers[i], newinst.uniformBuffersMemory[i]);

            vkMapMemory(*this->vkdevice, newinst.uniformBuffersMemory[i], 0, bufferSize, 0, &newinst.uniformBuffersMapped[i]);
        }

        //DESCRIPTOR POOL
        std::array<VkDescriptorPoolSize, 2> poolSizes{};
        poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        poolSizes[0].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        poolSizes[1].descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);

        if (vkCreateDescriptorPool(*this->vkdevice, &poolInfo, nullptr, &newinst.descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        //DESCRIPTOR SETS
        std::vector<VkDescriptorSetLayout> layouts(MAX_FRAMES_IN_FLIGHT, shaderdata->descriptorSetLayout);
        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = newinst.descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        newinst.descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(*this->vkdevice, &allocInfo, newinst.descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = newinst.uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(TransformUBO);

            /*
            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = textureImageView;
            imageInfo.sampler = textureSampler;
            */

            std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = newinst.descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            /*
            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;
            */

            vkUpdateDescriptorSets(*this->vkdevice, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
        }

        //COMMITTING
		this->calls.insert_or_assign(instance_id, newinst);
        
		return &this->calls[instance_id].model;
    }

    void gfx::DrawCall::UnRegisterCall(void* instance_id)
    {
        bool exists = this->calls.find(instance_id) != this->calls.end();
        if (exists)
            callcount--;

        this->calls.erase(instance_id);
    }

    void gbe::gfx::DrawCall::UpdateGlobalUniforms(GlobalUniforms& ubo)
    {
        struct TransformUBO_INTERNAL {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };

        for (const auto& pair : this->calls)
        {
            const auto& data = pair.second;

            TransformUBO_INTERNAL ubo_translated = {
                data.model,
                ubo.view,
                ubo.proj,
            };

            for (size_t frameindex = 0; frameindex < MAX_FRAMES_IN_FLIGHT; frameindex++)
            {
                memcpy(data.uniformBuffersMapped[frameindex], &ubo_translated, sizeof(ubo_translated));
            }
        }
    }
}