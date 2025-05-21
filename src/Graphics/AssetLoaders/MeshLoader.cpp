#include "MeshLoader.h"

#include "../RenderPipeline.h"

bool gbe::gfx::MeshLoader::LoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* loaddata)
{
    auto meshpath = asset->Get_asset_directory() + importdata.path;

    //Read mesh file here

    std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    //Buffer creation
    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;

    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();
    RenderPipeline::createBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);

    void* data;
    vkMapMemory(*this->vkdevice, vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(*this->vkdevice, vertexBufferMemory);

    this->loaded_meshes.insert_or_assign(loaded_meshes_count, MeshData{
        vertices,
        vertexBuffer,
        vertexBufferMemory

        });

    //Increment
    loaded_meshes_count++;
    return true;
}

void gbe::gfx::MeshLoader::UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data)
{
	vkDestroyBuffer((*this->vkdevice), this->Get_mesh(data->gl_id).vertexBuffer, nullptr);
	vkFreeMemory((*this->vkdevice), this->Get_mesh(data->gl_id).vertexBufferMemory, nullptr);
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
