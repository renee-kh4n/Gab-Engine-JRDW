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

    this->loaded_meshes.insert_or_assign(loaded_meshes_count, MeshData{
        vertices,
        indices,
        vertexBuffer,
        vertexBufferMemory,
        indexBuffer,
        indexBufferMemory

        });

    //Increment
    loaded_meshes_count++;
    return true;
}

void gbe::gfx::MeshLoader::UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data)
{
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

void gbe::gfx::MeshLoader::PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice)
{
	this->vkdevice = vkdevice;
    this->vkphysicaldevice = vkphysicaldevice;
}
