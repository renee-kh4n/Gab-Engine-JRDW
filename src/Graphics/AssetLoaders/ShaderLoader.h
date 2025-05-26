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
		struct ShaderStageMeta {
			struct TextureMeta {
				std::string name;
				std::string type;
				unsigned int set;
				unsigned int binding;
			};
			struct UboType {
				struct UboTypeMember {
					std::string name;
					std::string type;
					unsigned int offset;
				};

				std::string name;
				std::vector<UboTypeMember> members;
			};
			struct UboMeta {
				std::string name;
				std::string type;
				unsigned int block_size;
				unsigned int set;
				unsigned int binding;
			};
			std::unordered_map<std::string, UboType> types;
			std::vector<TextureMeta> textures;
			std::vector<UboMeta> ubos;
		};

		struct ShaderData {
			struct ShaderBlock {
				std::string name;
				size_t block_size;
				unsigned int set;
				unsigned int binding;
			};

			struct ShaderField {
				std::string name;
				std::string block;
				asset::Shader::UniformFieldType type;
				size_t offset; // Offset in bytes
				size_t size; // Offset in bytes
			};

			std::unordered_map<unsigned int, std::vector<VkDescriptorSetLayoutBinding>> binding_sets;
			std::vector<VkDescriptorSetLayout> descriptorSetLayouts;
			std::vector<ShaderBlock> uniformblocks;
			std::vector<ShaderField> uniformfields;
			VkPipelineLayout pipelineLayout;
			VkPipeline pipeline;
			asset::Shader* asset;

			bool FindUniformField(std::string id, ShaderField& out_field, ShaderBlock& out_block);
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