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
            
            for (const auto& block : callinst.uniformBuffers)
            {
                for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                    vkDestroyBuffer(*this->vkdevice, block.uboPerFrame[i], nullptr);
                    vkFreeMemory(*this->vkdevice, block.uboMemoryPerFrame[i], nullptr);
                }
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
        for (const auto& block : shaderdata->uniformblocks)
        {
			auto newblockbuffer = CallInstance::UniformBlockBuffer{};
			newblockbuffer.block_name = block.name;

			// Create uniform buffer for each block
            VkDeviceSize bufferSize = block.block_size;

            newblockbuffer.uboPerFrame.resize(MAX_FRAMES_IN_FLIGHT);
            newblockbuffer.uboMemoryPerFrame.resize(MAX_FRAMES_IN_FLIGHT);
            newblockbuffer.uboMappedPerFrame.resize(MAX_FRAMES_IN_FLIGHT);

            for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
                RenderPipeline::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, newblockbuffer.uboPerFrame[i], newblockbuffer.uboMemoryPerFrame[i]);

                vkMapMemory(*this->vkdevice, newblockbuffer.uboMemoryPerFrame[i], 0, bufferSize, 0, &newblockbuffer.uboMappedPerFrame[i]);
            }

			newinst.uniformBuffers.push_back(newblockbuffer);
        }
        //Textures
		for (const auto& field : shaderdata->uniformfields)
		{
			if (field.type == asset::Shader::UniformFieldType::TEXTURE)
			{
				CallInstance::UniformTexture newtexture{};
				newtexture.texture_name = field.name;
				// Leave image view and sampler for the texture default
				newtexture.imageView;
				newtexture.sampler;
				newinst.uniformTextures.push_back(newtexture);
			}
		}

        //DESCRIPTOR POOL
        std::vector<VkDescriptorPoolSize> poolSizes{};
        for (const auto& pair : shaderdata->binding_sets)
        {
			const auto& setlist = pair.second;

			for (const auto& binding : setlist)
			{
                poolSizes.push_back({
                        .type = binding.descriptorType,
                        .descriptorCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT)
                    });
			}
        }

        VkDescriptorPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
        poolInfo.pPoolSizes = poolSizes.data();
        poolInfo.maxSets = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT * shaderdata->descriptorSetLayouts.size());

        if (vkCreateDescriptorPool(*this->vkdevice, &poolInfo, nullptr, &newinst.descriptorPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create descriptor pool!");
        }

        //DESCRIPTOR SETS
        std::vector<VkDescriptorSetLayout> layouts{};
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
        {
            layouts.append_range(shaderdata->descriptorSetLayouts);
        }

        VkDescriptorSetAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        allocInfo.descriptorPool = newinst.descriptorPool;
        allocInfo.descriptorSetCount = static_cast<uint32_t>(MAX_FRAMES_IN_FLIGHT);
        allocInfo.pSetLayouts = layouts.data();

        newinst.descriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(*this->vkdevice, &allocInfo, newinst.descriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t f_i = 0; f_i < MAX_FRAMES_IN_FLIGHT; f_i++) {
			std::vector<VkDescriptorBufferInfo> bufferInfos;

            for (const auto& uniformblock : newinst.uniformBuffers)
            {
                ShaderData::ShaderBlock blockinfo{};

                bool found_block = shaderdata->FindUniformBlock(uniformblock.block_name, blockinfo);
                if(!found_block)
					throw std::runtime_error("Failed to find uniform block: " + uniformblock.block_name);

                VkDescriptorBufferInfo bufferInfo{};
                bufferInfo.buffer = uniformblock.uboPerFrame[f_i];
                bufferInfo.offset = 0;
                bufferInfo.range = blockinfo.block_size;

                bufferInfos.push_back(bufferInfo);
            }


            std::vector<VkDescriptorImageInfo> imageInfos;

            for (const auto& uniformtex : newinst.uniformTextures)
            {
                VkDescriptorImageInfo imageInfo{};

                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = uniformtex.imageView;
                imageInfo.sampler = uniformtex.sampler;

				imageInfos.push_back(imageInfo);
            }

            std::vector<VkWriteDescriptorSet> descriptorWrites{};

            for (const auto& bufferinfo : bufferInfos)
            {
				VkWriteDescriptorSet descriptorWrite{};

                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = newinst.descriptorSets[f_i];
                descriptorWrite.dstBinding = 0;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferinfo;

				descriptorWrites.push_back(descriptorWrite);
            }

            for (const auto& imageinfo : imageInfos)
            {
                VkWriteDescriptorSet descriptorWrite{};

                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = newinst.descriptorSets[f_i];
                descriptorWrite.dstBinding = 1;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pImageInfo = &imageinfo;

                descriptorWrites.push_back(descriptorWrite);
            }

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

    void gbe::gfx::DrawCall::UpdateUniforms(GlobalUniforms& ubo, unsigned int frameindex)
    {
        for (auto& pair : this->calls)
        {
            auto& callinstance = pair.second;

            ApplyOverride<Matrix4>(callinstance.model, "model", frameindex, callinstance);
            ApplyOverride<Matrix4>(ubo.view, "view", frameindex, callinstance);
            ApplyOverride<Matrix4>(ubo.proj, "proj", frameindex, callinstance);
        }
    }
}

bool gbe::gfx::DrawCall::CallInstance::GetBlock(std::string name, gbe::gfx::DrawCall::CallInstance::UniformBlockBuffer& out_block)
{
    for (auto& block: this->uniformBuffers)
    {
		if (block.block_name == name)
		{
            out_block = block;
            return true;
		}
    }

    return false;
}
