#pragma once

#include "Asset/gbe_asset.h"
#include "Math/gbe_math.h"
#include <glad/glad.h>
#include "TextureLoader.h"

namespace gbe {
	namespace gfx {
		class ShaderLoader : public asset::AssetLoader<asset::Shader, asset::data::ShaderImportData, asset::data::ShaderLoadData> {
		private:
			unsigned int TryCompileShader(std::string path, int compile_type);
		protected:
			bool LoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
			void UnLoadAsset_(asset::Shader* asset, const asset::data::ShaderImportData& importdata, asset::data::ShaderLoadData* data) override;
		};
	}
}