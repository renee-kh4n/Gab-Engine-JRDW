#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include "../Data/Vertex.h"

#include <vulkan/vulkan.h>
#include "TextureLoader.h"

#include <optional>
#include <tuple>
#include <iostream>
#include <fstream>
#include <sstream>

namespace gbe {
	namespace gfx {
		struct ShaderData {
			VkDescriptorSetLayout descriptorSetLayout;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			asset::Shader* asset;
		};

		class ShaderLoader : public asset::AssetLoader<asset::Shader, asset::data::ShaderImportData, asset::data::ShaderLoadData, ShaderData> {
		private:
			std::unordered_map<std::string, ShaderData> loaded_shaders;
			VkDevice* vkdevice;
			VkExtent2D* vkextent;
			VkRenderPass* vkrenderpass;
			VkShaderModule TryCompileShader(const std::vector<char>& code);
		protected:
			ShaderData LoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
			void UnLoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
		public:
			void PassDependencies(VkDevice* vkdevice, VkExtent2D* vkextent, VkRenderPass* vkrenderpass);
		};
	}
}