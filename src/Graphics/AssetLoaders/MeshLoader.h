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
		struct MeshData {
			std::vector<Vertex> vertices;
			std::vector<uint16_t> indices;
			VkBuffer vertexBuffer;
			VkDeviceMemory vertexBufferMemory;
			VkBuffer indexBuffer;
			VkDeviceMemory indexBufferMemory;
			std::vector<VkDescriptorSet> descriptorSets;
		};

		class MeshLoader : public asset::AssetLoader<asset::Mesh, asset::data::MeshImportData, asset::data::MeshLoadData> {
		private:
			std::unordered_map<unsigned int, MeshData> loaded_meshes;
			VkDevice* vkdevice;
			VkPhysicalDevice* vkphysicaldevice;
			unsigned int loaded_meshes_count = 0;
		protected:
			bool LoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data) override;
			void UnLoadAsset_(asset::Mesh* asset, const asset::data::MeshImportData& importdata, asset::data::MeshLoadData* data) override;
		public:
			MeshData& Get_mesh(unsigned int id);
			void PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice);
		};
	}
}