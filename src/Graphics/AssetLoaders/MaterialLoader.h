#pragma once

#include "Asset/gbe_asset.h"
#include "ShaderLoader.h"

namespace gbe {
	namespace gfx {
		class MaterialLoader : public asset::AssetLoader<asset::Material, asset::data::MaterialImportData, asset::data::MaterialLoadData> {
		private:
			ShaderLoader* shaderloader;
		protected:
			bool LoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) override;
			void UnLoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) override;
		public:
			void PassDependencies(ShaderLoader* shaderloader);
		};
	}
}