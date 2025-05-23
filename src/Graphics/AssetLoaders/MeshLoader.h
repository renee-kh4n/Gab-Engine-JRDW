#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include "../Data/Vertex.h"

#include <vulkan/vulkan.h>
#include "TextureLoader.h"
#include <optional>
#include <tuple>

namespace gbe {
	namespace gfx {
		struct UniformBufferObject {
			gbe::Matrix4 model;
			gbe::Matrix4 view;
			gbe::Matrix4 proj;
		};

		struct MeshData {
			std::vector<Vertex> vertices;
			std::vector<uint16_t> indices;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			VkBuffer indexBuffer;
			VkDeviceMemory indexBufferMemory;

			std::vector<VkBuffer> uniformBuffers;
			std::vector<VkDeviceMemory> uniformBuffersMemory;
			std::vector<void*> uniformBuffersMapped;
		};

		class MeshLoader : public asset::AssetLoader<asset::Mesh, asset::data::MeshImportData, asset::data::MeshLoadData> {
		private:
			std::unordered_map<unsigned int, MeshData> loaded_meshes;
			VkDevice* vkdevice;
			VkPhysicalDevice* vkphysicaldevice;
			unsigned int loaded_meshes_count = 0;
			unsigned int MAX_FRAMES_IN_FLIGHT = 0;
		protected:
			bool LoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data) override;
			void UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data) override;
		public:
			MeshData& Get_mesh(unsigned int id);
			void SetBufferMemory(const MeshData& data, unsigned int index, UniformBufferObject& ubo);
			void PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice, int MAX_FRAMES_IN_FLIGHT);
		};
	}
}