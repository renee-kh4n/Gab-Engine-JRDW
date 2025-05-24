#pragma once

#include "Asset/gbe_asset.h"
#include "ShaderLoader.h"

namespace gbe {
	namespace gfx {
		class MaterialData {

		};

		class MaterialLoader : public asset::AssetLoader<asset::Material, asset::data::MaterialImportData, asset::data::MaterialLoadData, MaterialData> {
		private:
			ShaderLoader* shaderloader;
		protected:
			MaterialData LoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) override;
			void UnLoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) override;
		public:
			void PassDependencies(ShaderLoader* shaderloader);
		};
	}
}