#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"

#include <vulkan/vulkan.h>
#include "TextureLoader.h"
#include <optional>
#include <tuple>

namespace gbe {
	namespace gfx {
		struct ShaderData {
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
		};

		class ShaderLoader : public asset::AssetLoader<asset::Shader, asset::data::ShaderImportData, asset::data::ShaderLoadData> {
		private:
			std::unordered_map<unsigned int, ShaderData> loaded_shaders;
			VkDevice* vkdevice;
			VkExtent2D* vkextent;
			VkRenderPass* vkrenderpass;
			VkShaderModule TryCompileShader(const std::vector<char>& code);
			unsigned int loaded_shaders_count = 0;
		protected:
			bool LoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
			void UnLoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
		public:
			ShaderData& Get_shader(unsigned int id);
			void PassDependencies(VkDevice* vkdevice, VkExtent2D* vkextent, VkRenderPass* vkrenderpass);
		};
	}
}