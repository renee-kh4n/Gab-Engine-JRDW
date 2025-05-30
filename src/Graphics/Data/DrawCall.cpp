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

    gfx::DrawCall::CallInstance& gfx::DrawCall::get_call_instance(unsigned int index)
    {
        auto it_call = this->calls.begin();

        for (int iters = 0; iters < index; iters++)
        {
            it_call++;
        }

        return it_call->second;
    }

    unsigned int gfx::DrawCall::get_call_count()
    {
        return this->callcount;
    }

    unsigned int gfx::DrawCall::get_order()
    {
        return this->order;
    }

    bool gfx::DrawCall::SyncMaterialData(unsigned int frameindex)
    {
        for (size_t m_i = 0; m_i < this->get_material()->getOverrideCount(); m_i++)
        {
            std::string id;
            auto& overridedata = this->get_material()->getOverride(m_i, id);

            if (overridedata.handled_change == false)
                overrideHandledList[id].clear();
			else if (overrideHandledList[id].size() >= MAX_FRAMES_IN_FLIGHT)
                continue;

            //CONFIRM UNIFORM FIELD EXISTS
            ShaderData::ShaderField fieldinfo;
            ShaderData::ShaderBlock blockinfo;
            bool found = shaderdata->FindUniformField(id, fieldinfo, blockinfo);
            if (!found)
                continue;

            if (overridedata.type == asset::Shader::UniformFieldType::TEXTURE)
            {
                auto findtexturedata = TextureLoader::GetAssetData(overridedata.value_tex.Get_asset());

                //CREATE NEW DESCRIPTOR WRITE
                VkDescriptorImageInfo imageInfo{};

                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = findtexturedata.textureImageView;
                imageInfo.sampler = findtexturedata.textureSampler;

                //CREATE THE WRITE DATA FOR EACH INSTANCE
				for (const auto& callinstpair : this->calls)
				{
                    auto& callinst = callinstpair.second;

                    VkWriteDescriptorSet descriptorWrite{};

                    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                    descriptorWrite.dstSet = callinst.allocdescriptorSets[frameindex];
                    descriptorWrite.dstBinding = fieldinfo.binding;
                    descriptorWrite.dstArrayElement = 0;
                    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                    descriptorWrite.descriptorCount = 1;
                    descriptorWrite.pImageInfo = &imageInfo;

                    vkUpdateDescriptorSets(*this->vkdevice, static_cast<uint32_t>(1), &descriptorWrite, 0, nullptr);
				}   
            }
            else if (overridedata.type == asset::Shader::UniformFieldType::VEC4) {
                //CREATE THE WRITE DATA FOR EACH INSTANCE
                for (auto& callinstpair : this->calls)
                {
                    CallInstance& callinst = callinstpair.second;
                    
                    this->ApplyOverride<Vector4>(overridedata.value_vec4, id, frameindex, callinst);
                }
            }

			overrideHandledList[id].push_back(frameindex);

            overridedata.handled_change = true;
        }

        return true;
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
                auto defaultImage = TextureLoader::GetDefaultImage();
				newtexture.imageView = defaultImage.textureImageView;
				newtexture.sampler = defaultImage.textureSampler;

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

        newinst.allocdescriptorSets.resize(MAX_FRAMES_IN_FLIGHT);
        if (vkAllocateDescriptorSets(*this->vkdevice, &allocInfo, newinst.allocdescriptorSets.data()) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate descriptor sets!");
        }

        for (size_t f_i = 0; f_i < MAX_FRAMES_IN_FLIGHT; f_i++) {
            std::vector<VkWriteDescriptorSet> descriptorWrites{};

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

                //CREATE THE WRITE DATA
                VkWriteDescriptorSet descriptorWrite{};

                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = newinst.allocdescriptorSets[f_i];
                descriptorWrite.dstBinding = blockinfo.binding;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pBufferInfo = &bufferInfo;

                descriptorWrites.push_back(descriptorWrite);
            }


            for (const auto& uniformtex : newinst.uniformTextures)
            {
                VkDescriptorImageInfo imageInfo{};

                imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
                imageInfo.imageView = uniformtex.imageView;
                imageInfo.sampler = uniformtex.sampler;

                //FIND THE BINDING INDEX
                ShaderData::ShaderField fieldinfo;
                ShaderData::ShaderBlock blockinfo;
                shaderdata->FindUniformField(uniformtex.texture_name, fieldinfo, blockinfo);
                //CREATE THE WRITE DATA
                VkWriteDescriptorSet descriptorWrite{};

                descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
                descriptorWrite.dstSet = newinst.allocdescriptorSets[f_i];
                descriptorWrite.dstBinding = fieldinfo.binding;
                descriptorWrite.dstArrayElement = 0;
                descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
                descriptorWrite.descriptorCount = 1;
                descriptorWrite.pImageInfo = &imageInfo;

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
