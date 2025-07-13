#include "MeshLoader.h"

#include "../RenderPipeline.h"

gbe::gfx::MeshData gbe::gfx::MeshLoader::LoadAsset_(asset::Mesh * asset, const asset::data::MeshImportData & importdata, asset::data::MeshLoadData * loaddata)
{
    auto meshpath = asset->Get_asset_directory() + importdata.path;

    //Read mesh file here
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, meshpath.c_str())) {
        throw std::runtime_error(warn + err);
    }

    std::vector<asset::data::Vertex> vertices = {};
    std::vector<uint16_t> indices = {};
    std::vector<std::vector<uint16_t>> faces = {};

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            asset::data::Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.normal = {
                attrib.normals[3 * index.normal_index + 0],
                attrib.normals[3 * index.normal_index + 1],
                attrib.normals[3 * index.normal_index + 2]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }

        size_t index_counter = 0;
        for (const auto& faceverts : shape.mesh.num_face_vertices)
        {
            std::vector<uint16_t> cur_face = {};

            for (size_t f = 0; f < faceverts; f++)
            {
                cur_face.push_back(indices[index_counter]);

                index_counter++;
            }

            faces.push_back(cur_face);
        }

    }

    //VULKAN MESH SETUP vvvvvvvvvvv

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

    //COMMITTING
    loaddata->indices = indices;
    loaddata->vertices = vertices;
    loaddata->faces = faces;

    return MeshData{
        loaddata,
        vertexBuffer,
        vertexBufferMemory,
        indexBuffer,
        indexBufferMemory
        };
}

void gbe::gfx::MeshLoader::UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data)
{
    const auto& meshdata = this->GetAssetData(asset);

    vkDestroyBuffer((*this->vkdevice), meshdata.vertexBuffer, nullptr);
	vkFreeMemory((*this->vkdevice), meshdata.vertexBufferMemory, nullptr);
    vkDestroyBuffer((*this->vkdevice), meshdata.indexBuffer, nullptr);
    vkFreeMemory((*this->vkdevice), meshdata.indexBufferMemory, nullptr);
}

void gbe::gfx::MeshLoader::PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice)
{
	this->vkdevice = vkdevice;
    this->vkphysicaldevice = vkphysicaldevice;
}
