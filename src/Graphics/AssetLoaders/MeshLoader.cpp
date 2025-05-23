#include "MeshLoader.h"

#include "../RenderPipeline.h"

bool gbe::gfx::MeshLoader::LoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* loaddata)
{
    auto meshpath = asset->Get_asset_directory() + importdata.path;

    //Read mesh file here

    const std::vector<Vertex> vertices = {
    {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
    };

    const std::vector<uint16_t> indices = {
    0, 1, 2, 2, 3, 0
    };

    //VERTEX BUFFER
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkDeviceSize vbufferSize = sizeof(vertices[0]) * vertices.size();

    VkBuffer vstagingBuffer;
    VkDeviceMemory vstagingBufferMemory;
    RenderPipeline::createBuffer(vbufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vstagingBuffer, vstagingBufferMemory);

    void* vdata;
    vkMapMemory(*this->vkdevice, vstagingBufferMemory, 0, vbufferSize, 0, &vdata);
    memcpy(vdata, vertices.data(), (size_t)vbufferSize);
    vkUnmapMemory(*this->vkdevice, vstagingBufferMemory);

    RenderPipeline::createBuffer(vbufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
    RenderPipeline::copyBuffer(vstagingBuffer, vertexBuffer, vbufferSize);

    vkDestroyBuffer(*this->vkdevice, vstagingBuffer, nullptr);
    vkFreeMemory(*this->vkdevice, vstagingBufferMemory, nullptr);

    //INDEX BUFFER
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkDeviceSize ibufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer istagingBuffer;
    VkDeviceMemory istagingBufferMemory;
    RenderPipeline::createBuffer(ibufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, istagingBuffer, istagingBufferMemory);

    void* idata;
    vkMapMemory(*this->vkdevice, istagingBufferMemory, 0, ibufferSize, 0, &idata);
    memcpy(idata, indices.data(), (size_t)ibufferSize);
    vkUnmapMemory(*this->vkdevice, istagingBufferMemory);

    RenderPipeline::createBuffer(ibufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
    RenderPipeline::copyBuffer(istagingBuffer, indexBuffer, ibufferSize);

    vkDestroyBuffer(*this->vkdevice, istagingBuffer, nullptr);
    vkFreeMemory(*this->vkdevice, istagingBufferMemory, nullptr);

    //UNIFORM BUFFER
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
    std::vector<void*> uniformBuffersMapped;

    VkDeviceSize bufferSize = sizeof(UniformBufferObject);

    uniformBuffers.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMemory.resize(MAX_FRAMES_IN_FLIGHT);
    uniformBuffersMapped.resize(MAX_FRAMES_IN_FLIGHT);

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        RenderPipeline::createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(*this->vkdevice, uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }

    //COMMITTING

    this->loaded_meshes.insert_or_assign(loaded_meshes_count, MeshData{
        vertices,
        indices,
        vertexBuffer,
        vertexBufferMemory,
        indexBuffer,
        indexBufferMemory,
		uniformBuffers,
        uniformBuffersMemory,
        uniformBuffersMapped
        });
    loaddata->gl_id = loaded_meshes_count;

    //Increment
    loaded_meshes_count++;
    return true;
}

void gbe::gfx::MeshLoader::UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data)
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroyBuffer(*this->vkdevice, this->Get_mesh(data->gl_id).uniformBuffers[i], nullptr);
        vkFreeMemory(*this->vkdevice, this->Get_mesh(data->gl_id).uniformBuffersMemory[i], nullptr);
    }

	vkDestroyBuffer((*this->vkdevice), this->Get_mesh(data->gl_id).vertexBuffer, nullptr);
	vkFreeMemory((*this->vkdevice), this->Get_mesh(data->gl_id).vertexBufferMemory, nullptr);
    vkDestroyBuffer((*this->vkdevice), this->Get_mesh(data->gl_id).indexBuffer, nullptr);
    vkFreeMemory((*this->vkdevice), this->Get_mesh(data->gl_id).indexBufferMemory, nullptr);


	this->loaded_meshes.erase(data->gl_id);
}

gbe::gfx::MeshData& gbe::gfx::MeshLoader::Get_mesh(unsigned int id)
{
	auto it = this->loaded_meshes.find(id);
	if (it != this->loaded_meshes.end()) {
		return it->second;
	}
	else {
		throw std::runtime_error("Mesh not found");
	}
}

void gbe::gfx::MeshLoader::SetBufferMemory(const MeshData& data, unsigned int index, UniformBufferObject& ubo)
{
    struct UniformBufferObject_INTERNAL {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

    UniformBufferObject_INTERNAL ubo_translated = {
        ubo.model,
        ubo.view,
        ubo.proj,
    };

    memcpy(data.uniformBuffersMapped[index], &ubo_translated, sizeof(ubo_translated));
}

void gbe::gfx::MeshLoader::PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice, int MAX_FRAMES_IN_FLIGHT)
{
	this->vkdevice = vkdevice;
    this->vkphysicaldevice = vkphysicaldevice;
    this->MAX_FRAMES_IN_FLIGHT = MAX_FRAMES_IN_FLIGHT;
}
