#include "Material.h"

namespace gde {
	using namespace rendering;

	Material::Material(Shader* m_shader)
	{
		this->m_shader = m_shader;
	}

	MaterialOverride::MaterialOverride() {

	}
}