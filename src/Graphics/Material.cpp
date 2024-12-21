#include "Material.h"

namespace gbe {
	using namespace gfx;

	Material::Material(asset::Shader* m_shader)
	{
		this->m_shader.Assign(m_shader);
	}

	MaterialOverride::MaterialOverride() {

	}
}