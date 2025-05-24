#include "MaterialLoader.h"
#include "MaterialLoader.h"

using namespace gbe;
using namespace gfx;

MaterialData MaterialLoader::LoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) {

	asset->setShader(this->shaderloader->GetAsset(importdata.shader));

	return MaterialData{
		//NONE
	};
}

void MaterialLoader::UnLoadAsset_(asset::Material* asset, const asset::data::MaterialImportData& importdata, asset::data::MaterialLoadData* data) {

}

void gbe::gfx::MaterialLoader::PassDependencies(gbe::gfx::ShaderLoader* shaderloader)
{
	this->shaderloader = shaderloader;
}

