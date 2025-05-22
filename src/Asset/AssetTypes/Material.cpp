#include "Material.h"

namespace gbe {
	using namespace asset;

	Material::Material(std::string path) : BaseAsset(path)
	{
	}

	void Material::setShader(asset::Shader* shader)
	{
		this->m_shader.Assign(shader);
	}

	asset::Shader* Material::getShader()
	{
		return this->m_shader.Get_asset();
	}

	MaterialOverride::MaterialOverride() {

	}
}