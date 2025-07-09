#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <vulkan/vulkan.h>
#include <stb_image.h>
#include <functional>
#include <unordered_map>
#include <stack>

namespace gbe {
	namespace gfx {
		struct TextureData {
			VkImageView textureImageView;
			VkImage textureImage;

			VkDeviceMemory textureImageMemory;
			VkSampler textureSampler;

			VkDescriptorSet DS;

			unsigned int width;
			unsigned int height;
		};

		class TextureLoader : public asset::AssetLoader<asset::Texture, asset::data::TextureImportData, asset::data::TextureLoadData, TextureData> {
		private:
			VkDevice* vkdevice;
			VkPhysicalDevice* vkphysicaldevice;
			TextureData defaultImage;

			static std::function<VkDescriptorSet(VkSampler, VkImageView)> Ui_Callback;
		protected:
			TextureData LoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
			void UnLoadAsset_(asset::Texture* asset, const asset::data::TextureImportData& importdata, asset::data::TextureLoadData* data) override;
		public:
			static TextureData& GetDefaultImage();
			const static void Set_Ui_Callback(std::function<VkDescriptorSet(VkSampler, VkImageView)> func);
			void PassDependencies(VkDevice* vkdevice, VkPhysicalDevice* vkphysicaldevice);
		};
	}
}