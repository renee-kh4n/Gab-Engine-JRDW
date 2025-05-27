#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <unordered_map>
#include <stack>

namespace gbe {
	namespace gfx {
		struct TextureData {
			VkImage textureImage;
			VkDeviceMemory textureImageMemory;
			VkImageView textureImageView;
			VkSampler textureSampler;
		};

		class TextureLoader : public asset::AssetLoader<asset::Texture, asset::data::TextureImportData, asset::data::TextureLoadData, TextureData> {
		private:
			VkDevice* vkdevice;
			VkPhysicalDevice* vkphysicaldevice;
			TextureData defaultImage;
		protected:
			TextureData LoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
			void UnLoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
		public:
			static TextureData& GetDefaultImage();
			void PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice);
		};
	}
}